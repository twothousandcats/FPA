#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr Vector2f WINDOW_CENTER = {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f};
constexpr float SAFE_ZONE_RADIUS = 2.f;

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

enum class CatState
{
    Idle,
    Moving,
};

struct Target
{
    float distance = 0.f;
    Vector2f normVector = {0.f, 0.f};

    void clear()
    {
        distance = 0.f;
        normVector = {0.f, 0.f};
    }

    void setByVector(const Vector2f &toTarget)
    {
        distance = toTarget.length();
        normVector = toTarget.normalized();
    }
};

void setOrignInCenterSprite(
    Sprite &sprite,
    const Texture &texture)
{
    const Vector2u textureSize = texture.getSize();
    sprite.setOrigin({textureSize.x / 2.f, textureSize.y / 2.f});
}

void initCat(
    Sprite &cat,
    const Texture &texture)
{
    cat.setPosition(WINDOW_CENTER);
    setOrignInCenterSprite(cat, texture);
}

void initLaserPointer(
    Sprite &laserPointer,
    const Texture &texture)
{
    laserPointer.setPosition(WINDOW_CENTER);

    setOrignInCenterSprite(laserPointer, texture);
}

void pollEvents(
    RenderWindow &window,
    Sprite &laserPointer,
    const Sprite &cat,
    CatState &state,
    Target &target)
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

            // Клик вне безопасной зоны указки
            if (!isPointInSafeZoneByCenter(laserPointer.getPosition(), mousePosition))
            {
                // Смена позиции указки на позицию мыши
                laserPointer.setPosition(mousePosition);

                // Корректировка состояния кота.
                // Если лазер в безопасной зоне кота, то кот останавливается, иначе - идёт
                const Vector2f toTarget = mousePosition - cat.getPosition();
                state = isPointInSafeZone(toTarget)
                            ? CatState::Idle
                            : CatState::Moving;
                if (state == CatState::Moving)
                    target.setByVector(toTarget);
                else
                    target.clear();
            }
        }
    }
}

void rotateCat(
    Sprite &cat,
    const Vector2f direction)
{
    constexpr float TO_LEFT_DIRECTION = -1.f;
    const float directionScale = direction.x < 0.f
                                     ? TO_LEFT_DIRECTION
                                     : -TO_LEFT_DIRECTION;
    cat.setScale({directionScale, 1.f});
}

void moveCat(
    Sprite &cat,
    CatState &catState,
    Target &target,
    const float dt)
{
    if (target.distance <= SAFE_ZONE_RADIUS)
    {
        catState = CatState::Idle;
        // Нет очистки target, чтобы сохранить значение для scale
        return;
    }

    constexpr float MOVE_SPEED = 100.f;

    // Перемещение — использовать нормализованный вектор
    const float maxDistance = MOVE_SPEED * dt;
    const float moveDistance = min(maxDistance, target.distance);
    cat.move(target.normVector * moveDistance);

    target.distance -= moveDistance;
}

void updateCat(
    Sprite &cat,
    CatState &catState,
    Target &target,
    const float dt)
{
    if (catState == CatState::Idle)
    {
        return;
    }

    moveCat(cat, catState, target, dt);
    rotateCat(cat, target.normVector);
}

void render(
    RenderWindow &window,
    const Sprite &cat,
    const Sprite &laserPointer,
    const bool laserPointerVisible)
{
    window.clear(Color::White);
    if (laserPointerVisible)
    {
        window.draw(laserPointer);
    }

    window.draw(cat);
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

        Sprite cat(catTexture);
        initCat(cat, catTexture);

        Sprite laserPointer(laserPointerTexture);
        initLaserPointer(laserPointer, laserPointerTexture);

        RenderWindow window(
            VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
            "Cat moves following the laser pointer");

        Clock clock;
        auto catState = CatState::Idle;
        Target target;

        while (window.isOpen())
        {
            pollEvents(window, laserPointer, cat, catState, target);
            updateCat(cat, catState, target, clock.restart().asSeconds());
            render(window, cat, laserPointer, catState == CatState::Moving);
        }
    }
    catch (const sf::Exception &error)
    {
        cerr << "SFML Error: " << error.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
