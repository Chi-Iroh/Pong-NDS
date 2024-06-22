#pragma once

namespace sprite_palette {
    enum Color : unsigned {
        BLACK,
        BLUE,
        RED,
        YELLOW,
        // For debug purposes
        DEBUG_PINK,
        // Not assigned in the palette, acts as a sentinel value if needed to iterate on color values
        MAX
    };

    // Initializes the sprites color palette
    void init();
}
