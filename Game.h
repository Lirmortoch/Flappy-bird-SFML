#pragma once

#include <SFML/Graphics.hpp>

#include "Definitions.h"
#include "ResourceHolder.h"
#include "StateMachine.h"

struct GameData
{
    sf::RenderWindow window;
    StateMachine machine;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game
{
private:
    const float dt = 1.0f / 60.0f;

    sf::Clock clock;

    GameDataRef data = std::make_shared<GameData>();

    int loadResource();

    void run();
public:
    Game();
};