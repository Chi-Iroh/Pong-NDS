#pragma once
#include <nds.h>
#include <array>
#include <optional>
#include <utility>
#include "Ball.hpp"
#include "Rotation.hpp"
#include "ScreenSize.hpp"
#include "Sprite.hpp"

class Paddle {
public:
    enum class Type {
        PLAYER,
        ENEMY
    };

private:
    int x;
    std::array<int, 3> y;
    const std::array<Sprite, 3> sprites{};
    std::array<u16*, 3> gfx;
    const unsigned paletteIndex;

    static constexpr int WIDTH{ UNIT_SIZE };
    static constexpr int SINGLE_HEIGHT{ WIDTH };
    static constexpr int HEIGHT{ 3 * SINGLE_HEIGHT };

public:
    Paddle(Type type);
    ~Paddle();

    void setPosition(int x, int y);
    void move(int x, int y);
    void draw() const;
    std::optional<Direction> intersects(const std::pair<int, int>& coords) const;

    inline std::pair<int, int> pos() const {
        return { this->x, this->y[1] };
    }
};