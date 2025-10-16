#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

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
    RenderWindow &window
) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
    }
}

void redraw(
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
        "Arrow with outline",
        Style::Default,
        State::Windowed,
        settings
    );

    ConvexShape arrow;
    initArrow(arrow);

    while (window.isOpen()) {
        pollEvents(window);
        redraw(window, arrow);
    }

    return 0;
}
