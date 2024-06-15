#include <nds.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Palette.hpp"
#include "ScreenSize.hpp"

Ball* ball{ nullptr };
Paddle* player{ nullptr };
Paddle* enemy{ nullptr };

static void updateCoords() {
    static unsigned playerScore{ 0 };
    static unsigned enemyScore{ 0 };

    if (const std::optional<Player> playerZone{ ball->inPlayerZone() }; playerZone.has_value()) {
        ball->reset();
        if (playerZone.value() == Player::Player) {
            enemyScore++; // ball is in player territory --> enemy scored 1 point
        } else {
            playerScore++;
        }
        iprintf("Player %u | Enemy %u\n", playerScore, enemyScore);
    }

    scanKeys();
    const u32 keys{ keysHeld() | keysDown() };

    if ((keys & KEY_DOWN)) {
        player->move(0, 1);
    } else if ((keys & KEY_UP)) {
        player->move(0, -1);
    }

    const unsigned ballY{ ball->pos().second };
    const unsigned enemyY{ enemy->pos().second };
    if (enemyY > ballY) {
        enemy->move(0, -1);
    } else if (enemyY < ballY) {
        enemy->move(0, 1);
    }

    const auto coords{ ball->pos() };
    if (const std::optional playerIntersect{ player->intersects(coords) }; playerIntersect.has_value()) {
        ball->rotate(playerIntersect.value());
    } else if (const std::optional enemyIntersect{ enemy->intersects(coords) }; enemyIntersect.has_value()) {
        ball->rotate(enemyIntersect.value());
    }
    ball->forward();
}

int main() {
    std::srand(std::time(nullptr));
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);

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

    iprintf("Player 0 | Enemy 0\n");
    timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(10), updateCoords);

    while (true) {
        swiWaitForVBlank();
        player.draw();
        enemy.draw();
        ball.draw();
        oamUpdate(&oamMain);
    }
}