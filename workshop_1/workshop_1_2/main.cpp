#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Graphics.hpp>
#include <algorithm>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr float MOVE_SPEED = 20.f;
constexpr float ROTATION_SPEED_DEG = 90.f;

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
    constexpr float SIZE = 100.f;
    constexpr Color COLOR = Color::Yellow;
    constexpr Color LINE_COLOR = Color::Black;
    constexpr float LINE_THICKNESS = 2.f;
    constexpr float HALF_SIZE = SIZE / 2.f;
    constexpr float QUARTER_SIZE = HALF_SIZE / 2.f;
    constexpr float START_X = WINDOW_WIDTH / 2.f;
    constexpr float START_Y = WINDOW_HEIGHT / 2.f - HALF_SIZE;

    // Установка точек
    arrow.setPointCount(7);
    arrow.setPoint(0, { 0, 0 }); // нос
    arrow.setPoint(1, { -HALF_SIZE, HALF_SIZE });
    arrow.setPoint(2, { -HALF_SIZE, QUARTER_SIZE });
    arrow.setPoint(3, { -SIZE, QUARTER_SIZE });
    arrow.setPoint(4, { -SIZE, -QUARTER_SIZE });
    arrow.setPoint(5, { -HALF_SIZE, -QUARTER_SIZE });
    arrow.setPoint(6, { -HALF_SIZE, -HALF_SIZE });

    arrow.setOrigin({ 0, 0 });
    arrow.setFillColor(COLOR);
    arrow.setOutlineColor(LINE_COLOR);
    arrow.setOutlineThickness(LINE_THICKNESS);
    arrow.setPosition({ START_X, START_Y });
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
            mousePos = { 
                static_cast<float>(moved->position.x), 
                static_cast<float>(moved->position.y) 
            };
        }
    }
}

void rotateArrow(
    ConvexShape &arrow,
    const float targetAngle,
    const float targetDistance,
    const float dt
) {
    constexpr float STOP_ROTATION_DISTANCE = 2.f;
    if (targetDistance <= STOP_ROTATION_DISTANCE) {
        return;
    }

    // Текущий угол [-180, 180)
    const float currentAngle = arrow.getRotation().wrapSigned().asDegrees();
    // Разница м-у целевым углом и текущим [-180, 180)
    const float diff = normalizeAngle(toDegrees(targetAngle) - currentAngle);

    const float maxAngle = ROTATION_SPEED_DEG * dt;
    // Угол, обновленный за дельту
    const float deltaAngle = clamp(diff, -maxAngle, maxAngle);
    arrow.rotate(degrees(deltaAngle));
}

void moveArrow(
    ConvexShape &arrow,
    const float targetAngle,
    const float targetDistance,
    const float dt
) {
    constexpr float STOP_MOVE_DISTANCE = 1.f;
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
    arrow.move(direction * moveDistance);
}

void updateArrow(
    ConvexShape &arrow,
    const Vector2f &target,
    const float dt
) {
    const Vector2f toTarget = target - arrow.getPosition();
    // Целевой угл в радианах [-180, 180]
    const float targetAngle = atan2(toTarget.y, toTarget.x);
    const float distance = hypot(toTarget.x, toTarget.y);
    
    rotateArrow(arrow, targetAngle, distance, dt);
    moveArrow(arrow, targetAngle, distance, dt);
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
        VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
        "Arrow Follows Mouse",
        Style::Default,
        State::Windowed,
        settings
    );

    Vector2f mousePos{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f };
    
    ConvexShape arrow;
    initArrow(arrow);

    Clock clock;
    while (window.isOpen()) {
        pollEvents(window, mousePos);
        updateArrow(arrow, mousePos, clock.restart().asSeconds());
        renderFrame(window, arrow);
    }

    return 0;
}
