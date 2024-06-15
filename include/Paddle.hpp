#pragma once
#include <nds.h>
#include <array>
#include <optional>
#include <utility>
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
    unsigned x;
    std::array<unsigned, 3> y;
    const std::array<Sprite, 3> sprites{};
    std::array<u16*, 3> gfx;
    const unsigned paletteIndex;

public:
    Paddle(Type type);
    ~Paddle();

    void setPosition(unsigned x, unsigned y);
    void move(int x, int y);
    void draw() const;
    std::optional<Direction> intersects(const std::pair<unsigned, unsigned>& coords) const;

    inline std::pair<unsigned, unsigned> pos() const {
        return { this->x, this->y[1] };
    }
};