#pragma once

#include "Game.h"

class Background
{
private:
    GameDataRef data;

    sf::Sprite s_back1;
    sf::Sprite s_back2;
public:
    Background(GameDataRef _data);

    void generateBack(float x);

    void drawBack();
};
