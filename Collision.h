#pragma once

#include "Game.h"

class Collision
{
public:
    Collision();

    bool checkSpriteCollision(sf::Sprite sprite1, float scale1, sf::Sprite sprite2, float scale2);
};