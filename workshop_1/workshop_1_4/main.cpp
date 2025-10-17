#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr Vector2f WINDOW_CENTER = { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f };

void setOrignInCenterSprite(
    Sprite &sprite,
    const Texture &texture
) {
    const Vector2u textureSize = texture.getSize();
    sprite.setOrigin({ textureSize.x / 2.f, textureSize.y / 2.f });
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
    Sprite &lazerPointer
) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
        if (const auto *clicked = event->getIf<Event::MouseButtonPressed>()) {
            lazerPointer.setPosition({
                static_cast<float>(clicked->position.x), 
                static_cast<float>(clicked->position.y)
            });
        }
    }
}

void rotateCat(
    Sprite &cat,
    const float targetAngle
) {
    const float direction = cos(targetAngle) < 0.f ? -1.f : 1.f;
    cat.setScale({ direction, 1 });
}

void moveCat(
    Sprite &cat,
    const float targetAngle,
    const Vector2f toTarget,
    const float dt
) {
    constexpr float STOP_MOVE_DISTANCE = 1.f;
    constexpr float MOVE_SPEED = 20.f;

    const float targetDistance = hypot(toTarget.x, toTarget.y);
    if (targetDistance <= STOP_MOVE_DISTANCE) {
        return;
    }

    const float maxDistance = MOVE_SPEED * dt;
    const float moveDistance = min(maxDistance, targetDistance);
    // Единичный вектор направления
    const Vector2f direction{
        cos(targetAngle),
        sin(targetAngle)
    };
    cat.move(direction * moveDistance);
}

void updateCat(
    Sprite &cat,
    const Sprite &lazerPointer,
    const float dt
) {
    const Vector2f lazerPointerPosition = lazerPointer.getPosition();
    const Vector2f catPosition = cat.getPosition();
    const Vector2f toTarget = lazerPointerPosition - catPosition;
    // Целевой угл в радианах [-180, 180]
    const float targetAngle = atan2(toTarget.y, toTarget.x);

    rotateCat(cat, targetAngle);
    moveCat(cat, targetAngle, toTarget, dt);
}

void render(
    RenderWindow &window,
    const Sprite &cat,
    const Sprite &lazerPointer
) {
    window.clear(Color::White);
    window.draw(lazerPointer);
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
            VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
            "Cat moves following the lazer pointer"
        );

        Clock clock;
        while (window.isOpen()) {
            pollEvents(window, lazerPointer);
            updateCat(cat, lazerPointer, clock.restart().asSeconds());
            render(window, cat, lazerPointer);
        }
        
    } catch (const sf::Exception& error) {
        cerr << "SFML Error: " << error.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
