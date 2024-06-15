#include <nds.h>
#include "Palette.hpp"

namespace palette {
    void init() {
        SPRITE_PALETTE[palette::Color::EMPTY] = RGB5(0, 0, 0);
        SPRITE_PALETTE[palette::Color::BLUE] = RGB5(0, 0, 31);
        SPRITE_PALETTE[palette::Color::RED] = RGB5(31, 0, 0);
        SPRITE_PALETTE[palette::Color::YELLOW] = RGB5(31, 31, 0);
        SPRITE_PALETTE[palette::Color::DEBUG_PINK] = RGB5(31, 0, 31);
    }
}