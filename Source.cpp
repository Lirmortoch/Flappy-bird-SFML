/*#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <ctime>
#include <map>
#include <string>
#include <stack>
#include <memory>

#define windowWidth 460
#define windowHeight 640
#define gameTitle "Flappy Bird"

#define g 180.0f //Gravity
#define flyingSpeed 180.0f //Vertical speed
#define rotationSpeed 200.0f //Rotation speed
#define moveSpeed 2.0f //Gorizontal speed

#define distanceBetweemPipes 45.0f
#define distance 256

#define bStateStill 1
#define bStateFlying 2

template<typename ResourceType, typename KeyType>
class ResourceHolder
{
    using MapType = std::map<KeyType, ResourceType*>;
public:
    template<typename ... Args>
    ResourceType* loadFromFile(const std::string& fileName, const KeyType& key, Args &&... args)
    {
        ResourceType*& resourceRef = mResources[key];
        if (resourceRef != nullptr) {
            return resourceRef;
        }
        resourceRef = new ResourceType();
        if (!resourceRef->loadFromFile(fileName, std::forward<Args>(args)...)) {
            delete resourceRef;
            resourceRef = nullptr;
        }
        return resourceRef;
    }
    ResourceType* getResource(const KeyType& key)
    {
        typename MapType::iterator resourcePairIt = mResources.find(key);
        if (resourcePairIt == mResources.end()) {
            return nullptr;
        }
        return resourcePairIt->second;
    }

    ~ResourceHolder()
    {
        for (auto& e : mResources) {
            delete e.second;
        }
    }

    ResourceHolder() = default;
    ResourceHolder(const ResourceHolder&) = delete;
    ResourceHolder(ResourceHolder&&) = delete;
    ResourceHolder& operator=(const ResourceHolder&) = delete;
    ResourceHolder& operator=(ResourceHolder&&) = delete;
private:
    MapType mResources;
};

template<typename ResourceType, typename KeyType>
ResourceHolder<ResourceType, KeyType>& getGlobalResourceHolder()
{
    static ResourceHolder<ResourceType, KeyType> holder;
    return holder;
}

auto getGlobalTextureHolder = getGlobalResourceHolder<sf::Texture, std::string>;

class State
{
public:
    virtual void init() = 0;

    virtual void handleInput() = 0;
    virtual void update(float dt) = 0;
    virtual void draw(float dt) = 0;

    virtual void pause() {}
    virtual void resume() {}
};

typedef std::unique_ptr <State> StateRef;

class StateMachine
{
private:
    std::stack <StateRef> states;
    StateRef newState;

    bool isRemoving;
    bool isAdding;
    bool isReplacing;
public:
    StateMachine() {}

    void addState(StateRef _newState, bool _isReplacing = true)
    {
        this->isAdding = true;
        this->isReplacing = _isReplacing;

        this->newState = std::move(_newState);
    }

    void removeState()
    {
        this->isRemoving = true;
    }

    void processStateChanges()
    {
        if (this->isRemoving && !this->states.empty())
        {
            this->states.pop();

            if (!this->states.empty())
            {
                this->states.top()->resume();
            }

            this->isRemoving = false;
        }

        if (this->isAdding)
        {
            if (!this->states.empty())
            {
                if (this->isReplacing)
                {
                    this->states.pop();
                }
                else
                {
                    this->states.top()->pause();
                }
            }

            this->states.push(std::move(this->newState));
            this->states.top()->init();
            this->isAdding = false;
        }
    }

    StateRef& getActiveState()
    {
        return this->states.top();
    }

    ~StateMachine() {}
};

struct GameData
{
    sf::RenderWindow window;
    StateMachine machine;

    bool IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window)
    {
        if (sf::Mouse::isButtonPressed(button))
        {
            sf::IntRect playButtonRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

            if (playButtonRect.contains(sf::Mouse::getPosition(window)))
            {
                return true;
            }
        }

        return false;
    }

    int loadResource()
    {
        sf::Texture* bird = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\birdAnimation.png", "bird");
        if (bird == nullptr)
        {
            return -1;
        }

        sf::Texture* back = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\background.png", "back");
        if (back == nullptr)
        {
            return -1;

        }

        sf::Texture* ground = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\ground.png", "ground");
        if (ground == nullptr)
        {
            return -1;
        }

        sf::Texture* top_pipe = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\UpperPipe.png", "top_pipe");
        if (top_pipe == nullptr)
        {
            return -1;
        }

        sf::Texture* down_pipe = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\DownPipe.png", "down_pipe");
        if (down_pipe == nullptr)
        {
            return -1;
        }

        sf::Texture* startButton = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\StartButton.png", "start_button");
        if (startButton == nullptr)
        {
            return -1;
        }

        sf::Texture* title = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\FB_Image.png", "title");
        if (title == nullptr)
        {
            return -1;
        }
    }
};

typedef std::shared_ptr<GameData> GameDataRef;

class Bird
{
private:
    GameDataRef data;

    sf::Sprite s_bird;
    sf::Clock clock;

    int currentBirdState;

    float currentFrame = 0.0f; //Current frame
    float rotation = 0.0f; //Variable for rotate
    float time = 0.0f; // Time
public:
    Bird(GameDataRef _data) : data(_data)
    {
        sf::Texture* bird = getGlobalTextureHolder().getResource("bird");
        if (bird == nullptr)
        {
            bird = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\birdAnimation.png", "bird");
        }

        s_bird.setTexture(*bird);

        s_bird.setTextureRect(sf::IntRect(0, 0, 34, 24));

        s_bird.setPosition((float)windowWidth / 2.0f, (float)windowHeight / 2.0f);

        s_bird.setOrigin(s_bird.getGlobalBounds().width / 2, s_bird.getGlobalBounds().height / 2);

        currentBirdState = bStateStill;
    }

    void updateBird(float dt)
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

    void tap()
    {
        currentBirdState = bStateFlying;
    }

    void animationBird() //Animation bird
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

    float getX()
    {
        return s_bird.getPosition().x;
    }

    float getY()
    {
        return s_bird.getPosition().y;
    }

    void drawBird()
    {
        data->window.draw(s_bird);
    }

    const sf::Sprite& getSprite()
    {
        return s_bird;
    }
};

class Background
{
private:
    GameDataRef data;

    sf::Sprite s_back1;
    sf::Sprite s_back2;
public:
    Background(GameDataRef _data) : data(_data)
    {
        sf::Texture* back = getGlobalTextureHolder().getResource("back");
        if (back == nullptr)
        {
            back = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\background.png", "back");
        }

        sf::Vector2u TextureSize = back->getSize();

        float ScaleX = (float)windowWidth / (float)TextureSize.x;
        float ScaleY = (float)windowHeight / (float)TextureSize.y;

        s_back1.setTexture(*back);
        s_back2.setTexture(*back);
        s_back1.setScale(ScaleX, ScaleY);
        s_back2.setScale(ScaleX, ScaleY);
    }

    void generateBack(float x)
    {
        if (x >= s_back1.getPosition().x + 230)
        {
            s_back2.setPosition(s_back1.getPosition().x + (float)windowWidth, 0.0f);
        }
        if (x >= s_back2.getPosition().x + 230)
        {
            s_back1.setPosition(s_back2.getPosition().x + (float)windowWidth, 0.0f);
        }
    }

    void drawBack()
    {
        data->window.draw(s_back1);
        data->window.draw(s_back2);
    }
};

class Ground
{
private:
    GameDataRef data;

    sf::Sprite s_ground;
    sf::Sprite s_ground2;
public:
    Ground(GameDataRef _data) : data(_data)
    {
        sf::Texture* ground = getGlobalTextureHolder().getResource("ground");
        if (ground == nullptr)
        {
            ground = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\ground.png", "ground");
        }

        s_ground.setTexture(*ground);
        s_ground2.setTexture(*ground);

        s_ground.setScale(1.003f, 1.0f);
        s_ground2.setScale(1.003f, 1.0f);

        s_ground.setPosition(0.0f, windowHeight / 1.3f);
    }

    void generateGround(float x)
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

    void drawGround()
    {
        data->window.draw(s_ground);
        data->window.draw(s_ground2);
    }

    const sf::Sprite& getFirstSprite()
    {
        return s_ground;
    }

    const sf::Sprite& getSecondSprite()
    {
        return s_ground2;
    }
};

class Pipes
{
private:
    GameDataRef data;

    sf::Sprite s_top1;
    sf::Sprite s_down1;

    sf::Sprite s_top2;
    sf::Sprite s_down2;

    int sizeGroundTexture;
    float originPipe = 0.0f;
    float randY = 0.0f;
public:
    Pipes(GameDataRef _data) : data(_data)
    {
        sf::Texture* top_pipe = getGlobalTextureHolder().getResource("top_pipe");
        if (top_pipe == nullptr)
        {
            top_pipe = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\UpperPipe.png", "top_pipe");
        }

        sf::Texture* down_pipe = getGlobalTextureHolder().getResource("down_pipe");
        if (down_pipe == nullptr)
        {
            down_pipe = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\DownPipe.png", "down_pipe");
        }

        s_top1.setTexture(*top_pipe);
        s_top2.setTexture(*top_pipe);

        originPipe = top_pipe->getSize().x / 2.0f;

        s_down1.setTexture(*down_pipe);
        s_down2.setTexture(*down_pipe);

        sf::Texture* gTexture = getGlobalTextureHolder().getResource("ground");
        if (gTexture == nullptr)
        {
            gTexture = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\ground.png", "ground");
        }

        sizeGroundTexture = gTexture->getSize().y;
    }

    void randomizeY()
    {
        std::mt19937 gen;
        gen.seed(time(0));

        std::uniform_int_distribution<> uid(1, sizeGroundTexture);

        randY = uid(gen);
    }

    void generatePipes(float x, bool state)
    {
        if (state == false)
        {
            randomizeY();

            s_top1.setPosition(distance + x, 0.0f - distanceBetweemPipes - randY);
            s_down1.setPosition(distance + x,
                (float)windowHeight - s_top1.getGlobalBounds().height + distanceBetweemPipes - randY);

            randomizeY();

            s_top2.setPosition(s_top1.getPosition().x + distance, 0.0f - distanceBetweemPipes - randY);
            s_down2.setPosition(s_down1.getPosition().x + distance,
                (float)windowHeight - s_top2.getGlobalBounds().height + distanceBetweemPipes - randY);
        }
        else if (x == s_top2.getPosition().x + 24)
        {
            randomizeY();

            s_top1.setPosition(s_top2.getPosition().x + distance, 0.0f - distanceBetweemPipes - randY);
            s_down1.setPosition(s_down2.getPosition().x + distance,
                (float)windowHeight - s_top1.getGlobalBounds().height + distanceBetweemPipes - randY);

        }
        else if (x == s_top1.getPosition().x + 24)
        {
            randomizeY();

            s_top2.setPosition(s_top1.getPosition().x + distance, 0.0f - distanceBetweemPipes - randY);
            s_down2.setPosition(s_down1.getPosition().x + distance,
                (float)windowHeight - s_top2.getGlobalBounds().height + distanceBetweemPipes - randY);
        }
    }

    void drawPipes()
    {
        data->window.draw(s_top1);
        data->window.draw(s_down1);

        data->window.draw(s_top2);
        data->window.draw(s_down2);
    }

    const sf::Sprite& getFirstTopSprite()
    {
        return s_top1;
    }

    const sf::Sprite& getFirstDownSprite()
    {
        return s_down1;
    }

    const sf::Sprite& getSecondTopSprite()
    {
        return s_top2;
    }

    const sf::Sprite& getSecondDownSprite()
    {
        return s_down2;
    }
};

class Camera
{
private:
    GameDataRef data;

    sf::View view;
public:
    Camera(GameDataRef _data) : data(_data)
    {
        view.setCenter(sf::Vector2f(windowWidth, windowHeight));
        view.setSize(sf::Vector2f(windowWidth, windowHeight));
    }

    void getBirdCoordinateForCamera(float x, float y)
    {
        float tempX = x;
        float tempY = y;

        if (y < 320 || y > 320)
        {
            tempY = 320;
        }

        view.setCenter(tempX, tempY);
    }

    void setCamera()
    {
        data->window.setView(view);
    }
};

class Collision
{
public:
    Collision()
    {

    }

    bool checkSpriteCollision(sf::Sprite sprite1, sf::Sprite sprite2)
    {
        sf::Rect<float> rect1 = sprite1.getGlobalBounds();
        sf::Rect<float> rect2 = sprite2.getGlobalBounds();

        if (rect1.intersects(rect2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class GameState : public State
{
private:
    GameDataRef data;

    Bird* bird1;
    Background* back1;
    Ground* ground1;
    Pipes* pipes1;
    Camera* camera1;
    Collision collis1;

    sf::Clock clock;

    enum GameStates
    {
        eReady,
        ePlaying,
        eGameOver
    };

    int currentGameState;
    int score;
    bool cState;
public:
    GameState(GameDataRef _data) : data(_data)
    {

    }

    void init()
    {
        bird1 = new Bird(data);
        back1 = new Background(data);
        ground1 = new Ground(data);
        pipes1 = new Pipes(data);
        camera1 = new Camera(data);
        currentGameState = eReady;
        cState = false;
    }

    void handleInput()
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
                }
            }
        }
    }

    void update(float dt)
    {
        if (eGameOver != currentGameState)
        {
            bird1->animationBird();

            back1->generateBack(bird1->getX());
            ground1->generateGround(bird1->getX());
            camera1->getBirdCoordinateForCamera(bird1->getX(), bird1->getY());
            camera1->setCamera();
        }

        if (ePlaying == currentGameState)
        {
            pipes1->generatePipes(bird1->getX(), cState);
            cState = true;
        }

        bird1->updateBird(dt);

        if (collis1.checkSpriteCollision(bird1->getSprite(), ground1->getFirstSprite())
            || collis1.checkSpriteCollision(bird1->getSprite(), ground1->getSecondSprite()))
        {
            currentGameState = eGameOver;
        }

        if (collis1.checkSpriteCollision(bird1->getSprite(), pipes1->getFirstTopSprite())
            || collis1.checkSpriteCollision(bird1->getSprite(), pipes1->getFirstDownSprite()))
        {
            currentGameState = eGameOver;
        }

        if (collis1.checkSpriteCollision(bird1->getSprite(), pipes1->getSecondTopSprite())
            || collis1.checkSpriteCollision(bird1->getSprite(), pipes1->getSecondDownSprite()))
        {
            currentGameState = eGameOver;
        }

        if (eGameOver == currentGameState)
        {
            //this->data->machine.addState(StateRef(new GameOverState(data, score)), true);
        }
    }

    void draw(float dt)
    {
        this->data->window.clear();

        back1->drawBack();

        if (ePlaying == currentGameState)
        {
            pipes1->drawPipes();
        }

        ground1->drawGround();
        bird1->drawBird();

        this->data->window.display();
    }
};

//////////
class GameOverState : public State
{
private:
    GameDataRef data;

    sf::Sprite s_back;
    sf::Sprite s_gameOverTitle;
    sf::Sprite s_retryButton;

    int score;
public:
    GameOverState(GameDataRef _data, int _score) : data(_data), score(_score)
    {

    }

    void init()
    {
        sf::Texture* back = getGlobalTextureHolder().getResource("back");
        if (back == nullptr)
        {
            back = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\background.png", "back");
        }

        sf::Vector2u TextureSize = back->getSize();

        float ScaleX = (float)windowWidth / (float)TextureSize.x;
        float ScaleY = (float)windowHeight / (float)TextureSize.y;

        s_back.setTexture(*back);
        s_back.setScale(ScaleX, ScaleY);

        sf::Texture* title = getGlobalTextureHolder().getResource("title");
        if (title == nullptr)
        {
            title = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\FB_Image.png", "title");
        }

        s_gameOverTitle.setTexture(*title);

        sf::Texture* button = getGlobalTextureHolder().getResource("start_button");
        if (button == nullptr)
        {
            button = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\StartButton.png", "start_button");
        }

        s_retryButton.setTexture(*button);

        s_gameOverTitle.setPosition(windowWidth / 3.15, windowHeight / 6);
        s_retryButton.setPosition(windowWidth / 2.46, windowHeight / 2);
    }

    void handleInput()
    {
        sf::Event event;

        while (this->data->window.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                this->data->window.close();
            }

            if (this->data->IsSpriteClicked(this->s_retryButton, sf::Mouse::Left, this->data->window))
            {
                // Switch to main menu
                this->data->machine.addState(StateRef(new GameState(data)), true);
            }
        }
    }

    void update(float dt)
    {

    }

    void draw(float dt)
    {
        this->data->window.clear();

        this->data->window.draw(this->s_back);
        this->data->window.draw(this->s_gameOverTitle);
        this->data->window.draw(this->s_retryButton);

        this->data->window.display();
    }
};
////////////

class MainMenuState : public State
{
private:
    GameDataRef data;

    sf::Sprite s_title;
    sf::Sprite s_background;
    sf::Sprite s_button;
public:
    MainMenuState(GameDataRef _data) : data(_data)
    {
        data->loadResource();
    }

    void init()
    {
        sf::Texture* back = getGlobalTextureHolder().getResource("back");
        if (back == nullptr)
        {
            back = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\background.png", "back");
        }

        sf::Vector2u TextureSize = back->getSize();

        float ScaleX = (float)windowWidth / (float)TextureSize.x;
        float ScaleY = (float)windowHeight / (float)TextureSize.y;

        s_background.setTexture(*back);
        s_background.setScale(ScaleX, ScaleY);

        sf::Texture* title = getGlobalTextureHolder().getResource("title");
        if (title == nullptr)
        {
            title = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\FB_Image.png", "title");
        }

        s_title.setTexture(*title);

        sf::Texture* button = getGlobalTextureHolder().getResource("start_button");
        if (button == nullptr)
        {
            button = getGlobalTextureHolder().loadFromFile("D:\\FB\\gfx\\StartButton.png", "start_button");
        }

        s_button.setTexture(*button);

        s_title.setPosition(windowWidth / 3.15, windowHeight / 6);
        s_button.setPosition(windowWidth / 2.46, windowHeight / 2);
    }

    void handleInput()
    {
        sf::Event event;

        while (this->data->window.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                this->data->window.close();
            }

            if (this->data->IsSpriteClicked(this->s_button, sf::Mouse::Left, this->data->window))
            {
                // Switch to main menu
                this->data->machine.addState(StateRef(new GameState(data)), true);
            }
        }
    }

    void update(float dt)
    {

    }

    void draw(float dt)
    {
        this->data->window.clear();

        this->data->window.draw(this->s_background);
        this->data->window.draw(this->s_title);
        this->data->window.draw(this->s_button);

        this->data->window.display();
    }
};

class Game
{
private:
    const float dt = 1.0f / 60.0f;

    sf::Clock clock;

    GameDataRef data = std::make_shared<GameData>();

    void run()
    {
        float newTime;
        float frameTime;
        float interpolation;

        float currentTime = this->clock.getElapsedTime().asSeconds();

        float accumulator = 0.0f;

        while (this->data->window.isOpen())
        {
            this->data->machine.processStateChanges();

            newTime = this->clock.getElapsedTime().asSeconds();
            frameTime = newTime - currentTime;

            if (frameTime > 0.25f)
            {
                frameTime = 0.25f;
            }

            currentTime = newTime;
            accumulator += frameTime;

            while (accumulator >= dt)
            {
                this->data->machine.getActiveState()->handleInput();
                this->data->machine.getActiveState()->update(dt);

                accumulator -= dt;
            }

            interpolation = accumulator / dt;
            this->data->machine.getActiveState()->draw(interpolation);
        }
    }
public:
    Game()
    {
        srand(time(NULL));

        data->window.create(sf::VideoMode(windowWidth, windowHeight), gameTitle);
        data->machine.addState(StateRef(new MainMenuState(this->data)));

        this->run();
    }
};

int main()
{
    Game game1;

    return 0;
}*/

#include "Game.h"

int main()
{
    Game game1;

    return 0;
}