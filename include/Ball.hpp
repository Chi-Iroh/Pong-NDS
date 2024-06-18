#pragma once
#include <optional>
#include <utility>

#include <nds.h>

#include "ScreenSize.hpp"
#include "Player.hpp"
#include "Rotation.hpp"
#include "Sprite.hpp"

class Ball : public Sprite {
    int horizontalMove;
    int verticalMove;

    int x;
    int y;

    u16* gfx{ nullptr };

public:
    static constexpr int SIZE{ UNIT_SIZE };

    Ball();
    ~Ball();

    std::pair<int, int> pos() const;

    std::optional<Direction> touchedEdge() const;

    void rotate(Direction obstacleDirection);

    void draw() const;

    void forward();

    void reset();
};