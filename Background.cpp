#include "Background.h"

Background::Background(GameDataRef _data) : data(_data)
{
    sf::Texture* back = getGlobalTextureHolder().getResource("back");

    sf::Vector2u TextureSize = back->getSize();

    float ScaleX = (float)windowWidth / (float)TextureSize.x;
    float ScaleY = (float)windowHeight / (float)TextureSize.y;

    s_back1.setTexture(*back);
    s_back2.setTexture(*back);
    s_back1.setScale(ScaleX, ScaleY);
    s_back2.setScale(ScaleX, ScaleY);
}

void Background::generateBack(float x)
{
    if (x >= s_back1.getPosition().x + 230)
    {
        s_back2.setPosition(s_back1.getPosition().x + (float)windowWidth, 0.0f);
    }
    if (x >= s_back2.getPosition().x + 230)
    {
        s_back1.setPosition(s_back2.getPosition().x + (float)windowWidth, 0.0f);
    }
}

void Background::drawBack()
{
    data->window.draw(s_back1);
    data->window.draw(s_back2);
}
