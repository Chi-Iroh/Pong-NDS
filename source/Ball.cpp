#include <algorithm>
#include <cstdlib>
#include <functional>
#include "Ball.hpp"
#include "Coords.hpp"
#include "Palette.hpp"
#include "Rotation.hpp"
#include "ScreenSize.hpp"

Ball::Ball() :
    gfx{ oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_16Color) }
{
    this->reset();
    // SpriteMapping_1D_32 = 32 KB = 1 tile every 32 bytes (there are 1024 tiles)
    // Our sprite is in 16 colors mode, thus 1 pixel = 1 color = 4 bits (2^4 = 16)
    // A 8x8 sprite contains 64 pixels.
    // The GFX is a u16 pointer, each element can hold 16 bits of data.
    // Each color is held in 4 bits, thus each u16 element of the GFX holds 4 pixels.
    // As each element of the GFX holds 4 pixels, we need 8x8/4 elements / iterations.
    for (unsigned i{ 0 }; i < 8 * 8 / 4; i++) {
        this->gfx[i] = palette::YELLOW;
        this->gfx[i] = (this->gfx[i] << 4) | palette::YELLOW;
        this->gfx[i] = (this->gfx[i] << 4) | palette::YELLOW;
        this->gfx[i] = (this->gfx[i] << 4) | palette::YELLOW;
    }
}

Ball::~Ball() {
    oamFreeGfx(&oamMain, this->gfx);
}

std::pair<unsigned, unsigned> Ball::pos() const {
    return { this->x, this-> y };
}

std::optional<Direction> Ball::touchedEdge() const {
    if (this->x == 0 || this->x == MAX_X) {
        return Direction::Vertical;
    } else if (this->y == 0 || this->y == MAX_Y) {
        return Direction::Horizontal;
    }
    return std::nullopt;
}

void Ball::rotate(Direction obstacleDirection) {
    std::tie(this->horizontalMove, this->verticalMove) = ::rotate({ this->horizontalMove, this->verticalMove }, obstacleDirection);
}

void Ball::draw() const {
    oamSet(
        &oamMain,
        Sprite::ID,
        this->x,
        this->y,
        0,
        0,
        SpriteSize_8x8,
        SpriteColorFormat_16Color,
        this->gfx,
        -1,
        false,
        false,
        false,
        false,
        false
    );
}

void Ball::forward() {
    const std::optional edge{ this->touchedEdge() };
    if (edge.has_value()) {
        this->rotate(edge.value());
    }
    if (this->horizontalMove == 1) {
        this->x = add(this->x, 1, MAX_X);
    } else {
        this->x = sub(this->x, 1, MAX_X);
    }
    if (this->verticalMove == 1) {
        this->y = add(this->y, 1, MAX_Y);
    } else {
        this->y = sub(this->y, 1, MAX_Y);
    }
}

void Ball::reset() {
    this->horizontalMove = std::rand() % 2 ? 1 : -1;
    this->verticalMove = std::rand() % 2 ? 1 : -1;
    this->x = MAX_X / 2;
    this->y = MAX_Y / 2;
}

std::optional<Player> Ball::inPlayerZone() const {
    if (this->x == 0) {
        return Player::Player;
    } else if (this->x == MAX_X) {
        return Player::Enemy;
    }
    return std::nullopt;
}