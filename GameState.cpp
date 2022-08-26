#include "GameState.h"
#include "GameOverState.h"

GameState::GameState(GameDataRef _data) : data(_data)
{

}

void GameState::init()
{
    bird1 = new Bird(data);
    back1 = new Background(data);
    ground1 = new Ground(data);
    pipes1 = new Pipes(data);
    camera1 = new Camera(data);
    hud1 = new Hud(data);
    currentGameState = eReady;
    cState = false;
    score = 0;
    hud1->updateScore(score);

    die.loadFromFile("Resource\\sounds\\sfx_die.ogg");
    hit.loadFromFile("Resource\\sounds\\sfx_hit.ogg");
    point.loadFromFile("Resource\\sounds\\sfx_point.ogg");
    wing.loadFromFile("Resource\\sounds\\sfx_wing.ogg");

    so_die.setBuffer(die);
    so_hit.setBuffer(hit);
    so_point.setBuffer(point);
    so_wing.setBuffer(wing);
}

void GameState::handleInput()
{
    sf::Event event;

    while (this->data->window.pollEvent(event))
    {
        if (sf::Event::Closed == event.type)
        {
            this->data->window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (eGameOver != currentGameState)
            {
                currentGameState = ePlaying;
                bird1->tap();
                so_wing.play();
            }
        }
    }
}

void GameState::update(float dt)
{
    if (eGameOver != currentGameState)
    {
        bird1->animationBird();

        back1->generateBack(bird1->getX());
        ground1->generateGround(bird1->getX());
        hud1->move();

        camera1->getBirdCoordinateForCamera(bird1->getX(), bird1->getY());
        camera1->setCamera();
    }

    bird1->updateBird(dt);

    if (ePlaying == currentGameState)
    {
        pipes1->generatePipes(bird1->getX(), cState);
        cState = true;

        if (collis1.checkSpriteCollision(bird1->getSprite(), 0.7f, ground1->getFirstSprite(), 1.0f)
            || collis1.checkSpriteCollision(bird1->getSprite(), 0.7f, ground1->getSecondSprite(), 1.0f))
        {
            currentGameState = eGameOver;
            so_hit.play();
        }

        if (collis1.checkSpriteCollision(bird1->getSprite(), 0.625f, pipes1->getFirstTopSprite(), 1.0f)
            || collis1.checkSpriteCollision(bird1->getSprite(), 0.625f, pipes1->getFirstDownSprite(), 1.0f))
        {
            currentGameState = eGameOver;
            so_hit.play();
        }

        if (collis1.checkSpriteCollision(bird1->getSprite(), 0.625f, pipes1->getSecondTopSprite(), 1.0f)
            || collis1.checkSpriteCollision(bird1->getSprite(), 0.625f, pipes1->getSecondDownSprite(), 1.0f))
        {
            currentGameState = eGameOver;
            so_hit.play();
        }

        if (pipes1->score(bird1->getX()))
        {
            score++;
            hud1->updateScore(score);
            so_point.play();
        }
    }
   
    if (eGameOver == currentGameState)
    {
        this->data->machine.addState(StateRef(new GameOverState(data, score)), true);
    }
}

void GameState::draw(float dt)
{
    this->data->window.clear();

    back1->drawBack();

    if (ePlaying == currentGameState)
    {
        pipes1->drawPipes();
    }

    ground1->drawGround();
    bird1->drawBird();
    hud1->draw();

    this->data->window.display();
}