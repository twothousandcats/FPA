#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr float MOVE_SPEED = 20.f;
constexpr float ROTATION_SPEED_DEG = 180.f;

float toDegrees(
    const float radians
) {
    return radians * 180.f / static_cast<float>(M_PI);
}

float toRadians(
    const float degrees
) {
    return degrees * static_cast<float>(M_PI) / 180.f;
}

float normalizeAngle(
    float angle
) {
    while (angle > 180.f) angle -= 360.f;
    while (angle <= -180.f) angle += 360.f;
    return angle;
}

void initArrow(
    ConvexShape &arrow
) {
    arrow.setPointCount(7);
    arrow.setPoint(0, {0, 0}); // нос
    arrow.setPoint(1, {-50, 50});
    arrow.setPoint(2, {-50, 25});
    arrow.setPoint(3, {-100, 25});
    arrow.setPoint(4, {-100, -25});
    arrow.setPoint(5, {-50, -25});
    arrow.setPoint(6, {-50, -50});

    arrow.setOrigin({0, 0});
    arrow.setFillColor(Color::Yellow);
    arrow.setOutlineColor(Color::Black);
    arrow.setOutlineThickness(2.f);
    arrow.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
}

void pollEvents(
    RenderWindow &window,
    Vector2f &mousePos
) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
        if (const auto *moved = event->getIf<Event::MouseMoved>()) {
            mousePos = {static_cast<float>(moved->position.x), static_cast<float>(moved->position.y)};
        }
    }
}

void rotateArrow(
    ConvexShape &arrow,
    const Vector2f &toTargetV,
    const float dt
) {
    const float targetAngle = toDegrees(atan2(toTargetV.y, toTargetV.x)); // целевой угл
    const float currentAngle = arrow.getRotation().asDegrees(); // [0, 360)
    const float currentNormalized = normalizeAngle(currentAngle); // [-180, 180)
    const float diff = normalizeAngle(targetAngle - currentNormalized);
    // разница м-у целевым углом и текущим [-180, 180)

    const float maxRot = ROTATION_SPEED_DEG * dt;
    const float newAngle = currentNormalized + clamp(diff, -maxRot, maxRot); // обновленный за дельту
    arrow.setRotation(degrees(newAngle));
}

void moveArrow(
    ConvexShape &arrow,
    const float distance,
    const float dt
) {
    const float maxDiff = MOVE_SPEED * dt;
    const float moveDist = min(maxDiff, distance);
    const float angleRad = toRadians(arrow.getRotation().asDegrees()); // текущий угол
    const Vector2f direction{
        cos(angleRad),
        sin(angleRad)
    }; // получаем единичный вектор направления
    arrow.move(direction * moveDist);
}

void updateArrow(
    ConvexShape &arrow,
    const Vector2f &target,
    const float dt
) {
    const Vector2f toTarget = target - arrow.getPosition();
    const float distance = hypot(toTarget.x, toTarget.y);

    if (distance > 1.f) {
        rotateArrow(arrow, toTarget, dt);
    }

    if (distance > 2.f) {
        moveArrow(arrow, distance, dt);
    }
}

void renderFrame(
    RenderWindow &window,
    const ConvexShape &arrow
) {
    window.clear(Color::White);
    window.draw(arrow);
    window.display();
}

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;

    RenderWindow window(
        VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Arrow Follows Mouse",
        Style::Default,
        State::Windowed,
        settings
    );

    ConvexShape arrow;
    Vector2f mousePos{WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f};
    initArrow(arrow);

    Clock clock;
    while (window.isOpen()) {
        const float dt = clock.restart().asSeconds();
        pollEvents(window, mousePos);
        updateArrow(arrow, mousePos, dt);
        renderFrame(window, arrow);
    }

    return 0;
}
