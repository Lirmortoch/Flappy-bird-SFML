#include "Game.h"
#include "MainMenuState.h"

int Game::loadResource()
{
    auto& tH = getGlobalTextureHolder();
    auto& fH = getGlobalFontHolder();
    
    return
        tH.loadFromFile("Resource\\res\\birdAnimation.png", "bird") &&
        tH.loadFromFile("Resource\\res\\background.png", "back") &&
        tH.loadFromFile("Resource\\res\\ground.png", "ground") &&
        tH.loadFromFile("Resource\\res\\UpperPipe.png", "top_pipe") &&
        tH.loadFromFile("Resource\\res\\DownPipe.png", "down_pipe") &&
        tH.loadFromFile("Resource\\res\\StartButton.png", "start_button") &&
        tH.loadFromFile("Resource\\res\\FB_Image.png", "title") &&
        tH.loadFromFile("Resource\\res\\GameOver.png", "gameOver") &&
        tH.loadFromFile("Resource\\res\\board.png", "board") &&
        tH.loadFromFile("Resource\\res\\goldMedal.png", "gold_medal") &&
        tH.loadFromFile("Resource\\res\\silverMedal.png", "silver_medal") &&
        tH.loadFromFile("Resource\\res\\bronzeMedal.png", "bronze_medal") &&
        tH.loadFromFile("Resource\\res\\ironMedal.png", "iron_medal") &&
        fH.loadFromFile("Resource\\FlappyFont.ttf", "numbers");
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

    if (!this->loadResource())
    {
        std::cout << "Fattal error";
    }
    
    this->run();
}
