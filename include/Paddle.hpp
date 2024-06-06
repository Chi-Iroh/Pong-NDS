#pragma once
#include <nds.h>
#include <optional>
#include <utility>
#include "Rotation.hpp"
#include "ScreenSize.hpp"

class Paddle {
    unsigned x;
    unsigned y[3];
    const unsigned paletteIndex;

public:
    Paddle(unsigned paletteIndex, unsigned x, unsigned y = MAX_Y / 2);

    void setPosition(unsigned x, unsigned y);
    void move(int x, int y);
    void draw() const;
    std::optional<Direction> intersects(const std::pair<unsigned, unsigned>& coords) const;

    inline std::pair<unsigned, unsigned> pos() const {
        return { this->x, this->y[1] };
    }
};