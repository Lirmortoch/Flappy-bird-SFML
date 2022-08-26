#pragma once

#include "Game.h"

class Ground
{
private:
    GameDataRef data;

    sf::Sprite s_ground;
    sf::Sprite s_ground2;
public:
    Ground(GameDataRef _data);

    void generateGround(float x);

    void drawGround();

    const sf::Sprite& getFirstSprite();

    const sf::Sprite& getSecondSprite();
};
