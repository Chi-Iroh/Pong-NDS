#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <nds.h>

#include "Ball.hpp"
#include "Layer.hpp"
#include "Paddle.hpp"
#include "ScreenSize.hpp"
#include "Sound.hpp"
#include "SpritePalette.hpp"

#include "background.h"
#include "beep.h"
#include "bgm.h"
#include "lose_point.h"
#include "win_point.h"

/**
 * timerStart has a void(void) callback, so to pass data to it, we store their address as global variables.
 * We store pointers and not copies, to avoid defining copy-constructors, because Sprite class assigns unique IDs and isn't very practical to provide a relevant copy-constructor.
*/

static Ball* ball{ nullptr };
static Paddle* player{ nullptr };
static Paddle* enemy{ nullptr };

std::atomic<void*> soundToPlay{ nullptr };

static void checkCollisions() {
    const auto coords{ ball->pos() };
    if (const std::optional<Direction> playerIntersect{ player->intersects(coords) }; playerIntersect.has_value()) {
        ball->rotate(playerIntersect.value());
        soundToPlay = beep_raw;
        ball->forward();
    } else if (const std::optional<Direction> enemyIntersect{ enemy->intersects(coords) }; enemyIntersect.has_value()) {
        ball->rotate(enemyIntersect.value());
        soundToPlay = beep_raw;
        ball->forward();
    }
}

static void movePlayer() {
    // Updates keys status, NOT stylus status.
    scanKeys();
    // Get both held keys and only pressed keys.
    const u32 keys{ keysHeld() | keysDown() };

    if (!(keys & KEY_DOWN) && !(keys & KEY_UP)) {
        return;
    }

    for (int i{ 0 }; i < 8; i++) {
        player->move(0, (keys & KEY_UP) ? -1 : 1);
        checkCollisions();
    }
}

static void moveEnemy() {
    const int ballY{ ball->pos().second };
    const int enemyY{ enemy->pos().second };
    const int direction{ enemyY > ballY ? -1 : 1 };

    for (int i{ 0 }; i < UNIT_SIZE; i++) {
        enemy->move(0, direction);
        checkCollisions();
    }
}

static void moveBall() {
    static unsigned playerScore{ 0 };
    static unsigned enemyScore{ 0 };

    bool isThereGoal{ true };
    if (player->isInGoalZone(*ball)) {
        enemyScore++; // ball is in player territory --> enemy scored 1 point
        soundToPlay = lose_point_raw;
    } else if (enemy->isInGoalZone(*ball)) {
        playerScore++;
        soundToPlay = win_point_raw;
    } else {
        isThereGoal = false;
    }

    if (isThereGoal) {
        ball->reset();
        iprintf("Player %u | Enemy %u\n", playerScore, enemyScore);
    }

    checkCollisions();
    ball->forward();
}

int main() {
    std::srand(std::time(nullptr));

    // Mode 5 supports simple (text) backgrounds on layers 0 and 1, extended backgrounds on layers 2 and 3, so bgInit must be called with 2 or 3 for the layer
    videoSetMode(MODE_5_2D);

    /**
     * Full addresses are specified so we can easily see it there is an overwrite
     * See on the VRAM selector : https://mtheall.com/banks.html#A=MBG0&B=MOBJ0
    **/
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);

    /**
     * The NDS has 2 engines, the main and the sub engine. These two have different capabilities, heavily described in sverx's tutorial (video.html).
     * Assigns the main engine to the top screen.
    **/
    lcdMainOnTop();

    // Initializes the bottom screen such as we can write text on it, using iprintf() function
    consoleDemoInit();

    /**
     * Initializes the OAM, the sprite manager (see sprite.html) and links it to the main engine (top screen then)
     * SpriteMapping_1D_32 : sprites are represented as 1D (linear) array of tiles, and each tile is 32 bytes
     * false : no extended palette (not 256 colors, only 16), thus sprites tiles use 4-bit colors
    **/
    oamInit(&oamMain, SpriteMapping_1D_32, false);
    sprite_palette::init();

    Paddle player{ Paddle::Type::PLAYER };
    Paddle enemy{ Paddle::Type::ENEMY };
    Ball ball{};

    ::ball = &ball;
    ::player = &player;
    ::enemy = &enemy;

    soundEnable();

    /**
     * A bitmap background consists of many 8x8 pixels tiles, with each pixel represented by a number, an offset in a color list, called color palette.
     * Palette is either 16, or 256 colors (256 = extended palette). 16 colors means each pixel is encoded in 4 bits (2^4 = 16), or 8 bits if 256 colors (256 = 2^8).
     *
     * Initializes the video memory to hold a background on layer/background 2
     * As we previously initialized the video mode (with videoSetMode(MODE_5_2D)) to mode 5
     * Mode 5 supports extended background (with 256 colors) on layer 2 and 3, 2 was arbitrarily chosen, but 3 will also work
     * BgType_Bmp8 = 8-bit colors (thus, extended palette)
     * BgSize_B8_256x256 = 8bit bitmap, reserves space for 256x256 pixels. The used background is 256x192, it fits in 256x256.
     * Last, the 0, 0 are offsets for the bitmap and its related palette, it must be used when dealing with multiple backgrounds,
     *      otherwise they will overlap on each other and weird pixels will appear.
    **/
    const int bg{ bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0) };
    /**
     * If not set, the priority defaults to 0 (the upper layer), so here we set a lower priority.
     * If not, we would be forced to draw all sprites to the upper layer to see them, otherwise this background will be drawn in on them.
    **/
    bgSetPriority(bg, Layer::LOWER_LAYER);
    // Copies the background bitmap to the video memory associated to the background (address given by bgGetGfxPtr), using the DMA (memory management unit)
    dmaCopy(backgroundBitmap, bgGetGfxPtr(bg), backgroundBitmapLen);
    // The same thing for the background color palette
    dmaCopy(backgroundPal, BG_PALETTE, backgroundPalLen);

    // Writes text on bottom screen.
    iprintf("Player 0 | Enemy 0\n");
    /**
     * There are 4 timers on the NDS.
     * Will call movePlayer 15 times per second.
     * Clock_Divider_XXX is a divider factor, the bigger it is, the more accurate it is. 1024 = 1024 divisions of a second, but 32 = 32 divisions of a second, less precise.
    */
    timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(15), movePlayer);
    timerStart(1, ClockDivider_1024, TIMER_FREQ_1024(10), moveEnemy);
    timerStart(2, ClockDivider_1024, TIMER_FREQ_1024(60), moveBall);

    soundPlaySample(bgm_raw, SoundFormat_16Bit, bgm_raw_len, SOUND_FREQUENCY, 127, 64, true, 0);

    while (true) {
        // Waits for a screen refresh.
        swiWaitForVBlank();
        player.draw();
        enemy.draw();
        ball.draw();
        // Updates (refresh) the sprites of the main engine (top screen).
        oamUpdate(&oamMain);
        if (soundToPlay) {
            soundPlaySample(soundToPlay.load(), SoundFormat_16Bit, beep_raw_len, SOUND_FREQUENCY, 127, 64, false, 0);
            soundToPlay = nullptr;
        }
    }
}