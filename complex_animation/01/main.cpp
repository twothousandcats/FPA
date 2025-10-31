#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 1000;
constexpr unsigned WINDOW_HEIGHT = 800;
constexpr Vector2f WINDOW_CENTER = {1000 / 2.f, 800 / 2.f};
constexpr float BASE_SIDE = 50.f;
constexpr size_t BLOCKS_COUNT = 8;
constexpr size_t ANIMATION_STEPS = 5;
constexpr float ANIMATION_DURATION = 5.f;
constexpr float SPACING = 100.f;

struct Block {
    RectangleShape shape;
    Vector2f basePosition;
    Vector2f baseSize;
    Color baseColor;
    float phaseOffset{}; //async ani
};

// Утилита для линейной интерполяции
template<typename T>
T lerp(
    T a, T b,
    float t
) {
    return a + (b - a) * t;
}

Vector2f getHorizontalPosition(
    const size_t index,
    float centerY
) {
    constexpr float startX = (WINDOW_WIDTH - (BLOCKS_COUNT - 1) * SPACING) / 2.0f;
    return {
        startX + static_cast<float>(index) * SPACING,
        centerY
    };
}

Vector2f getVerticalPosition(
    const size_t index,
    float centerX
) {
    constexpr float startY = (WINDOW_HEIGHT - (BLOCKS_COUNT - 1) * SPACING) / 2.0f;
    return {
        centerX,
        startY + static_cast<float>(index) * SPACING
    };
}

void createBlock(
    vector<Block> &blocks
) {
    constexpr float startX = (WINDOW_WIDTH - (BLOCKS_COUNT - 1) * SPACING) / 2.0f;
    constexpr float startY = WINDOW_HEIGHT / 2.0f;

    for (size_t i = 0; i < BLOCKS_COUNT; ++i) {
        Block b;

        b.basePosition = {startX + i * SPACING, startY};
        b.shape.setPosition(b.basePosition);

        b.baseSize = {BASE_SIDE, BASE_SIDE};
        b.shape.setSize(b.baseSize);

        b.baseColor = Color::Green;
        b.phaseOffset = static_cast<float>(i) / BLOCKS_COUNT * ANIMATION_DURATION;

        blocks.push_back(b);
    }
}

void pollEvents(RenderWindow &window) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
    }
}

void update(
    vector<Block> &blocks,
    const Clock &clock
) {
    const float time = clock.getElapsedTime().asSeconds();
    const float cycleTime = fmod(time, ANIMATION_DURATION);
    const float normalized = cycleTime / ANIMATION_DURATION; // [0, 1)

    constexpr float stepDuration = 1.f / ANIMATION_STEPS;
    const auto step = static_cast<size_t>(normalized / stepDuration);
    const float alpha = (normalized - static_cast<float>(step) * stepDuration) / stepDuration; // [0, 1)

    // Исходная горизонтальная линия (центр Y)
    constexpr float centerY = WINDOW_CENTER.y;
    constexpr float topY = 150.f; // верхняя граница

    switch (step % ANIMATION_STEPS) {
        case 0: {
            // синхронный сдвиг влево
            for (size_t i = 0; i < blocks.size(); ++i) {
                const Vector2f startPos = getHorizontalPosition(i, centerY);
                const Vector2f endPos = startPos + Vector2f(-120.f, 0.f);
                const Vector2f pos = {
                    lerp(startPos.x, endPos.x, alpha),
                    lerp(startPos.y, endPos.y, alpha)
                };
                blocks[i].shape.setPosition(pos);
                blocks[i].shape.setSize(blocks[i].baseSize);
                blocks[i].shape.setFillColor(Color::Green);
            }
            break;
        }
        case 1: {
            // Асинхронное вертикальное центрирование относительно центра окна
            for (size_t i = 0; i < blocks.size(); ++i) {
                const Vector2f startPos = getHorizontalPosition(i, centerY) + Vector2f(-120.f, 0.f);
                const Vector2f endPos = getVerticalPosition(i, WINDOW_CENTER.x);
                const Vector2f pos = {
                    lerp(startPos.x, endPos.x, alpha),
                    lerp(startPos.y, endPos.y, alpha)
                };
                blocks[i].shape.setPosition(pos);
                blocks[i].shape.setSize(blocks[i].baseSize);
                blocks[i].shape.setFillColor(Color::Green);
            }
            break;
        }
        case 2: {
            // асинхронное движение вверх в горизонтальную линию
            for (size_t i = 0; i < blocks.size(); ++i) {
                const Vector2f startPos = getVerticalPosition(i, WINDOW_CENTER.x);
                const Vector2f endPos = getHorizontalPosition(i, topY);
                const Vector2f pos = {
                    lerp(startPos.x, endPos.x, alpha),
                    lerp(startPos.y, endPos.y, alpha)
                };
                blocks[i].shape.setPosition(pos);
                blocks[i].shape.setSize(blocks[i].baseSize);
                blocks[i].shape.setFillColor(Color::Green);
            }
            break;
        }
        case 3: {
            // синхронное движение в цетнр со сменой альфы
            const auto a = static_cast<uint8_t>(lerp(255.f, 80.f, alpha));
            const Color color(0, 255, 0, a);

            for (size_t i = 0; i < blocks.size(); ++i) {
                const Vector2f startPos = getHorizontalPosition(i, topY);
                const Vector2f endPos = getHorizontalPosition(i, centerY); // исходная позиция
                const Vector2f pos = {
                    lerp(startPos.x, endPos.x, alpha),
                    lerp(startPos.y, endPos.y, alpha)
                };
                blocks[i].shape.setPosition(pos);
                blocks[i].shape.setSize(blocks[i].baseSize);
                blocks[i].shape.setFillColor(color);
            }
            break;
        }
        case 4: {
            // синхронное изменение размера
            float sizeFactor;
            if (alpha < 0.333f) {
                sizeFactor = lerp(1.0f, 2.0f, alpha * 3);
            } else if (alpha < 0.666f) {
                sizeFactor = lerp(2.0f, 0.5f, (alpha - 0.333f) * 3);
            } else {
                sizeFactor = lerp(0.5f, 1.0f, (alpha - 0.666f) * 3);
            }

            const Vector2f size = {BASE_SIDE * sizeFactor, BASE_SIDE * sizeFactor};

            uint8_t r = 0;
            if (alpha < 0.5f) {
                r = static_cast<uint8_t>(lerp(0, 255, alpha * 2));
            } else {
                r = static_cast<uint8_t>(lerp(255, 0, (alpha - 0.5f) * 2));
            }
            const Color color(r, 255, 0, 255);

            for (size_t i = 0; i < blocks.size(); ++i) {
                Vector2f pos = getHorizontalPosition(i, centerY);
                blocks[i].shape.setPosition(pos);
                blocks[i].shape.setSize(size);
                blocks[i].shape.setFillColor(color);
            }
            break;
        }
    }

    // Асинхронный эффект: лёгкое дрожание
    for (auto &block: blocks) {
        float t = fmod(time + block.phaseOffset, 1.0f);
        float driftX = sin(t * 4 * M_PI) * 1.0f;
        float driftY = cos(t * 5 * M_PI) * 0.7f;
        Vector2f pos = block.shape.getPosition();
        block.shape.setPosition(pos + Vector2f(driftX, driftY));
    }
}

void render(
    RenderWindow &window,
    const vector<Block> &blocks
) {
    window.clear(Color::White);
    for (const auto &block: blocks) {
        window.draw(block.shape);
    }
    window.display();
}

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;

    RenderWindow window(
        VideoMode({
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
        }),
        "Complex animation",
        Style::Default,
        State::Windowed,
        settings
    );

    vector<Block> blocks;
    blocks.reserve(BLOCKS_COUNT);
    createBlock(blocks);

    Clock clock;

    while (window.isOpen()) {
        pollEvents(window);
        update(blocks, clock);
        render(window, blocks);
    }

    return 0;
}
