#pragma once

#include "Game.h"

#include <random>

class Pipes
{
private:
    GameDataRef data;

    sf::Sprite s_top1;
    sf::Sprite s_down1;

    sf::Sprite s_top2;
    sf::Sprite s_down2;

    int sizeGroundTexture;
    float originPipe = 0.0f;
    float randY = 0.0f;
public:
    Pipes(GameDataRef _data);

    void randomizeY();

    void generatePipes(float x, bool state);

    void drawPipes();

    bool score(float x);

    const sf::Sprite& getFirstTopSprite();

    const sf::Sprite& getFirstDownSprite();

    const sf::Sprite& getSecondTopSprite();

    const sf::Sprite& getSecondDownSprite();
};
