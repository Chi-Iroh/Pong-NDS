#include <algorithm>
#include <cmath>
#include "Coords.hpp"
#include "Paddle.hpp"
#include "Palette.hpp"
#include "ScreenSize.hpp"

Paddle::Paddle(Type type) :
    paletteIndex{ type == Type::ENEMY ? palette::RED : palette::BLUE }
{
    if (type == Type::ENEMY) {
        this->setPosition(MAX_X - 1, MAX_Y / 2);
    } else {
        this->setPosition(1, MAX_Y / 2);
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
        ((u16*)BG_MAP_RAM(0))[this->y[i] * 32 + this->x] = this->paletteIndex;
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
