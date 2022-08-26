#include "Hud.h"

Hud::Hud(GameDataRef _data) : data(_data)
{
	sf::Font* text = getGlobalFontHolder().getResource("numbers");
	
	scoreText.setFont(*text);

	scoreText.setString("0");

	scoreText.setCharacterSize(64);

	scoreText.setFillColor(sf::Color::White);

	scoreText.setOrigin(sf::Vector2f(scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().height / 2));

	scoreText.setPosition(sf::Vector2f(windowWidth / 2, windowHeight / 7));
}

void Hud::move()
{
	scoreText.move(moveSpeed, 0.0f);
}

void Hud::updateScore(int score)
{
	scoreText.setString(std::to_string(score));
}

void Hud::draw()
{
	data->window.draw(scoreText);
}