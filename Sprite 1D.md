# Note about 1D sprite mapping and how tiles must be stored

As a remainder, when we setup the OAM with `oamInit`, we must pass a sprite mapping policy (from `SpriteMapping` enum).  
There are 2 main types : `SpriteMapping_2D_XXX`, to treat a sprite as a 2D array, or `SpriteMapping_1D_XXX` to treat a sprite as a 1D array, a contiguous array of all tiles (more details below).  

If we want to define a 16x16 square sprite, we can make an array of 4 tiles :  
```c
u32 SQUARE[8 * 4] = {
    0x1111'1111,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,

    0x1111'1111,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,

    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1000'0000,
    0x1111'1111,

    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x0000'0001,
    0x1111'1111
};
```

In 1D mode, the tiles need to be stored from left to right and from top to bottom.  
For instance, a 16x16 sprite is stored like :

Sprite layout when displayed (2x2 = 4 tiles) :  
| 0 | 1 |
|---|---|
| 2 | 3 |

Sprite layout in memory (4 contiguous tiles) :  
| 0 |
|---|
| 1 |
| 2 |
| 3 |

This explains the order of tiles in the above `SQUARE` array :
- 1st tile = top-left
- 2nd tile = top-right
- 3rd tile = bottom-left
- 4th tile = bottom-right

The first tile in the `SQUARE` array describes a line on the left and another one on the top.  
The tiles numbers must be read from left to right for readability, so we write them as big endian.  
But the Nintendo DS is little endian, so these numbers must be converted to little endian before copying them to the VRAM.  

16 colors only:
However, each color takes 4 bits, so we must swap each nibble, not byte.  
Here's a function to perform the nibble swap :  
```c
/**
 * Reverses the endianness, but swaps each nibble and not each byte.
 * Used for 16 colors sprites hardcoded tiles.
*/
u32 nibbleSwap(u32 val) {
    u32 swapped = 0;
    u32 nibbleOffset = 32 - 4; // most signicant nibble (ex: 32-bit unsigned -> nibble at bit 29, 29-->32 = 4 bits)

    while (val) {
        swapped |= (val & 0xF) << nibbleOffset;
        val >>= 4;
        nibbleOffset -= 4;
    }
    return swapped;
}
```

Whenever 16 or 256 colors:
Anyway, here's how to copy the sprite into a gfx pointer:  
```c
u16* gfx = oamAllocateGfx(...);

for (unsigned i = 0; i < 8 * 4; i++) {
    ((u32*)gfx)[i] = nibbleSwap(SQUARE[i]);
}
```