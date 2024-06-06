#include <nds.h>
#include "Palette.hpp"

static void makeTile(u32 tile[8], unsigned paletteIndex) {
    for (unsigned n{ 0 }; n < 8; n++) {
        tile[n] = 0;
        for (unsigned i{ 0 }; i < 8; i++) {
            tile[n] <<= 4;
            tile[n] |= paletteIndex;
        }
    }
}

namespace palette {
    void init() {
        u32 tile[palette::Color::MAX][8];

        for (unsigned paletteIndex{ 0 }; paletteIndex < palette::Color::MAX; paletteIndex++) {
            makeTile(tile[paletteIndex], paletteIndex);
            swiCopy(tile[paletteIndex], ((u8*)BG_TILE_RAM(1)) + (32 * paletteIndex), 32);
        }
        BG_PALETTE[palette::Color::EMPTY] = RGB5(0, 0, 0);
        BG_PALETTE[palette::Color::BLUE] = RGB5(0, 0, 31);
        BG_PALETTE[palette::Color::RED] = RGB5(31, 0, 0);
        BG_PALETTE[palette::Color::YELLOW] = RGB5(31, 31, 0);
        BG_PALETTE[palette::Color::DEBUG_PINK] = RGB5(31, 0, 31);
    }
}