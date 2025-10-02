#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

int main() {
    constexpr float BALL_SIZE = 40;

    RenderWindow window(VideoMode({
        WINDOW_WIDTH,
        WINDOW_HEIGHT}), "Wave Moving Ball");
    Clock clock;

    const Vector2f position = {10, 50};

    CircleShape ball(BALL_SIZE);
    ball.setFillColor(Color(0xFF, 0xFF, 0xFF));

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        constexpr float speedX = 100.f;
        constexpr float amplitudeY = 80.f;
        constexpr float periodY = 2;

        const float time = clock.getElapsedTime().asSeconds();
        const float wavePhase = time * float(2 * M_PI);
        const float x = speedX * time;
        const float y = amplitudeY * sin(wavePhase / periodY);
        const Vector2f offset = {x, y};

        ball.setPosition(position + offset);

        window.clear();
        window.draw(ball);
        window.display();
    }
}