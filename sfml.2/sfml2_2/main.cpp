#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

void initRose(
    ConvexShape &pointer,
    const Color &color,
    const Vector2f &windowSize,
    const int pointCount) {
    pointer.setPosition(windowSize);
    pointer.setFillColor(Color(color));
    pointer.setPointCount(pointCount);
}

void drawRose(ConvexShape &pointer, const int pointCount) {
    for (int pointNumber = 0; pointNumber < pointCount; ++pointNumber) {
        constexpr int petalCount = 6;
        const float baseRadius = 200.f;
        const float angle = float(2 * M_PI * pointNumber) / float(pointCount);
        const float radius = baseRadius * sin(petalCount * angle);
        const Vector2f point = Vector2f{
            radius * sin(angle),
            radius * cos(angle)
        };
        pointer.setPoint(pointNumber, point);
    }
}

int main() {
    constexpr int pointCount = 200;
    // constexpr Vector2f ellipseRadius = {200.0f, 80.f};
    constexpr Vector2f orbitCenter = {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f};

    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(
        VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Polar Rose",
        Style::Default,
        State::Windowed,
        settings);

    Clock clock;
    float time = 0.f;

    ConvexShape rose;
    initRose(
        rose,
        {0xFF, 0x09, 0x80},
        {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f},
        pointCount
    );
    drawRose(rose, pointCount);

    while (window.isOpen()) {
        constexpr float orbitRadius = 100.f;
        constexpr float speed = 1.5f;
        const float deltaTime = clock.restart().asSeconds();
        time += deltaTime;

        while (const auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        const float orbitAngle = speed * time;
        const Vector2f newPosition = {
            orbitCenter.x + orbitRadius * sin(orbitAngle),
            orbitCenter.y + orbitRadius * cos(orbitAngle)
        };

        rose.setPosition(newPosition);

        window.clear();
        window.draw(rose);
        window.display();
    }
}
