#pragma once

#include "Game.h"

class Bird
{
private:
    GameDataRef data;

    sf::Sprite s_bird;
    sf::Clock clock;

    int currentBirdState;

    float currentFrame = 0.0f; //Current frame
    float rotation = 0.0f; //Variable for rotate
    float time = 0.0f; // Time
public:
    Bird(GameDataRef _data);

    void updateBird(float dt);

    void tap();

    void animationBird(); //Animation bird

    float getX();

    float getY();

    void drawBird();

    const sf::Sprite& getSprite();
};