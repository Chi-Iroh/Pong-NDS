#include <nds.h>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include "Ball.hpp"
#include "Coords.hpp"
#include "Palette.hpp"
#include "Rotation.hpp"
#include "ScreenSize.hpp"

Ball::Ball() {
    this->reset();
}

std::pair<unsigned, unsigned> Ball::pos() const {
    return { this->x, this-> y };
}

std::optional<Direction> Ball::touchedEdge() const {
    if (this->x == 0 || this->x == MAX_X) {
        return Direction::Vertical;
    } else if (this->y == 0 || this->y == MAX_Y) {
        return Direction::Hozirontal;
    }
    return std::nullopt;
}

void Ball::rotate(Direction obstacleDirection) {
    std::tie(this->horizontalMove, this->verticalMove) = ::rotate({ this->horizontalMove, this->verticalMove }, obstacleDirection);
}

void Ball::draw() const {
    ((u16*)BG_MAP_RAM(0))[this->y * WIDTH_N_TILES + this->x] = palette::Color::YELLOW;
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