#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <set>

using namespace sf;
using namespace std;

struct Eye {
    ConvexShape base;
    ConvexShape pupil;
    Vector2f position;
};

constexpr unsigned WINDOW_WIDTH = 1000;
constexpr unsigned WINDOW_HEIGHT = 800;
constexpr Vector2f BASE_RADIUS = {80.f, 160.f};
constexpr Vector2f PUPIL_RADIUS = {20.f, 40.f};

void initEllipse(
    ConvexShape &ellipse,
    const Vector2f &radius,
    const int &pointCount
) {
    for (int pointNumber = 0; pointNumber < pointCount; ++pointNumber) {
        const float angle = static_cast<float>(2 * M_PI * pointNumber) / static_cast<float>(pointCount);
        const Vector2f point = {
            radius.x * sin(angle),
            radius.y * cos(angle)
        };
        ellipse.setPoint(pointNumber, point);
    }
}

void initEye(
    Eye &eye,
    const Vector2f &eyePosition
) {
    constexpr int pointCount = 200;
    eye.position = eyePosition;
    eye.base.setPosition(eyePosition);
    eye.base.setFillColor(Color::White);
    eye.base.setPointCount(pointCount);

    eye.pupil.setPosition(eyePosition);
    eye.pupil.setFillColor(Color::Black);
    eye.pupil.setPointCount(pointCount);

    initEllipse(eye.base, BASE_RADIUS, pointCount);
    initEllipse(eye.pupil, PUPIL_RADIUS, pointCount);
};

void onMouseMove(
    const Event::MouseMoved &evt,
    Vector2f &mousePosition
) {
    mousePosition = Vector2f(evt.position);
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

// нормализация вектора
Vector2f clampToEllipse(
    const Vector2f delta,
    const Vector2f maxOffset
) {
    if (maxOffset.x <= 0 || maxOffset.y <= 0) {
        return {0, 0};
    }

    const float nx = delta.x / maxOffset.x;
    const float ny = delta.y / maxOffset.y;
    const float len = sqrt(nx * nx + ny * ny);
    if (len <= 1.0f) {
        return delta;
    }
    return Vector2f{(nx / len) * maxOffset.x, (ny / len) * maxOffset.y};
}

void update(
    const Vector2f &mousePosition,
    Eye &eye
) {
    constexpr Vector2f maxOffset = {
        BASE_RADIUS.x / 1.5f - PUPIL_RADIUS.x,
        BASE_RADIUS.y / 1.5f - PUPIL_RADIUS.y
    };
    const Vector2f delta = mousePosition - eye.position;
    const Vector2f offset = clampToEllipse(delta, maxOffset);
    eye.pupil.setPosition(eye.position + offset);
}

void rerender(
    RenderWindow &window,
    const Eye &leftEye,
    const Eye &rightEye
) {
    window.clear();
    window.draw(leftEye.base);
    window.draw(leftEye.pupil);
    window.draw(rightEye.base);
    window.draw(rightEye.pupil);
    window.display();
}

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(
        VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Eyes",
        Style::Default,
        State::Windowed,
        settings
    );

    Eye leftEye, rightEye;
    Vector2f mousePosition;

    initEye(leftEye, {
                WINDOW_WIDTH / 2.f - 100, WINDOW_HEIGHT / 2.f
            });
    initEye(rightEye, {
                WINDOW_WIDTH / 2.f + 100, WINDOW_HEIGHT / 2.f
            });

    while (window.isOpen()) {
        pollEvents(window, mousePosition);
        update(mousePosition, leftEye);
        update(mousePosition, rightEye);
        rerender(window, leftEye, rightEye);
    }
}
