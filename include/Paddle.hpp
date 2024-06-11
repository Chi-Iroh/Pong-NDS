#pragma once
#include <nds.h>
#include <optional>
#include <utility>
#include "Rotation.hpp"
#include "ScreenSize.hpp"

class Paddle {
public:
    enum class Type {
        PLAYER,
        ENEMY
    };

private:
    unsigned x;
    unsigned y[3];
    const unsigned paletteIndex;

public:
    Paddle(Type type);

    void setPosition(unsigned x, unsigned y);
    void move(int x, int y);
    void draw() const;
    std::optional<Direction> intersects(const std::pair<unsigned, unsigned>& coords) const;

    inline std::pair<unsigned, unsigned> pos() const {
        return { this->x, this->y[1] };
    }
};