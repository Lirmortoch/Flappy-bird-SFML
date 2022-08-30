#pragma once

#include "Game.h"
#include <SFML/Graphics.hpp>

class InputManager
{
private:
    GameDataRef data;
public:
    InputManager(GameDataRef _data);

    bool IsSpriteClicked(sf::Event event, sf::Sprite object, sf::Mouse::Button button);

    sf::Vector2i GetMousePosition();
};
