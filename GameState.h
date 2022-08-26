#pragma once

#include <SFML/Audio.hpp>

#include "State.h"
#include "Game.h"

#include "Background.h"
#include "Bird.h"
#include "Ground.h"
#include "Pipes.h"
#include "Camera.h"
#include "Collision.h"
#include "Hud.h"

class GameState : public State
{
private:
    GameDataRef data;

    Bird* bird1;
    Background* back1;
    Ground* ground1;
    Pipes* pipes1;
    Collision collis1;
    Camera* camera1;
    Hud *hud1;

    sf::Clock clock;

    sf::Sound so_die;
    sf::Sound so_hit;
    sf::Sound so_point;
    sf::Sound so_wing;

    sf::SoundBuffer die;
    sf::SoundBuffer hit;
    sf::SoundBuffer point;
    sf::SoundBuffer wing;

    enum GameStates
    {
        eReady,
        ePlaying,
        eGameOver
    };

    int currentGameState;
    int score;
    bool cState;
public:
    GameState(GameDataRef _data);

    void init();

    void handleInput();

    void update(float dt);

    void draw(float dt);
};