#include "MainMenuState.h"
#include "GameState.h"

MainMenuState::MainMenuState(GameDataRef _data) : data(_data)
{
    
}

void MainMenuState::init()
{
    iM1 = new InputManager(data);

    sf::Texture* back = getGlobalTextureHolder().getResource("back");
  
    sf::Vector2u TextureSize = back->getSize();

    float ScaleX = (float)windowWidth / (float)TextureSize.x;
    float ScaleY = (float)windowHeight / (float)TextureSize.y;

    s_background.setTexture(*back);
    s_background.setScale(ScaleX, ScaleY);

    sf::Texture* title = getGlobalTextureHolder().getResource("title");

    s_title.setTexture(*title);

    sf::Texture* button = getGlobalTextureHolder().getResource("start_button");

    s_button.setTexture(*button);

    s_title.setPosition(windowWidth / 3.15, windowHeight / 6);
    s_button.setPosition(windowWidth / 2.46, windowHeight / 2);
}

void MainMenuState::handleInput()
{
    sf::Event event;

    while (this->data->window.pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            this->data->window.close();
        }

        if (iM1->IsSpriteClicked(event, this->s_button, sf::Mouse::Left))
        {
            this->data->machine.addState(StateRef(new GameState(data)), true);
        }
    }
}

void MainMenuState::update(float dt)
{

}

void MainMenuState::draw(float dt)
{
    this->data->window.clear();

    this->data->window.draw(this->s_background);
    this->data->window.draw(this->s_title);
    this->data->window.draw(this->s_button);

    this->data->window.display();
}