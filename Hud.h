#pragma once

#include "Game.h"

#include <string>

class Hud
{
private:
	GameDataRef data;

	sf::Text scoreText;
public:
	Hud(GameDataRef _data);

	void move();

	void updateScore(int score);

	void draw();
};
