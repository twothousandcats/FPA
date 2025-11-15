#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

constexpr float BASE_SIDE = 50.f;
constexpr Vector2f BASE_SIZE = {BASE_SIDE, BASE_SIDE};
constexpr unsigned WINDOW_WIDTH = 1000;
constexpr unsigned WINDOW_HEIGHT = 800;
constexpr Vector2f WINDOW_CENTER = {1000 / 2.f, 800 / 2.f};
constexpr Vector2f INITIAL_POSITION = {BASE_SIDE / 2.f, BASE_SIDE / 2.f};
constexpr size_t BLOCKS_COUNT = 6;
constexpr size_t ANIMATION_STEPS = 5;
constexpr float ANIMATION_DURATION = 1.f;
constexpr float SPACING = 80.f;
constexpr float HALF_COMPENSATION_FACTOR = 0.5f;

constexpr Color DEFAULT_COLOR = {102, 0, 102, 255};

enum class AnimationStage {
    MoveRight,
    MoveToWindowCenter,
    MoveToHorizontal,
    MoveTop,
    MoveToVerticalStack,
    MoveToInitialPosition,
    Finished
};

struct Block {
    RectangleShape shape;
    Color baseColor = DEFAULT_COLOR;

    AnimationStage stage = AnimationStage::MoveRight;
    float stageStartTime = 0.f;

    size_t index{};
    Vector2f stageStartPos; // начало этапа
    Vector2f stageStartSize;
};

// lerp
Vector2f linearInterpolation(
    const Vector2f &a,
    const Vector2f &b,
    const float time
) {
    return a + (b - a) * time;
}

Vector2f getInitialPosition(
    const size_t index
) {
    return {INITIAL_POSITION.x, INITIAL_POSITION.y + static_cast<float>(index) * SPACING};
}

void createBlock(
    vector<Block> &blocks
) {
    for (size_t i = 0; i < BLOCKS_COUNT; ++i) {
        Block b;

        b.shape.setPosition(getInitialPosition(i));
        b.shape.setSize(BASE_SIZE);
        b.shape.setFillColor(b.baseColor);
        b.shape.setOrigin(BASE_SIZE / 2.f);

        b.index = i;
        b.stageStartPos = getInitialPosition(i);
        b.stageStartSize = BASE_SIZE;

        blocks.push_back(b);
    }
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

float getNormalizedTime(
    const Block &block,
    const float capacityTime,
    const float stageDuration = ANIMATION_DURATION
) {
    const float elapsedTime = capacityTime - block.stageStartTime;
    return min(elapsedTime / stageDuration, 1.f);
}

void updatePosition(
    Block &block,
    const Vector2f &endPosition,
    const float normalizedTime
) {
    block.shape.setPosition(
        linearInterpolation(
            block.stageStartPos,
            endPosition,
            normalizedTime
        )
    );
}

void updateSize(
    Block &block,
    const Vector2f &startSize,
    const Vector2f &endSize,
    const float normalizedTime
) {
    block.shape.setSize(
        linearInterpolation(
            startSize,
            endSize,
            normalizedTime
        )
    );
}

void updateAlpha(
    Block &block,
    const uint8_t &startA,
    const uint8_t &endA,
    const float normalizedTime
) {
    const auto currentA = static_cast<uint8_t>(
        startA + normalizedTime * (static_cast<float>(endA) - static_cast<float>(startA))
    );

    Color currentColor = block.baseColor;
    currentColor.a = currentA;
    block.shape.setFillColor(currentColor);
}

void toNextStage(
    Block &block,
    const AnimationStage nextStage,
    const float totalTime,
    const float normalizedTime
) {
    if (normalizedTime >= ANIMATION_DURATION) {
        block.stage = nextStage;
        block.stageStartTime = totalTime;
        block.stageStartPos = block.shape.getPosition();
        block.stageStartSize = block.shape.getSize();
    }
}

// Позиции
Vector2f computeMoveRightTarget(
    const Block &block,
    const float shift = 200.f
) {
    Vector2f initial = getInitialPosition(block.index);
    return {initial.x + shift, initial.y};
}

Vector2f computeGatherAtCenterTarget(
    const Block &block
) {
    const float totalHeight = (BLOCKS_COUNT - 1) * SPACING;
    const float startY = WINDOW_CENTER.y - totalHeight * HALF_COMPENSATION_FACTOR;
    float targetY = startY + static_cast<float>(block.index) * SPACING;
    return {WINDOW_CENTER.x, targetY};
}

Vector2f computeSpreadHorizontalTarget(
    const Block &block
) {
    const float totalWidth = (BLOCKS_COUNT - 1) * SPACING;
    const float startX = WINDOW_CENTER.x - totalWidth / 2.f;
    float targetX = startX + static_cast<float>(block.index) * SPACING;
    return {targetX, WINDOW_CENTER.y};
}

Vector2f computeLiftUpTarget(
    const Block &block,
    const float shift = 200.f
) {
    return {block.stageStartPos.x, block.stageStartPos.y - shift};
}

Vector2f computeVerticalStackTarget(
    const Block &block
) {
    constexpr float newHeight = BASE_SIDE * HALF_COMPENSATION_FACTOR;
    constexpr float spacingY = newHeight + SPACING;
    constexpr float totalWidth = (BLOCKS_COUNT - 1) * SPACING;
    constexpr float firstBlockX = WINDOW_CENTER.x - totalWidth * HALF_COMPENSATION_FACTOR;
    float targetY = block.stageStartPos.y + static_cast<float>(block.index) * spacingY;
    return {firstBlockX, targetY};
}

Vector2f computeReturnToInitialTarget(
    const Block &block
) {
    return getInitialPosition(block.index);
}

// размер
Vector2f computeLiftUpTargetSize() {
    return {BASE_SIZE.x, BASE_SIZE.y * HALF_COMPENSATION_FACTOR};
}

Vector2f computeReturnToInitialSize() {
    return BASE_SIZE;
}

// альфа
uint8_t computeDimmedAlpha() {
    return DEFAULT_COLOR.a / 2;
}

uint8_t computeFullAlpha() {
    return DEFAULT_COLOR.a;
}

void resetAnimation(
    Block &block,
    const float totalTime
) {
    const Vector2f position = getInitialPosition(block.index);
    block.shape.setPosition(position);
    block.shape.setSize(BASE_SIZE);
    block.shape.setFillColor(block.baseColor);

    block.stage = AnimationStage::MoveRight;
    block.stageStartPos = position;
    block.stageStartSize = BASE_SIZE;
    block.stageStartTime = totalTime;
}

void update(
    vector<Block> &blocks,
    const Clock &clock
) {
    const float totalTime = clock.getElapsedTime().asSeconds();

    for (auto &block: blocks) {
        // сценарий
        switch (block.stage) {
            case AnimationStage::MoveRight: {
                const float t = getNormalizedTime(block, totalTime);
                Vector2f endPos = computeMoveRightTarget(block);
                updatePosition(block, endPos, t);
                toNextStage(block, AnimationStage::MoveToWindowCenter, totalTime, t);
                break;
            }
            case AnimationStage::MoveToWindowCenter: {
                const float t = getNormalizedTime(block, totalTime);
                Vector2f endPos = computeGatherAtCenterTarget(block);
                uint8_t endA = computeDimmedAlpha();
                updatePosition(block, endPos, t);
                updateAlpha(block, block.baseColor.a, endA, t);
                toNextStage(block, AnimationStage::MoveToHorizontal, totalTime, t);
                break;
            }
            case AnimationStage::MoveToHorizontal: {
                const float t = getNormalizedTime(block, totalTime);
                Vector2f endPos = computeSpreadHorizontalTarget(block);
                updatePosition(block, endPos, t);
                toNextStage(block, AnimationStage::MoveTop, totalTime, t);
                break;
            }
            case AnimationStage::MoveTop: {
                const float t = getNormalizedTime(block, totalTime);
                Vector2f endPos = computeLiftUpTarget(block);
                Vector2f endSize = computeLiftUpTargetSize();
                updatePosition(block, endPos, t);
                updateSize(block, block.stageStartSize, endSize, t);
                toNextStage(block, AnimationStage::MoveToVerticalStack, totalTime, t);
                break;
            }
            case AnimationStage::MoveToVerticalStack: {
                const float t = getNormalizedTime(block, totalTime);
                Vector2f endPos = computeVerticalStackTarget(block);
                updatePosition(block, endPos, t);
                toNextStage(block, AnimationStage::MoveToInitialPosition, totalTime, t);
                break;
            }
            case AnimationStage::MoveToInitialPosition: {
                const float t = getNormalizedTime(block, totalTime);
                Vector2f endPos = computeReturnToInitialTarget(block);
                Vector2f endSize = computeReturnToInitialSize();
                uint8_t endA = computeFullAlpha();
                updatePosition(block, endPos, t);
                updateSize(block, block.stageStartSize, endSize, t);
                updateAlpha(block, block.baseColor.a / 2, endA, t);
                toNextStage(block, AnimationStage::Finished, totalTime, t);
                break;
            }
            case AnimationStage::Finished: {
                resetAnimation(block, totalTime);
                break;
            }
        }
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
