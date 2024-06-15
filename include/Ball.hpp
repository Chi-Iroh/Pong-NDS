#pragma once
#include <nds.h>
#include <optional>
#include <utility>
#include "Player.hpp"
#include "Rotation.hpp"
#include "Sprite.hpp"

class Ball : public Sprite {
    int horizontalMove;
    int verticalMove;

    unsigned x;
    unsigned y;

    u16* gfx{ nullptr };

public:
    Ball();
    ~Ball();

    std::pair<unsigned, unsigned> pos() const;

    std::optional<Direction> touchedEdge() const;

    void rotate(Direction obstacleDirection);

    void draw() const;

    void forward();

    void reset();

    std::optional<Player> inPlayerZone() const;
};