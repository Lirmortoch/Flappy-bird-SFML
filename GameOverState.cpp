#include "GameOverState.h"
#include "GameState.h"

GameOverState::GameOverState(GameDataRef _data, int _score) : data(_data), score(_score)
{

}

void GameOverState::init()
{
    std::fstream read("BestScore.txt", std::ios::in);

    if (read.is_open())
    {
        while (!read.eof())
        {
            read >> bestScore;
        }
    }

    read.close();

    std::fstream write("BestScore.txt", std::ios::out);

    if (write.is_open())
    {
        if (score > bestScore)
        {
            bestScore = score;
        }

        write << bestScore;
    }

    write.close();

    camera2 = new Camera(data);
    iM2 = new InputManager(data);

    sf::Texture* back = getGlobalTextureHolder().getResource("back");

    sf::Vector2u TextureSize = back->getSize();

    float ScaleX = (float)windowWidth / (float)TextureSize.x;
    float ScaleY = (float)windowHeight / (float)TextureSize.y;

    s_back.setTexture(*back);
    s_back.setScale(ScaleX, ScaleY);

    sf::Texture* gameOverTitle = getGlobalTextureHolder().getResource("gameOver");

    s_gameOverTitle.setTexture(*gameOverTitle);

    sf::Texture* button = getGlobalTextureHolder().getResource("start_button");

    s_retryButton.setTexture(*button);

    camera2->setCamera();

    s_back.setPosition(windowWidth / 2, windowHeight / 2);
    s_gameOverTitle.setPosition(windowWidth / 1.21, windowHeight / 1.51);
    s_retryButton.setPosition(windowWidth / 1.09, windowHeight / 1.0);

    sf::Texture* board = getGlobalTextureHolder().getResource("board");

    s_board.setTexture(*board);
 
    s_board.setPosition(windowWidth / 1.26, windowHeight / 1.29);

    sf::Texture* gold_medal = getGlobalTextureHolder().getResource("gold_medal");

    s_goldMedal.setTexture(*gold_medal);

    sf::Texture* silver_medal = getGlobalTextureHolder().getResource("silver_medal");

    s_silverMedal.setTexture(*silver_medal);

    sf::Texture* bronze_medal = getGlobalTextureHolder().getResource("bronze_medal");

    s_bronzeMedal.setTexture(*bronze_medal);

    sf::Texture* iron_medal = getGlobalTextureHolder().getResource("iron_medal");

    s_ironMedal.setTexture(*iron_medal);

    sf::Font* text = getGlobalFontHolder().getResource("numbers");

    t_score.setFont(*text);
    t_bestScore.setFont(*text);

    t_score.setString(std::to_string(score));
    t_score.setCharacterSize(20);
    t_score.setFillColor(sf::Color::White);
    t_score.setOrigin(sf::Vector2f(t_score.getGlobalBounds().width / 2, t_score.getGlobalBounds().height / 2));

    t_bestScore.setString(std::to_string(bestScore));
    t_bestScore.setCharacterSize(20);
    t_bestScore.setFillColor(sf::Color::White);
    t_bestScore.setOrigin(sf::Vector2f(t_score.getGlobalBounds().width / 2, t_score.getGlobalBounds().height / 2));

    t_score.setPosition(550, 534);
    t_bestScore.setPosition(551, 576);

    if (score > 0 && score < 25)
    {
        s_ironMedal.setPosition(390, 539);
    }
    else if (score >= 25 && score < 50)
    {
        s_bronzeMedal.setPosition(390, 539);
    }
    else if (score >= 50 && score < 75)
    {
        s_silverMedal.setPosition(390, 539);
    }
    else if (score >= 75)
    {
        s_goldMedal.setPosition(390, 539);
    }
}

void GameOverState::handleInput()
{
    sf::Event event;

    while (this->data->window.pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            this->data->window.close();
        }

        if (iM2->IsSpriteClicked(event, this->s_retryButton, sf::Mouse::Left))
        {
            this->data->machine.addState(StateRef(new GameState(data)), true);
        }
    }
}

void GameOverState::update(float dt)
{

}

void GameOverState::draw(float dt)
{
    this->data->window.clear();

    this->data->window.draw(this->s_back);
    this->data->window.draw(this->s_retryButton);
    this->data->window.draw(this->s_gameOverTitle);
    this->data->window.draw(this->s_board);

    if (score > 0 && score < 25)
    {
        this->data->window.draw(this->s_ironMedal);
    }
    else if (score >= 25 && score < 50)
    {
        this->data->window.draw(this->s_bronzeMedal);
    }
    else if (score >= 50 && score < 75)
    {
        this->data->window.draw(this->s_silverMedal);
    }
    else if (score >= 75)
    {
        this->data->window.draw(this->s_goldMedal);
    }
    
    this->data->window.draw(t_score);
    this->data->window.draw(t_bestScore);

    this->data->window.display();
}