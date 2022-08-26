#pragma once

#include "Game.h"
#include "InputManager.h"

class MainMenuState : public State
{
private:
    GameDataRef data;

    InputManager *iM1;

    sf::Sprite s_title;
    sf::Sprite s_background;
    sf::Sprite s_button;
public:
    MainMenuState(GameDataRef _data);

    void init();
    void handleInput();

    void update(float dt);
    void draw(float dt);
};