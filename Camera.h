#pragma once

#include "Game.h"

class Camera
{
private:
    GameDataRef data;
    sf::View view;
public:
    Camera(GameDataRef _data);

    void getBirdCoordinateForCamera(float x, float y);

    void setCamera();
};