#include "Pipes.h"

Pipes::Pipes(GameDataRef _data) : data(_data)
{
    sf::Texture* top_pipe = getGlobalTextureHolder().getResource("top_pipe");

    sf::Texture* down_pipe = getGlobalTextureHolder().getResource("down_pipe");

    s_top1.setTexture(*top_pipe);
    s_top2.setTexture(*top_pipe);

    originPipe = top_pipe->getSize().x / 2.0f;

    s_down1.setTexture(*down_pipe);
    s_down2.setTexture(*down_pipe);

    sf::Texture* gTexture = getGlobalTextureHolder().getResource("ground");

    sizeGroundTexture = gTexture->getSize().y;
}

void Pipes::randomizeY()
{
    std::mt19937 gen;
    gen.seed(time(0));

    std::uniform_int_distribution<> uid(1, sizeGroundTexture);

    randY = uid(gen);
}

void Pipes::generatePipes(float x, bool state)
{
    if (state == false)
    {
        randomizeY();

        s_top1.setPosition(DISTANCE + x, 0.0f - distanceBetweemPipes - randY);
        s_down1.setPosition(DISTANCE + x,
            (float)windowHeight - s_top1.getGlobalBounds().height + distanceBetweemPipes - randY);

        randomizeY();

        s_top2.setPosition(s_top1.getPosition().x + DISTANCE, 0.0f - distanceBetweemPipes - randY);        
        s_down2.setPosition(s_down1.getPosition().x + DISTANCE,
            (float)windowHeight - s_top2.getGlobalBounds().height + distanceBetweemPipes - randY);
    }
    else if (x == s_top2.getPosition().x + 24)
    {
        randomizeY();

        s_top1.setPosition(s_top2.getPosition().x + DISTANCE, 0.0f - distanceBetweemPipes - randY);     
        s_down1.setPosition(s_down2.getPosition().x + DISTANCE,
            (float)windowHeight - s_top1.getGlobalBounds().height + distanceBetweemPipes - randY);
    }
    else if (x == s_top1.getPosition().x + 24)
    {
        randomizeY();

        s_top2.setPosition(s_top1.getPosition().x + DISTANCE, 0.0f - distanceBetweemPipes - randY);
        s_down2.setPosition(s_down1.getPosition().x + DISTANCE,
            (float)windowHeight - s_top2.getGlobalBounds().height + distanceBetweemPipes - randY);
    }
}

bool Pipes::score(float x)
{
    if (x == s_top1.getPosition().x + 24)
    {
        return true;
    }
    else if (x == s_top2.getPosition().x + 24)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Pipes::drawPipes()
{
    data->window.draw(s_top1);
    data->window.draw(s_down1);

    data->window.draw(s_top2);
    data->window.draw(s_down2);
}

const sf::Sprite& Pipes::getFirstTopSprite()
{
    return s_top1;
}

const sf::Sprite& Pipes::getFirstDownSprite()
{
    return s_down1;
}

const sf::Sprite& Pipes::getSecondTopSprite()
{
    return s_top2;
}

const sf::Sprite& Pipes::getSecondDownSprite()
{
    return s_down2;
}