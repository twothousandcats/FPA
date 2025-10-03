#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

int main() {
    constexpr int pointCount = 200;
    constexpr Vector2f ellipseRadius = {200.0f, 80.f};

    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(
        VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Ellipse",
        Style::Default,
        State::Windowed,
        settings);

    ConvexShape ellipse;
    ellipse.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
    ellipse.setFillColor(Color(0xFF, 0xFF, 0xFF, 0xFF));
    ellipse.setPointCount(pointCount);

    for (int pointNumber = 0; pointNumber < pointCount; ++pointNumber) {
        const float angle = float(2 * M_PI * pointNumber) / float(pointCount);
        Vector2f point = Vector2f{
            ellipseRadius.x * sin(angle),
            ellipseRadius.y * cos(angle)
        };
        ellipse.setPoint(pointNumber, point);
    }

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(ellipse);
        window.display();
    }
}
