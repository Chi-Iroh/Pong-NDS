#include <algorithm>
#include <cstdlib>
#include <functional>
#include "Ball.hpp"
#include "Coords.hpp"
#include "Layer.hpp"
#include "Rotation.hpp"
#include "ScreenSize.hpp"
#include "SpritePalette.hpp"

Ball::Ball() :
    gfx{ oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_16Color) }
{
    this->reset();
    /**
     * SpriteMapping_1D_32 = 32 KB = 1 tile every 32 bytes (there are 1024 tiles)
     * Our sprite is in 16 colors mode, thus 1 pixel = 1 color = 4 bits (2^4 = 16)
     * A 8x8 sprite contains 64 pixels.
     * The GFX is a u16 pointer, each element can hold 16 bits of data.
     * Each color is held in 4 bits, thus each u16 element of the GFX holds 4 pixels.
     * As each element of the GFX holds 4 pixels, we need 8x8/4 elements, thus iterations.
    **/
    for (unsigned i{ 0 }; i < 8 * 8 / 4; i++) {
        this->gfx[i] = sprite_palette::YELLOW;
        this->gfx[i] = (this->gfx[i] << 4) | sprite_palette::YELLOW;
        this->gfx[i] = (this->gfx[i] << 4) | sprite_palette::YELLOW;
        this->gfx[i] = (this->gfx[i] << 4) | sprite_palette::YELLOW;
    }
}

Ball::~Ball() {
    oamFreeGfx(&oamMain, this->gfx);
}

std::pair<int, int> Ball::pos() const {
    return { this->x, this-> y };
}

std::optional<Direction> Ball::touchedEdge() const {
    if (this->x == 0 || this->x >= WIDTH - SIZE) {
        return Direction::Vertical;
    } else if (this->y == 0 || this->y >= HEIGHT - SIZE) {
        return Direction::Horizontal;
    }
    return std::nullopt;
}

void Ball::rotate(Direction obstacleDirection) {
    std::tie(this->horizontalMove, this->verticalMove) = ::rotate({ this->horizontalMove, this->verticalMove }, obstacleDirection);
}

void Ball::draw() const {
    oamSet(
        &oamMain,                   // Main engine (top screen)
        Sprite::ID,                 // Sprite ID
        this->x,                    // X coord
        this->y,                    // Y coord
        Layer::UPPER_LAYER,         // Priority / layer, see Layer.hpp
        0,                          // If bitmap (NOT in this case) --> alpha (transparency) value
                                    // Otherwise (this case) --> palette index (there are many color palettes for sprites, 16 for 4 bit colors and 16 more for 8 bit colors) in [0;15]
                                    // So here, 0 = first 4 bit (16 color) sprite pamette, as plain colors are used for sprites, one palette can hold them all
        SpriteSize_8x8,             // 8x8 pixels
        SpriteColorFormat_16Color,  // 16 colors palette, thus NOT bitmap
        this->gfx,                  // Sprite memory
        -1,                         // Value about rotation, the sprite isn't rotated, so this value must be < 0 or > 31
        false,                      // true = double size if rotation enabled (see previous argument), false doesn't change anything
        false,                      // true = hide, false = show
        false,                      // true = horizontally mirror, false = don't change
        false,                      // true = vertically mirror, false = don't change
        false                       // true = mosaic mode (color shading), false = don't change
    );
}

void Ball::forward() {
    const std::optional edge{ this->touchedEdge() };
    if (edge.has_value()) {
        this->rotate(edge.value());
    }
    this->x = std::clamp(this->x + this->horizontalMove, 0, MAX_X);
    this->y = std::clamp(this->y + this->verticalMove, 0, MAX_Y);
}

void Ball::reset() {
    this->horizontalMove = std::rand() % 2 ? 1 : -1;
    this->verticalMove = std::rand() % 2 ? 1 : -1;
    this->x = MAX_X / 2;
    this->y = MAX_Y / 2;
}
