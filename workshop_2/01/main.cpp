#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr float BALL_SIZE = 40;
constexpr float HUNDRED = 100.f;

// speed
constexpr Vector2f LOW_SPEED = {HUNDRED * 2.f, HUNDRED * 2.f};
constexpr Vector2f MIDDLE_SPEED = {HUNDRED * 3.f, HUNDRED * 3.f};
constexpr Vector2f TOP_SPEED = {HUNDRED * 4.f, HUNDRED * 4.f};

// colors
constexpr Color RED_COLOR = {255, 0, 0};
constexpr Color GREEN_COLOR = {0, 255, 0};
constexpr Color BLUE_COLOR = {0, 0, 255};
constexpr Color YELLOW_COLOR = {255, 255, 0};
constexpr Color PURPLE_COLOR = {255, 0, 255};

// positions
constexpr Vector2f TOP_LEFT = {0, 0};
constexpr Vector2f TOP_RIGHT = {WINDOW_WIDTH, WINDOW_HEIGHT};
constexpr Vector2f BOTTOM_LEFT = {0, WINDOW_WIDTH - 1};
constexpr Vector2f BOTTOM_RIGHT = {WINDOW_WIDTH, WINDOW_HEIGHT - 1};
constexpr Vector2f CENTER = {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f};

struct Ball {
    CircleShape base;
    Vector2f speed;
    float radius = BALL_SIZE;
    Color color;

    Ball(
        const Color &c,
        const Vector2f &pos,
        const Vector2f &spd
    ) : speed(spd), color(c) {
        base.setRadius(radius);
        base.setFillColor(color);
        base.setPosition(pos);
    }
};

void initBall(
    Ball &ball,
    const Color &color,
    const Vector2f &position,
    const Vector2f &speed = LOW_SPEED
) {
    ball.base.setRadius(ball.radius);
    ball.base.setFillColor(color);
    ball.base.setPosition(position);
    ball.speed = speed;
};

void pollEvents(RenderWindow &window) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
    }
}

void setNewPosition(Ball &ball, float deltaTime) {
    auto &shape = ball.base;
    auto &speed = ball.speed;

    Vector2f pos = shape.getPosition();
    Vector2f newPos = pos + speed * deltaTime;

    const float diameter = 2 * ball.radius;

    // Отскок по X
    if (newPos.x < 0) {
        newPos.x = 0;
        speed.x = -speed.x;
    } else if (newPos.x + diameter > WINDOW_WIDTH) {
        newPos.x = WINDOW_WIDTH - diameter;
        speed.x = -speed.x;
    }

    // Отскок по Y
    if (newPos.y < 0) {
        newPos.y = 0;
        speed.y = -speed.y;
    } else if (newPos.y + diameter > WINDOW_HEIGHT) {
        newPos.y = WINDOW_HEIGHT - diameter;
        speed.y = -speed.y;
    }

    shape.setPosition(newPos);
}

void update(
    vector<Ball> &balls,
    Clock &clock
) {
    const float dt = clock.restart().asSeconds();
    for (Ball &ball: balls) {
        setNewPosition(ball, dt);
    }
};

void render(
    RenderWindow &window,
    const vector<Ball> &balls
) {
    window.clear();
    for (const auto &ball: balls) {
        window.draw(ball.base);
    }
    window.display();
};

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;

    RenderWindow window(
        VideoMode({
            WINDOW_WIDTH,
            WINDOW_HEIGHT
        }),
        "Bouncing Balls With Constant Speed",
        Style::Default,
        State::Windowed,
        settings
    );
    Clock clock;

    vector<Ball> balls = {
        {BLUE_COLOR, TOP_LEFT, LOW_SPEED},
        {RED_COLOR, TOP_RIGHT, MIDDLE_SPEED},
        {GREEN_COLOR, BOTTOM_LEFT, TOP_SPEED},
        {YELLOW_COLOR, BOTTOM_RIGHT, {-HUNDRED * 20.f, HUNDRED * 2.f}},
        {PURPLE_COLOR, CENTER, {-HUNDRED * 2.f, -HUNDRED * 7.f}},
    };

    while (window.isOpen()) {
        pollEvents(window);
        update(balls, clock);
        render(window, balls);
    }
}
