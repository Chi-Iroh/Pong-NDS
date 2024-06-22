#include <algorithm>
#include <cmath>
#include "Coords.hpp"
#include "Layer.hpp"
#include "Paddle.hpp"
#include "ScreenSize.hpp"
#include "SpritePalette.hpp"

Paddle::Paddle(Type type) :
    type{ type },
    paletteIndex{ type == Type::ENEMY ? sprite_palette::RED : sprite_palette::BLUE }
{
    for (u16*& gfx : this->gfx) {
        // Allocates some memory for a 8x8 pixels sprite, with a 16-colors palette
        gfx = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_16Color);

        /**
         * SpriteMapping_1D_32 = 32 KB = 1 tile every 32 bytes (there are 1024 tiles)
         * Our sprite is in 16 colors mode, thus 1 pixel = 1 color = 4 bits (2^4 = 16)
         * A 8x8 sprite contains 64 pixels.
         * The GFX is a u16 pointer, each element can hold 16 bits of data.
         * Each color is held in 4 bits, thus each u16 element of the GFX holds 4 pixels.
         * As each element of the GFX holds 4 pixels, we need 8x8/4 elements, thus iterations.
        **/
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
        // Frees the sprite memory.
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
            &oamMain,                   // Main engine (top screen)
            this->sprites[i].ID,        // Sprite ID
            this->x,                    // X coord
            this->y[i],                 // Y coord
            Layer::UPPER_LAYER,         // Prioriry / layer, see Layer.hpp
            0,                          // If bitmap (NOT in this case) --> alpha (transparency) value
                                        // Otherwise (this case) --> palette index (there are many color palettes for sprites, 16 for 4 bit colors and 16 more for 8 bit colors) in [0;15]
                                        // So here, 0 = first 4 bit (16 color) sprite pamette, as plain colors are used for sprites, one palette can hold them all
            SpriteSize_8x8,             // 8x8 pixels
            SpriteColorFormat_16Color,  // 16 colors palette, thus NOT bitmap
            this->gfx[i],               // Sprite memory
            -1,                         // Value about rotation, the sprite isn't rotated, so this value must be < 0 or > 31
            false,                      // true = double size if rotation enabled (see previous argument), false doesn't change anything
            false,                      // true = hide, false = show
            false,                      // true = horizontally mirror, false = don't change
            false,                      // true = vertically mirror, false = don't change
            false                       // true = mosaic mode (color shading), false = don't change
        );
    }
}

bool Paddle::isInXRange(int x) const {
    return this->x <= x && x <= this->x + WIDTH;
}

bool Paddle::isInYRange(int y) const {
    for (int paddleY : this->y) {
        const int diff{ y - paddleY };
        if (0 <= diff && diff <= SINGLE_HEIGHT) {
            return true;
        }
    }
    return false;
}

bool Paddle::collidesOnTopBottom(const std::pair<int, int>& coords) const {
    const auto [x, y] = coords;
    return this->isInXRange(x) && (y == this->y[0] - 1 || y == this->y[2] + 1);
}

bool Paddle::collidesOnRightLeft(const std::pair<int, int>& coords) const {
    const auto [x, y] = coords;
    return this->isInYRange(y) && (x == this->x - WIDTH || x == this->x + WIDTH);
    // this->x - WIDTH because if this->x == ballX, they are on top of each other
}

std::optional<Direction> Paddle::intersects(const std::pair<int, int>& coords) const {
    if (this->collidesOnTopBottom(coords)) {
        return Direction::Horizontal;
    } else if (this->collidesOnRightLeft(coords)) {
        return Direction::Vertical;
    }
    return std::nullopt;
}

bool Paddle::isInGoalZone(const Ball& ball) const {
    const int ballX{ ball.pos().first };

    if (this->type == Type::PLAYER) {
        return ballX == 0;
    } else {
        return ballX == MAX_X;
    }
}
