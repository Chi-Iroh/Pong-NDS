#include <array>
#include <utility>

#include "DigitSprites.hpp"
#include "Layer.hpp"
#include "ScoreBoard.hpp"

static constexpr std::array<std::pair<const unsigned(*)[512], int>, 10> DIGIT_TILES_AND_LEN{
    std::pair{ &zeroTiles, zeroTilesLen },
    std::pair{ &oneTiles, oneTilesLen },
    std::pair{ &twoTiles, twoTilesLen },
    std::pair{ &threeTiles, threeTilesLen },
    std::pair{ &fourTiles, fourTilesLen },
    std::pair{ &fiveTiles, fiveTilesLen },
    std::pair{ &sixTiles, sixTilesLen },
    std::pair{ &sevenTiles, sevenTilesLen },
    std::pair{ &eightTiles, eightTilesLen },
    std::pair{ &nineTiles, nineTilesLen }
};

using Digits = std::array<u16*, 10>;
Digits digitsPlayer{};
Digits digitsEnemy{};

static constexpr int digitPlayerOAM_ID(int score) {
    sassert(score < 10, "Score can only be in [0;9] but got %u !\n", score);
    return score;
}

static constexpr int digitEnemyOAM_ID(int score) {
    sassert(score < 10, "Score can only be in [0;9] but got %u !\n", score);
    return 10 + score;
}

void initDigits() {
    dmaCopy(zeroPal, SPRITE_PALETTE_SUB, zeroPalLen); // all digits are black and transparent, thus same palette for every digit
    for (unsigned i{ 0 }; i < 10; i++) {
        digitsPlayer[i] = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_16Color);
        dmaCopy(*DIGIT_TILES_AND_LEN[i].first, digitsPlayer[i], DIGIT_TILES_AND_LEN[i].second);
    }
    for (unsigned i{ 0 }; i < 10; i++) {
        digitsEnemy[i] = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_16Color);
        dmaCopy(*DIGIT_TILES_AND_LEN[i].first, digitsEnemy[i], DIGIT_TILES_AND_LEN[i].second);
    }
}

void freeDigits() {
    for (unsigned i{ 0 }; i < 10; i++) {
        oamFreeGfx(&oamSub, digitsEnemy[i]);
        oamFreeGfx(&oamSub, digitsPlayer[i]);
    }
}

void displayPlayerScore(unsigned score) {
    sassert(score <= 9, "Score can only be in [0;9] but got %u !\n", score);
    for (unsigned i{ 0 }; u16* digitGfx : digitsPlayer) {
        oamSet(
            &oamSub,
            digitPlayerOAM_ID(i),
            SCOREBOARD_PLAYER_X_OFFSET,
            SCOREBOARD_PLAYER_Y_OFFSET,
            Layer::UPPER_LAYER,
            0,
            SpriteSize_64x64,
            SpriteColorFormat_16Color,
            digitGfx,
            -1,
            false,
            i != score, // hide unused digits
            false,
            false,
            false
        );
        i++;
    }
}

void displayEnemyScore(unsigned score) {
    sassert(score <= 9, "Score can only be in [0;9] but got %u !\n", score);
    for (unsigned i{ 0 }; u16* digitGfx : digitsEnemy) {
        oamSet(
            &oamSub,
            digitEnemyOAM_ID(i),
            SCOREBOARD_AI_X_OFFSET,
            SCOREBOARD_AI_Y_OFFSET,
            Layer::UPPER_LAYER,
            0,
            SpriteSize_64x64,
            SpriteColorFormat_16Color,
            digitGfx,
            -1,
            false,
            i != score, // hide unused digits
            false,
            false,
            false
        );
        i++;
    }
}