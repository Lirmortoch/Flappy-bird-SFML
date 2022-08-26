#include "InputManager.h"

InputManager::InputManager(GameDataRef _data) : data(_data)
{

}

/*bool InputManager::IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button)
{
    if (sf::Mouse::isButtonPressed(button))
    {
        sf::IntRect playButtonRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

        if (playButtonRect.contains(sf::Mouse::getPosition(data->window)))
        {
            return true;
        }
    }

    return false;
}*/

bool InputManager::IsSpriteClicked(sf::Event event, sf::Sprite object, sf::Mouse::Button button)
{
    if (event.type == sf::Event::MouseButtonReleased)
    { // on click
        auto pos = data->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (event.mouseButton.button == sf::Mouse::Left)
        { // on left click
            if (object.getGlobalBounds().contains(pos))
            {
                return true;
            }
        }
    }

    return false;
}

sf::Vector2i InputManager::GetMousePosition()
{
    return sf::Mouse::getPosition(data->window);
}