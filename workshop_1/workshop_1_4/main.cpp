#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr Vector2f WINDOW_CENTER = {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f};

enum class CatState {
    IDLE,
    MOVING,
};

void setOrignInCenterSprite(
    Sprite &sprite,
    const Texture &texture
) {
    const Vector2u textureSize = texture.getSize();
    sprite.setOrigin({textureSize.x / 2.f, textureSize.y / 2.f});
}

void initCat(
    Sprite &cat,
    const Texture &texture
) {
    cat.setPosition(WINDOW_CENTER);
    setOrignInCenterSprite(cat, texture);
}

void initLazerPointer(
    Sprite &lazerPointer,
    const Texture &texture
) {
    lazerPointer.setPosition(WINDOW_CENTER);

    setOrignInCenterSprite(lazerPointer, texture);
}

void pollEvents(
    RenderWindow &window,
    Sprite &lazerPointer,
    bool &lazerPointerVisible,
    Vector2f &targetPosition,
    CatState &state
) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
        if (const auto *clicked = event->getIf<Event::MouseButtonPressed>()) {
            targetPosition = {
                static_cast<float>(clicked->position.x),
                static_cast<float>(clicked->position.y)
            };
            lazerPointerVisible = true;
            state = CatState::MOVING;
            lazerPointer.setPosition(targetPosition);
        }
    }
}

void rotateCat(
    Sprite &cat,
    const Vector2f direction
) {
    constexpr float TO_LEFT_DIRECTION = -1.f;
    const float directionScale = direction.x < 0.f
                                     ? TO_LEFT_DIRECTION
                                     : -TO_LEFT_DIRECTION;
    cat.setScale({directionScale, 1.f});
}

void moveCat(
    Sprite &cat,
    CatState &catState,
    bool &lazerPointerVisible,
    const Vector2f toTarget,
    const float &targetDistance,
    const float dt
) {
    constexpr float STOP_MOVE_DISTANCE = 1.f;
    constexpr float MOVE_SPEED = 100.f;

    if (targetDistance <= STOP_MOVE_DISTANCE) {
        catState = CatState::IDLE;
        lazerPointerVisible = false;
        return;
    }

    // Перемещение — использовать нормализованный вектор
    const float maxDistance = MOVE_SPEED * dt;
    const float moveDistance = min(maxDistance, targetDistance);

    // Нормализуем вектор направления
    const Vector2f direction = toTarget / targetDistance; // единичный вектор

    cat.move(direction * moveDistance);

    // Поворот через scale
    rotateCat(cat, direction);
}

void updateCat(
    Sprite &cat,
    const Vector2f &targetPosition,
    CatState &catState,
    bool &lazerPointerVisible,
    const float dt
) {
    if (catState == CatState::IDLE) {
        return;
    }

    const Vector2f catPosition = cat.getPosition();
    const Vector2f toTarget = targetPosition - catPosition;
    const float targetDistance = hypot(toTarget.x, toTarget.y);

    moveCat(cat, catState, lazerPointerVisible, toTarget, targetDistance, dt);
}

void render(
    RenderWindow &window,
    const Sprite &cat,
    const Sprite &lazerPointer,
    const bool lazerPointerVisible
) {
    window.clear(Color::White);
    if (lazerPointerVisible) {
        window.draw(lazerPointer);
    }

    window.draw(cat);
    window.display();
}

int main() {
    const string CAT_FILE_NAME = "cat.png";
    const string LAZER_POINTER_FILE_NAME = "red_pointer.png";

    try {
        const Texture catTexture(CAT_FILE_NAME);
        const Texture lazerPointerTexture(LAZER_POINTER_FILE_NAME);

        Sprite cat(catTexture);
        initCat(cat, catTexture);

        Sprite lazerPointer(lazerPointerTexture);
        initLazerPointer(lazerPointer, lazerPointerTexture);

        RenderWindow window(
            VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
            "Cat moves following the lazer pointer"
        );

        Clock clock;
        auto catState = CatState::IDLE;
        Vector2f targetPosition;
        bool lazerPointerVisible = false;

        while (window.isOpen()) {
            pollEvents(window, lazerPointer, lazerPointerVisible, targetPosition, catState);
            updateCat(cat, targetPosition, catState, lazerPointerVisible, clock.restart().asSeconds());
            render(window, cat, lazerPointer, lazerPointerVisible);
        }
    } catch (const sf::Exception &error) {
        cerr << "SFML Error: " << error.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
