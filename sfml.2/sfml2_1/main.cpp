#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr float BALL_SIZE = 40;

void initBall(
    CircleShape &pointer,
    const Color &color,
    const Vector2f &position
) {
    pointer.setFillColor(Color(color));
    pointer.setPosition(position);
}

void redrawFrame(RenderWindow &window, CircleShape &pointer) {
    window.clear();
    window.draw(pointer);
    window.display();
}

int main() {
    RenderWindow window(VideoMode({
                            WINDOW_WIDTH,
                            WINDOW_HEIGHT
                        }), "Wave Moving Ball");
    Clock clock;
    Vector2f position = {BALL_SIZE, WINDOW_HEIGHT / 2.f - BALL_SIZE};
    float speedX = 200.f;
    float totalTime = 0.f;

    CircleShape ball(BALL_SIZE);
    initBall(ball, {0xFF, 0xFF, 0xFF}, position);

    while (window.isOpen()) {
        constexpr float amplitudeY = 80.f;
        constexpr float periodY = 1.f;

        while (const auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }

        const float deltaTime = clock.restart().asSeconds();
        totalTime += deltaTime;
        position.x += speedX * deltaTime;
        if (position.x + BALL_SIZE * 2 >= WINDOW_WIDTH) {
            speedX = -speedX;
        }
        if (position.x < 0) {
            speedX = -speedX;
        }

        const float wavePhase = totalTime * static_cast<float>(2 * M_PI);
        const float offsetY = amplitudeY * sin(wavePhase / periodY);
        ball.setPosition({position.x, position.y + offsetY});

        redrawFrame(window, ball);
    }
}
