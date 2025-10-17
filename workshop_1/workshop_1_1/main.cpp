#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

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
    constexpr Angle START_ANGLE = degrees(-80);

    // Установка точек
    arrow.setPointCount(7);
    arrow.setPoint(0, { 0, 0 }); // нос
    arrow.setPoint(1, { -HALF_SIZE, HALF_SIZE });
    arrow.setPoint(2, { -HALF_SIZE, QUARTER_SIZE });
    arrow.setPoint(3, { -SIZE, QUARTER_SIZE });
    arrow.setPoint(4, { -SIZE, -QUARTER_SIZE });
    arrow.setPoint(5, { -HALF_SIZE, -QUARTER_SIZE });
    arrow.setPoint(6, { -HALF_SIZE, -HALF_SIZE });

    arrow.setOrigin({0, 0});
    arrow.setRotation(START_ANGLE);
    arrow.setFillColor(COLOR);
    arrow.setOutlineColor(LINE_COLOR);
    arrow.setOutlineThickness(LINE_THICKNESS);
    arrow.setPosition({ START_X, START_Y });
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
        VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
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
