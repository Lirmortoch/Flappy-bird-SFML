#include "Camera.h"

Camera::Camera(GameDataRef _data) : data(_data)
{
    view.setCenter(sf::Vector2f(windowWidth, windowHeight));
    view.setSize(sf::Vector2f(windowWidth, windowHeight));
}

void Camera::getBirdCoordinateForCamera(float x, float y)
{
    float tempX = x;
    float tempY = y;

    if (y < 320 || y > 320)
    {
        tempY = 320;
    }

    view.setCenter(tempX, tempY);
}

void Camera::setCamera()
{
    data->window.setView(view);
}