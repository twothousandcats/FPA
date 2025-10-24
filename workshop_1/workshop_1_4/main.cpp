#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <memory>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr Vector2f WINDOW_CENTER = {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f};
constexpr float SAFE_ZONE_RADIUS = 5.f;

// point - точка относительно начала координат
bool isPointInSafeZone(
    const Vector2f &point)
{
    return point.x * point.x + point.y * point.y <= SAFE_ZONE_RADIUS * SAFE_ZONE_RADIUS;
}

bool isPointInSafeZoneByCenter(
    const Vector2f &centerSafeZone,
    const Vector2f &point)
{
    return isPointInSafeZone(point - centerSafeZone);
}

enum class Direction
{
    MovingDown = 0,
    MovingRight = 1,
    MovingUp = 2,
    MovingLeft = 3,
    Idle = 4
};

struct Target
{
    float distance = 0.f;
    Vector2f normVector = {0.f, 0.f};

    void setByVector(const Vector2f &toTarget)
    {
        distance = toTarget.length();
        normVector = toTarget.normalized();
    }

    Direction getDirection()
    {
        const float angle = normVector.angle().wrapUnsigned().asDegrees();
        if (angle < 45 || angle > 315)
            return Direction::MovingRight;
        if (angle >= 45 && angle < 135)
            return Direction::MovingDown;
        if (angle >= 135 && angle < 225)
            return Direction::MovingLeft;
        return Direction::MovingUp;    
    }

    // У цели?
    bool isInPlace()
    {
        return distance <= SAFE_ZONE_RADIUS;
    }
};

class IHasSprite
{
    std::unique_ptr<sf::Sprite> sprite = nullptr;

protected:
    IHasSprite(const sf::Texture &texture)
    {
        sprite = std::make_unique<sf::Sprite>(texture);
    }

    Vector2f getPosition()
    {
        return sprite ? sprite->getPosition() : sf::Vector2f(0.f, 0.f);
    }

    void setScale(const Vector2f &scale)
    {
        if (sprite)
            sprite->setScale(scale);
    }

    void setOrigin(const Vector2f &orign)
    {
        if (sprite)
            sprite->setOrigin(orign);
    }

    void move(const Vector2f &offset)
    {
        if (sprite)
            sprite->move(offset);
    }

public:
    void setPosition(const Vector2f newPosition)
    {
        if (sprite)
            sprite->setPosition(newPosition);
    }

    bool inSafeZone(const Vector2f point)
    {
        return sprite
                   ? isPointInSafeZoneByCenter(getPosition(), point)
                   : true;
    }

    void setFrame(const IntRect &rect)
    {
        if (sprite)
            sprite->setTextureRect(rect);
    }

    void draw(RenderWindow &window)
    {
        if (sprite)
            window.draw(*sprite);
    }
};

class Cat : public IHasSprite
{
    static constexpr float FRAME_TIME = 0.15f;
    static constexpr int FRAMES_COUNT = 4;
    static constexpr int FRAME_WIDTH = 100;
    static constexpr int FRAME_HEIGHT = 100;
    static constexpr float MOVE_SPEED = 100.f;

    Direction state = Direction::Idle;
    Target target;
    float frameTimer = 0.f;
    int currentFrame = 0;

    // Добрался до места
    void onPlace()
    {
        state = Direction::Idle;
        target.distance = 0.f;
    }

    void movePosition(const float dt)
    {
        // Кот дошёл до цели
        if (target.isInPlace())
        {
            onPlace();
            return;
        }

        // Перемещение — использовать нормализованный вектор
        const float maxDistance = MOVE_SPEED * dt;
        const float moveDistance = min(maxDistance, target.distance);
        move(target.normVector * moveDistance);

        // Уменьшение дистанции
        target.distance -= moveDistance;
    }

    // Нужно ли сдвинуть фрейм
    bool needNextFrame(const float dt)
    {
        // Получаем длительность с момента смены предыдущего фрейма
        frameTimer += dt;
        // Если превышает установленное время фрейма, нужно перейти на следующий
        const bool needNext = frameTimer >= FRAME_TIME;
        if (needNext)
            frameTimer = 0.f;
        return needNext;
    }

    // Установить фрейм с номером currentFrame
    void setCurrentFrame()
    {
        const IntRect rect(
            {currentFrame * FRAME_WIDTH, static_cast<int>(state) * FRAME_HEIGHT}, 
            {FRAME_WIDTH, FRAME_HEIGHT}
        );
        setFrame(rect);
    }

    void moveFrame(const float dt)
    {
        if (needNextFrame(dt))
        {
            currentFrame = (currentFrame + 1) % FRAMES_COUNT;
            setCurrentFrame();
        }
    }

    // Установка направления к цели
    void setDirection()
    {
        const Direction newDirection = target.getDirection();
        // Если новое направление не совпадает со старым => меняем линию в спрайт-листе
        if (newDirection != state) 
        {
            // Смена направления
            state = newDirection;
            // Сборс времени жизни текущего фрейма
            frameTimer = 0.f;
            // Сборс положения в линии
            currentFrame = 0;
            // Установка начального фрейма в линии
            setCurrentFrame();
        }
    }

public:
    Cat(const sf::Texture &texture) : IHasSprite(texture)
    {
        state = Direction::Idle;
        setPosition(WINDOW_CENTER);
        setCurrentFrame();
        setOrigin({FRAME_WIDTH / 2.f, FRAME_HEIGHT / 2.f});
    }

    bool isIdle()
    {
        return state == Direction::Idle;
    }

    bool isMoving()
    {
        return !isIdle();
    }

    void setTarget(const Vector2f point)
    {
        const Vector2f toTarget = point - getPosition();
        // Если лазер в безопасной зоне кота, то кот останавливается, иначе - идёт
        if (isPointInSafeZone(toTarget))
        {
            onPlace();
        }
        else
        {
            // Установка цели
            target.setByVector(toTarget);
            // Установка направления
            setDirection();
        }
    }

    void update(const float dt)
    {
        if (isIdle())
            return;
        moveFrame(dt);
        movePosition(dt);
    }
};

class LaserPointer : public IHasSprite
{

public:
    LaserPointer(const sf::Texture &texture) : IHasSprite(texture)
    {
        const Vector2u textureSize = texture.getSize();
        setOrigin({textureSize.x / 2.f, textureSize.y / 2.f});
    }
};

void pollEvents(
    RenderWindow &window,
    LaserPointer &laserPointer,
    Cat &cat)
{
    while (const auto event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();

        if (const auto *clicked = event->getIf<Event::MouseButtonPressed>())
        {
            const Vector2f mousePosition = {
                static_cast<float>(clicked->position.x),
                static_cast<float>(clicked->position.y)};

            // Клик вне безопасной зоны указки => указка перемещается, изменяется цель кота
            if (!laserPointer.inSafeZone(mousePosition))
            {
                // Смена позиции указки на позицию мыши
                laserPointer.setPosition(mousePosition);

                // Установка цели кота
                cat.setTarget(mousePosition);
            }
        }
    }
}

void update(
    Cat &cat,
    const float dt)
{
    cat.update(dt);
}

void render(
    RenderWindow &window,
    Cat &cat,
    LaserPointer &laserPointer)
{
    window.clear(Color::White);

    if (cat.isMoving())
        laserPointer.draw(window);
    cat.draw(window);

    window.display();
}

int main()
{
    const string CAT_FILE_NAME = "cat_sprites.png";
    Texture catTexture;
    if (!catTexture.loadFromFile(CAT_FILE_NAME))
    {
        cerr << "Invalid load cat texture" << endl;
        return EXIT_FAILURE;
    }

    const string LASER_POINTER_FILE_NAME = "red_pointer.png";
    Texture laserPointerTexture;
    if (!laserPointerTexture.loadFromFile(LASER_POINTER_FILE_NAME))
    {
        cerr << "Invalid load laser texture" << endl;
        return EXIT_FAILURE;
    }

    RenderWindow window(
        VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Cat moves following the laser pointer");

    Clock clock;
    Cat cat(catTexture);
    LaserPointer laserPointer(laserPointerTexture);

    while (window.isOpen())
    {
        pollEvents(window, laserPointer, cat);
        update(cat, clock.restart().asSeconds());
        render(window, cat, laserPointer);
    }

    return EXIT_SUCCESS;
}
