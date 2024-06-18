#include <algorithm>
#include <cmath>
#include "Coords.hpp"
#include "Paddle.hpp"
#include "Palette.hpp"
#include "ScreenSize.hpp"

Paddle::Paddle(Type type) :
    paletteIndex{ type == Type::ENEMY ? palette::RED : palette::BLUE }
{
    for (u16*& gfx : this->gfx) {
        gfx = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_16Color);

        // SpriteMapping_1D_32 = 32 KB = 1 tile every 32 bytes (there are 1024 tiles)
        // Our sprite is in 16 colors mode, thus 1 pixel = 1 color = 4 bits (2^4 = 16)
        // A 8x8 sprite contains 64 pixels.
        // The GFX is a u16 pointer, each element can hold 16 bits of data.
        // Each color is held in 4 bits, thus each u16 element of the GFX holds 4 pixels.
        // As each element of the GFX holds 4 pixels, we need 8x8/4 elements, thus iterations.
        for (unsigned i{ 0 }; i < 8 * 8 / 4; i++) {
            gfx[i] = this->paletteIndex;
            gfx[i] = (gfx[i] << 4) | this->paletteIndex;
            gfx[i] = (gfx[i] << 4) | this->paletteIndex;
            gfx[i] = (gfx[i] << 4) | this->paletteIndex;
        }
    }
    if (type == Type::ENEMY) {
        this->setPosition(MAX_X - WIDTH, MAX_Y / 2);
    } else {
        this->setPosition(WIDTH, MAX_Y / 2);
    }
}

Paddle::~Paddle() {
    for (u16* gfx : this->gfx) {
        oamFreeGfx(&oamMain, gfx);
    }
}

void Paddle::setPosition(int x, int y) {
    this->x = std::min(x, MAX_X);
    y = std::min(y, MAX_Y - Paddle::WIDTH);

    this->y[0] = modulus(y - Paddle::SINGLE_HEIGHT, MAX_Y);
    this->y[1] = y;
    this->y[2] = modulus(y + Paddle::SINGLE_HEIGHT, MAX_Y);
}

void Paddle::move(int x, int y) {
    this->x = modulus(this->x + x, MAX_X);
    for (unsigned i{ 0 }; i < 3; i++) {
        this->y[i] = modulus(this->y[i] + y, MAX_Y);
    }
}

void Paddle::draw() const {
    for (unsigned i{ 0 }; i < 3; i++) {
        oamSet(
            &oamMain,
            this->sprites[i].ID,
            this->x,
            this->y[i],
            0,
            0,
            SpriteSize_8x8,
            SpriteColorFormat_16Color,
            this->gfx[i],
            -1,
            false,
            false,
            false,
            false,
            false
        );
    }
}

std::optional<Direction> Paddle::intersects(const std::pair<int, int>& coords) const {
    if (coords.first == this->x && (coords.second == modulus(this->y[0] - 1, MAX_Y) || coords.second == modulus(this->y[2] + 1, MAX_Y))) {
        return Direction::Horizontal;
    }
    for (unsigned i{ 0 }; i < 3; i++) {
        if (coords.second == this->y[i] && (coords.first == modulus(this->x - 1, MAX_X) || coords.first == modulus(this->x + 1, MAX_X))) {
            return Direction::Vertical;
        }
    }
    return std::nullopt;
}
