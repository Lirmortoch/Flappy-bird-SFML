#pragma once

#include <fstream>
#include "State.h"
#include "Game.h"
#include "Camera.h"
#include "InputManager.h"

class GameOverState : public State
{
private:
    GameDataRef data;

    InputManager* iM2;
    Camera* camera2;

    sf::Sprite s_back;
    sf::Sprite s_gameOverTitle;
    sf::Sprite s_retryButton;
    sf::Sprite s_board;

    sf::Sprite s_goldMedal;
    sf::Sprite s_silverMedal;
    sf::Sprite s_bronzeMedal;
    sf::Sprite s_ironMedal;

    sf::Text t_score;
    sf::Text t_bestScore;

    int score;
    int bestScore;
public:
    GameOverState(GameDataRef _data, int _score);

    void init();

    void handleInput();

    void update(float dt);

    void draw(float dt);
};