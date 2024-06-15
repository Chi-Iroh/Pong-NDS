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
        // As each element of the GFX holds 4 pixels, we need 8x8/4 elements / iterations.
        for (unsigned i{ 0 }; i < 8 * 8 / 4; i++) {
            gfx[i] = this->paletteIndex;
            gfx[i] = (gfx[i] << 4) | this->paletteIndex;
            gfx[i] = (gfx[i] << 4) | this->paletteIndex;
            gfx[i] = (gfx[i] << 4) | this->paletteIndex;
        }
    }
    if (type == Type::ENEMY) {
        this->setPosition(MAX_X - 1, MAX_Y / 2);
    } else {
        this->setPosition(1, MAX_Y / 2);
    }
}

Paddle::~Paddle() {
    for (u16* gfx : this->gfx) {
        oamFreeGfx(&oamMain, gfx);
    }
}

void Paddle::setPosition(unsigned x, unsigned y) {
    this->x = std::min(x, MAX_X);
    this->y[0] = sub(y, 1, MAX_Y);
    this->y[1] = std::min(y, MAX_Y);
    this->y[2] = add(y, 1, MAX_Y);
}

void Paddle::move(int x, int y) {
    const unsigned absY{ static_cast<unsigned>(std::abs(y)) };
    this->x = add(this->x, x, MAX_X);
    for (unsigned i{ 0 }; i < 3; i++) {
        if (y > 0) {
            this->y[i] = add(this->y[i], absY, MAX_Y);
        } else {
            this->y[i] = sub(this->y[i], absY, MAX_Y);
        }
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

std::optional<Direction> Paddle::intersects(const std::pair<unsigned, unsigned>& coords) const {
    if (coords.first == this->x && (coords.second == sub(this->y[0], 1, MAX_Y) || coords.second == add(this->y[2], 1, MAX_Y))) {
        return Direction::Horizontal;
    }
    for (unsigned i{ 0 }; i < 3; i++) {
        if (coords.second == this->y[i] && (coords.first == sub(this->x, 1, MAX_X) || coords.first == add(this->x, 1, MAX_X))) {
            return Direction::Vertical;
        }
    }
    return std::nullopt;
}
