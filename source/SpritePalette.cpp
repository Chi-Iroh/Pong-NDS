#include "SpritePalette.hpp"

#include <nds.h>

namespace sprite_palette {
    void init() {
        SPRITE_PALETTE[sprite_palette::Color::BLACK] = RGB5(0, 0, 0);
        SPRITE_PALETTE[sprite_palette::Color::BLUE] = RGB5(0, 0, 31);
        SPRITE_PALETTE[sprite_palette::Color::RED] = RGB5(31, 0, 0);
        SPRITE_PALETTE[sprite_palette::Color::YELLOW] = RGB5(31, 31, 0);
        SPRITE_PALETTE[sprite_palette::Color::DEBUG_PINK] = RGB5(31, 0, 31);
    }
}