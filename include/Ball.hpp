#pragma once
#include <optional>
#include <utility>
#include "Player.hpp"
#include "Rotation.hpp"

class Ball {
    int horizontalMove;
    int verticalMove;

    unsigned x;
    unsigned y;

public:
    Ball();

    std::pair<unsigned, unsigned> pos() const;

    std::optional<Direction> touchedEdge() const;

    void rotate(Direction obstacleDirection);

    void draw() const;

    void forward();

    void reset();

    std::optional<Player> inPlayerZone() const;
};