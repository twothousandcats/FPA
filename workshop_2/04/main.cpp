#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

using namespace sf;
using namespace std;

// general
constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr float BALL_SIZE = 40;
constexpr float DIAMETER = 2 * BALL_SIZE;
constexpr float HUNDRED = 100.f;

// speed
constexpr float MIN_SPEED = HUNDRED;
constexpr float MAX_SPEED = HUNDRED * 4.f;

// positions
constexpr Vector2f TOP_LEFT = {0, 0};
constexpr Vector2f TOP_RIGHT = {WINDOW_WIDTH - DIAMETER, 0};
constexpr Vector2f BOTTOM_LEFT = {0, WINDOW_HEIGHT - DIAMETER};
constexpr Vector2f BOTTOM_RIGHT = {WINDOW_WIDTH - DIAMETER, WINDOW_HEIGHT - DIAMETER};
constexpr Vector2f CENTER = {WINDOW_WIDTH / 2.f - BALL_SIZE, WINDOW_HEIGHT / 2.f - BALL_SIZE};

const vector<Color> COLOR_PALETTE = {
    Color::Red,
    Color::Green,
    Color::Blue,
    Color::Yellow,
    Color::Magenta,
    Color::Cyan,
    Color::White,
    Color::Black,
};

const vector<Vector2f> INITIAL_POSITIONS = {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CENTER
};

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

struct PRNG {
    mt19937 engine;
};

void initGenerator(PRNG &gen) {
    // Попытка использовать random_device
    random_device rd;
    unsigned seed = rd();
    //  time как fallback, если seed мал
    if (seed == 0 || seed == 0x80000000) {
        seed = static_cast<unsigned>(time(nullptr));
    }
    gen.engine.seed(seed);
}

// Генерация случайной компоненты скорости в диапазоне [minSpeed, maxSpeed]
// со случайным знаком
float randomSpeedComponent(
    PRNG &gen,
    const float minSpeed = MIN_SPEED,
    const float maxSpeed = MAX_SPEED
) {
    uniform_real_distribution magnitudeDist(minSpeed, maxSpeed);
    uniform_int_distribution signDist(0, 1);
    const float magnitude = magnitudeDist(gen.engine);
    return signDist(gen.engine) ? magnitude : -magnitude;
}

// возвращаем псевдослучайный вектор скорости
Vector2f randomSpeed(
    PRNG &gen,
    const float minSpeed = MIN_SPEED,
    const float maxSpeed = MAX_SPEED
) {
    return {
        randomSpeedComponent(gen, minSpeed, maxSpeed),
        randomSpeedComponent(gen, minSpeed, maxSpeed)
    };
}

Color getRandomColor(PRNG &gen) {
    // размерность выборки
    uniform_int_distribution<size_t> indexDist(0, COLOR_PALETTE.size() - 1);
    // берем цвета по случайным индексам
    const size_t i = indexDist(gen.engine);
    const size_t j = indexDist(gen.engine);
    const Color &firstColor = COLOR_PALETTE[i];
    const Color &secondColor = COLOR_PALETTE[j];

    // среднее арифметическое через лямбду
    auto avg = [](const uint8_t a, const uint8_t b) -> uint8_t {
        return static_cast<uint8_t>((a + b) / 2);
    };

    // rgba
    return {
        avg(firstColor.r, secondColor.r),
        avg(firstColor.g, secondColor.g),
        avg(firstColor.b, secondColor.b),
        avg(firstColor.a, secondColor.a)
    };
}

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

void handleCollision(Ball &ballA, Ball &ballB) {
    const Vector2f posA = ballA.base.getPosition() + Vector2f(ballA.radius, ballA.radius);
    const Vector2f posB = ballB.base.getPosition() + Vector2f(ballB.radius, ballB.radius);
    const Vector2f diff = posB - posA;
    const float distanceSquared = diff.x * diff.x + diff.y * diff.y;
    const float minDistance = ballA.radius + ballB.radius;

    if (distanceSquared >= minDistance * minDistance) {
        return;
    }

    // Защита от совпадающих центров
    if (distanceSquared < 1e-12f) {
        // ~1e-6 в линейной шкале
        constexpr Vector2f separation(1e-3f, 0.f);
        ballA.base.move(-separation);
        ballB.base.move(separation);
        return;
    }

    const Vector2f normal = diff / sqrt(distanceSquared);
    const Vector2f relativeSpeed = ballA.speed - ballB.speed; // относительная скорость
    const float speedAlongNormal = relativeSpeed.x * normal.x + relativeSpeed.y * normal.y;

    if (speedAlongNormal <= 0) {
        return;
    }

    // обмен скоростями при упругом столкновении (массы равны)
    ballA.speed -= speedAlongNormal * normal;
    ballB.speed += speedAlongNormal * normal;
}

void update(
    vector<Ball> &balls,
    Clock &clock
) {
    const float dt = clock.restart().asSeconds();
    for (Ball &ball: balls) {
        setNewPosition(ball, dt);
    }

    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            handleCollision(balls[i], balls[j]);
        }
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
        "Bouncing Balls With Pseudorandom Speed",
        Style::Default,
        State::Windowed,
        settings
    );
    Clock clock;

    PRNG generator;
    initGenerator(generator);

    vector<Ball> balls;
    balls.reserve(INITIAL_POSITIONS.size());

    for (const auto &pos: INITIAL_POSITIONS) {
        balls.emplace_back(
            getRandomColor(generator),
            pos,
            randomSpeed(generator)
        );
    }

    while (window.isOpen()) {
        pollEvents(window);
        update(balls, clock);
        render(window, balls);
    }
}
