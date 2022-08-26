#include "Ground.h"

Ground::Ground(GameDataRef _data) : data(_data)
{
    sf::Texture* ground = getGlobalTextureHolder().getResource("ground");

    s_ground.setTexture(*ground);
    s_ground2.setTexture(*ground);

    s_ground.setScale(1.003f, 1.0f);
    s_ground2.setScale(1.003f, 1.0f);

    s_ground.setPosition(0.0f, windowHeight / 1.3f);
}

void Ground::generateGround(float x)
{
    if (x >= s_ground.getPosition().x + 230)
    {
        s_ground2.setPosition(s_ground.getPosition().x + (float)windowWidth, windowHeight / 1.3f);
    }
    if (x >= s_ground2.getPosition().x + 230)
    {
        s_ground.setPosition(s_ground2.getPosition().x + (float)windowWidth, windowHeight / 1.3f);
    }
}

void Ground::drawGround()
{
    data->window.draw(s_ground);
    data->window.draw(s_ground2);
}

const sf::Sprite& Ground::getFirstSprite()
{
    return s_ground;
}

const sf::Sprite& Ground::getSecondSprite()
{
    return s_ground2;
}