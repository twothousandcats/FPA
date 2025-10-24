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
constexpr float LEFT_DIRECTION = -1.f;
constexpr float RIGHT_DIRECTION = 1.f;

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

struct Target
{
    float distance = 0.f;
    Vector2f normVector = {0.f, 0.f};

    void setByVector(const Vector2f &toTarget)
    {
        distance = toTarget.length();
        normVector = toTarget.normalized();
    }

    float getDirection()
    {
        return normVector.x < 0.f ? LEFT_DIRECTION : RIGHT_DIRECTION;
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

    void shift(const Vector2f &offset)
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

    void draw(RenderWindow &window)
    {
        if (sprite)
            window.draw(*sprite);
    }
};

enum class CatState
{
    Idle,
    Moving,
};

class Cat : public IHasSprite
{
    CatState state = CatState::Idle;
    Target target;
    // Обработан поворот?
    bool rotationProcessed = true;

    void rotate()
    {
        // Выполняем поворот только при первом вызове rotate
        if (rotationProcessed)
            return;

        setScale({target.getDirection(), 1.f});

        // Устанавливаем флаг, что поворот обработан
        rotationProcessed = true;
    }

    // Добрался до места
    void onPlace()
    {
        target.distance = 0.f;
    }

    void move(const float dt)
    {
        // Кот дошёл до цели
        if (target.isInPlace())
        {
            state = CatState::Idle;
            onPlace();
            return;
        }

        constexpr float MOVE_SPEED = 100.f;

        // Перемещение — использовать нормализованный вектор
        const float maxDistance = MOVE_SPEED * dt;
        const float moveDistance = min(maxDistance, target.distance);
        shift(target.normVector * moveDistance);

        // Уменьшение дистанции
        target.distance -= moveDistance;
    }

public:
    Cat(const sf::Texture &texture) : IHasSprite(texture)
    {
        state = CatState::Idle;
        setPosition(WINDOW_CENTER);
        const Vector2u textureSize = texture.getSize();
        setOrigin({textureSize.x / 2.f, textureSize.y / 2.f});
    }

    bool isIdle()
    {
        return state == CatState::Idle;
    }

    bool isMoving()
    {
        return state == CatState::Moving;
    }

    void setTarget(const Vector2f point)
    {
        const Vector2f toTarget = point - getPosition();
        // Если лазер в безопасной зоне кота, то кот останавливается, иначе - идёт
        state = isPointInSafeZone(toTarget)
                    ? CatState::Idle
                    : CatState::Moving;
        // Корректировка цели в зависимости от состояния кота
        if (state == CatState::Moving)
        {
            target.setByVector(toTarget);
            rotationProcessed = false;
        }
        else
            onPlace();
    }

    void update(const float dt)
    {
        if (isIdle())
            return;
        rotate();
        move(dt);
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
        {
            window.close();
        }
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

void render(
    RenderWindow &window,
    Cat &cat,
    LaserPointer &laserPointer)
{
    window.clear(Color::White);

    cat.draw(window);
    if (cat.isMoving())
    {
        laserPointer.draw(window);
    }

    window.display();
}

int main()
{
    const string CAT_FILE_NAME = "cat.png";
    const string LASER_POINTER_FILE_NAME = "red_pointer.png";

    try
    {
        const Texture catTexture(CAT_FILE_NAME);
        const Texture laserPointerTexture(LASER_POINTER_FILE_NAME);

        Cat cat(catTexture);
        LaserPointer laserPointer(laserPointerTexture);

        RenderWindow window(
            VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
            "Cat moves following the laser pointer");

        Clock clock;

        while (window.isOpen())
        {
            pollEvents(window, laserPointer, cat);
            cat.update(clock.restart().asSeconds());
            render(window, cat, laserPointer);
        }
    }
    catch (const sf::Exception &error)
    {
        cerr << "SFML Error: " << error.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
