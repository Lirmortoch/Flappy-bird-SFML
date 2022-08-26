#include "Bird.h"

Bird::Bird(GameDataRef _data) : data(_data)
{
    sf::Texture* bird = getGlobalTextureHolder().getResource("bird");

    s_bird.setTexture(*bird);

    s_bird.setTextureRect(sf::IntRect(0, 0, 34, 24));

    s_bird.setPosition((float)windowWidth / 2.0f, (float)windowHeight / 2.0f);

    s_bird.setOrigin(s_bird.getGlobalBounds().width / 2, s_bird.getGlobalBounds().height / 2);

    currentBirdState = bStateStill;
}

void Bird::updateBird(float dt)
{
    if (bStateStill == currentBirdState)
    {
        s_bird.move(moveSpeed, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        s_bird.move(moveSpeed, -flyingSpeed * dt);

        rotation -= rotationSpeed * dt;

        if (rotation < -25.0f)
        {
            rotation = -25.0f;
        }

        s_bird.setRotation(rotation);
    }
    else
    {
        s_bird.move(moveSpeed, g * dt);

        rotation += rotationSpeed * dt;

        if (rotation > 90.0f)
        {
            rotation = 90.0f;
        }

        s_bird.setRotation(rotation);
    }
}

void Bird::tap()
{
    currentBirdState = bStateFlying;
}

void Bird::animationBird() //Animation bird
{
    currentFrame += 0.005f * time;

    if (currentFrame > 3)
    {
        currentFrame -= 3;
    }

    if (bStateStill == currentBirdState)
    {
        s_bird.setTextureRect(sf::IntRect(56 * (int)currentFrame, 0, 34, 24));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        s_bird.setTextureRect(sf::IntRect(56 * (int)currentFrame, 0, 34, 24));
    }
    else
    {
        s_bird.setTextureRect(sf::IntRect(56, 0, 34, 24));
    }

    time = clock.getElapsedTime().asMicroseconds();
    clock.restart();
    time /= 800.0f;
}

float Bird::getX()
{
    return s_bird.getPosition().x;
}

float Bird::getY()
{
    return s_bird.getPosition().y;
}

void Bird::drawBird()
{
    data->window.draw(s_bird);
}

const sf::Sprite& Bird::getSprite()
{
    return s_bird;
}