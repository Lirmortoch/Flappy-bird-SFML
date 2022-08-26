#include "Game.h"
#include "MainMenuState.h"

int Game::loadResource()
{
    sf::Texture* bird = getGlobalTextureHolder().loadFromFile("Resource\\res\\birdAnimation.png", "bird");
    if (bird == nullptr)
    {
        return -1;
    }

    sf::Texture* back = getGlobalTextureHolder().loadFromFile("Resource\\res\\background.png", "back");
    if (back == nullptr)
    {
        return -1;

    }

    sf::Texture* ground = getGlobalTextureHolder().loadFromFile("Resource\\res\\ground.png", "ground");
    if (ground == nullptr)
    {
        return -1;
    }

    sf::Texture* top_pipe = getGlobalTextureHolder().loadFromFile("Resource\\res\\UpperPipe.png", "top_pipe");
    if (top_pipe == nullptr)
    {
        return -1;
    }

    sf::Texture* down_pipe = getGlobalTextureHolder().loadFromFile("Resource\\res\\DownPipe.png", "down_pipe");
    if (down_pipe == nullptr)
    {
        return -1;
    }

    sf::Texture* startButton = getGlobalTextureHolder().loadFromFile("Resource\\res\\StartButton.png", "start_button");
    if (startButton == nullptr)
    {
        return -1;
    }

    sf::Texture* title = getGlobalTextureHolder().loadFromFile("Resource\\res\\FB_Image.png", "title");
    if (title == nullptr)
    {
        return -1;
    }

    sf::Texture* gameOverTitle = getGlobalTextureHolder().loadFromFile("Resource\\res\\GameOver.png", "gameOver");
    if (gameOverTitle == nullptr)
    {
        return -1;
    }

    sf::Texture* board = getGlobalTextureHolder().loadFromFile("Resource\\res\\board.png", "board");
    if (board == nullptr)
    {
        return -1;
    }

    sf::Texture* gold_medal = getGlobalTextureHolder().loadFromFile("Resource\\res\\goldMedal.png", "gold_medal");
    if (gold_medal == nullptr)
    {
        return -1;
    }

    sf::Texture* silver_medal = getGlobalTextureHolder().loadFromFile("Resource\\res\\silverMedal.png", "silver_medal");
    if (silver_medal == nullptr)
    {
        return -1;
    }

    sf::Texture* bronze_medal = getGlobalTextureHolder().loadFromFile("Resource\\res\\bronzeMedal.png", "bronze_medal");
    if (bronze_medal == nullptr)
    {
        return -1;
    }

    sf::Texture* iron_medal = getGlobalTextureHolder().loadFromFile("Resource\\res\\ironMedal.png", "iron_medal");
    if (iron_medal == nullptr)
    {
        return -1;
    }

    sf::Font* text = getGlobalFontHolder().loadFromFile("Resource\\FlappyFont.ttf", "numbers");
    if (text == nullptr)
    {
        return -1;
    }
}

void Game::run()
{
    float newTime;
    float frameTime;
    float interpolation;

    float currentTime = this->clock.getElapsedTime().asSeconds();

    float accumulator = 0.0f;

    while (this->data->window.isOpen())
    {
        this->data->machine.processStateChanges();

        newTime = this->clock.getElapsedTime().asSeconds();
        frameTime = newTime - currentTime;

        if (frameTime > 0.25f)
        {
            frameTime = 0.25f;
        }

        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt)
        {
            this->data->machine.getActiveState()->handleInput();
            this->data->machine.getActiveState()->update(dt);

            accumulator -= dt;
        }

        interpolation = accumulator / dt;
        this->data->machine.getActiveState()->draw(interpolation);
    }
}

Game::Game()
{
    srand(time(NULL));

    data->window.create(sf::VideoMode(windowWidth, windowHeight), gameTitle);
    data->machine.addState(StateRef(new MainMenuState(this->data)));

    this->loadResource();
    this->run();
}