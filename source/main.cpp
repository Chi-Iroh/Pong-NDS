#include <nds.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Palette.hpp"
#include "ScreenSize.hpp"
#include "background.h"

Ball* ball{ nullptr };
Paddle* player{ nullptr };
Paddle* enemy{ nullptr };

static void movePlayer() {
    scanKeys();
    const u32 keys{ keysHeld() | keysDown() };

    if ((keys & KEY_DOWN)) {
        player->move(0, UNIT_SIZE);
    } else if ((keys & KEY_UP)) {
        player->move(0, -UNIT_SIZE);
    }

    const int ballY{ ball->pos().second };
    const int enemyY{ enemy->pos().second };
    if (enemyY > ballY) {
        enemy->move(0, -UNIT_SIZE);
    } else if (enemyY < ballY) {
        enemy->move(0, UNIT_SIZE);
    }
}

static void moveBall() {
    static unsigned playerScore{ 0 };
    static unsigned enemyScore{ 0 };

    bool isThereGoal{ true };
    if (player->isInGoalZone(*ball)) {
        enemyScore++; // ball is in player territory --> enemy scored 1 point
    } else if (enemy->isInGoalZone(*ball)) {
        playerScore++;
    } else {
        isThereGoal = false;
    }

    if (isThereGoal) {
        ball->reset();
        iprintf("Player %u | Enemy %u\n", playerScore, enemyScore);
    }

    const auto coords{ ball->pos() };
    if (const std::optional<Direction> playerIntersect{ player->intersects(coords) }; playerIntersect.has_value()) {
        ball->rotate(playerIntersect.value());
    } else if (const std::optional<Direction> enemyIntersect{ enemy->intersects(coords) }; enemyIntersect.has_value()) {
        ball->rotate(enemyIntersect.value());
    }
    ball->forward();
}

int main() {
    std::srand(std::time(nullptr));

    // Background has 256 colors, so it is an extended palette
    // Mode 5 supports extended backgrounds on layers 2 and 3, so bgInit must be called with 2 or 3 for the layer
    videoSetMode(MODE_5_2D);

    // full addresses are specified so we can easily see it there is an overwrite
    // see on the VRAM selector : https://mtheall.com/banks.html#A=MBG0&B=MOBJ0
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);

    lcdMainOnTop();
    consoleDemoInit();
    palette::init();
    oamInit(&oamMain, SpriteMapping_1D_32, false);

    Paddle player{ Paddle::Type::PLAYER };
    Paddle enemy{ Paddle::Type::ENEMY };
    Ball ball{};

    ::ball = &ball;
    ::player = &player;
    ::enemy = &enemy;

    const int bg{ bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0) };
    dmaCopy(backgroundBitmap, bgGetGfxPtr(bg), backgroundBitmapLen);
    dmaCopy(backgroundPal, BG_PALETTE, backgroundPalLen);

    iprintf("Player 0 | Enemy 0\n");
    timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(10), movePlayer);
    timerStart(1, ClockDivider_1024, TIMER_FREQ_1024(60), moveBall);

    while (true) {
        swiWaitForVBlank();
        player.draw();
        enemy.draw();
        ball.draw();
        oamUpdate(&oamMain);
    }
}