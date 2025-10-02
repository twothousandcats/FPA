#include <SFML/Graphics.hpp>

using namespace sf;

int main() {
    constexpr float BALL_SIZE = 40;
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    RenderWindow window(VideoMode({
        WINDOW_WIDTH,
        WINDOW_HEIGHT}), "Simple Event Loop");
    Clock clock;

    CircleShape shape(BALL_SIZE);
    shape.setFillColor(Color(0xFF, 0xFF, 0xFF));
    shape.setPosition(Vector2f(200, 120));

    Vector2f speed = {200.f, 200.f};

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        const float deltaTime = clock.restart().asSeconds();
        Vector2f newPosition = shape.getPosition();
        newPosition += speed * deltaTime;

        if ((newPosition.x + 2 * BALL_SIZE >= WINDOW_WIDTH) && (speed.x > 0)) {
            speed.x = -speed.x;
        }
        if ((newPosition.x < 0) && (speed.x < 0)) {
            speed.x = -speed.x;
        }
        if ((newPosition.y + 2 * BALL_SIZE >= WINDOW_HEIGHT) && (speed.y > 0)) {
            speed.y = -speed.y;
        }
        if ((newPosition.y < 0) && (speed.y)) {
            speed.y = -speed.y;
        }

        shape.setPosition(newPosition);

        window.clear();
        window.draw(shape);
        window.display();
    }
}
