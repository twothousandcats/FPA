#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr float MAX_DEGREES = 15.f;

float toDegrees(const float radians) {
    return static_cast<float>(static_cast<double>(radians) * 180.0 / M_PI);
}

float normalizeAngle(float angle) {
    while (angle > 180.f) {
        angle -= 360.f;
    }
    while (angle <= -180.f) {
        angle += 360.f;
    }

    return angle;
}

void onMouseMove(const Event::MouseMoved evt, Vector2f &mousePosition) {
    mousePosition = Vector2f(evt.position);
}

void init(ConvexShape &pointer) {
    pointer.setPointCount(3);
    pointer.setPoint(0, {40, 0});
    pointer.setPoint(1, {-20, -20});
    pointer.setPoint(2, {-20, 20});
    pointer.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
    pointer.setFillColor(Color({0xFF, 0x80, 0x00, 0xFF}));
}

void pollEvents(RenderWindow &window, Vector2f &mousePosition) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
        if (const auto mouseButtonMoved = event->getIf<Event::MouseMoved>()) {
            onMouseMove(*mouseButtonMoved, mousePosition);
        }
    }
}

void update(
    const Vector2f &mousePosition,
    ConvexShape &pointer,
    const float &dt
) {
    const Vector2f delta = mousePosition - pointer.getPosition();
    const float targetAngle = toDegrees(atan2(delta.y, delta.x));

    const float currentAngle = pointer.getRotation().asDegrees(); // [0, 360);
    const float currentNormalizedAngle = normalizeAngle(currentAngle); // [-180, 180);
    const float diffAngle = normalizeAngle(targetAngle - currentNormalizedAngle);

    const float maxChange = MAX_DEGREES * dt;
    const float clampedDiff = max(-maxChange, min(diffAngle, maxChange));

    pointer.setRotation(degrees(currentNormalizedAngle + clampedDiff));
}

void renderFrame(RenderWindow &window, const ConvexShape &pointer) {
    window.clear();
    window.draw(pointer);
    window.display();
}

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;

    RenderWindow window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                        "Pointer Follows Mouse",
                        Style::Default,
                        State::Windowed,
                        settings);

    ConvexShape pointer;
    Vector2f mousePosition;
    init(pointer);
    Clock clock;

    while (window.isOpen()) {
        pollEvents(window, mousePosition);
        update(mousePosition, pointer, clock.restart().asSeconds());
        renderFrame(window, pointer);
    }
}
