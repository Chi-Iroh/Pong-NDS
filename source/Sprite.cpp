#include "Sprite.hpp"
#include <nds.h>

int Sprite::globalID{ 0 };

Sprite::Sprite() :
    ID{ Sprite::globalID++ }
{
    sassert(Sprite::globalID <= SPRITE_COUNT, "%d sprites are allowed !", SPRITE_COUNT);
}