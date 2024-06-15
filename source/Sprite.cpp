#include "Sprite.hpp"

int Sprite::globalID{ 0 };

Sprite::Sprite() :
    ID{ Sprite::globalID++ }
{}