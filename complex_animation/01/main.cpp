#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

constexpr float BASE_SIDE = 50.f;
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
    Stage0_MoveRight,
    Stage1_MoveToWindowCenter,
    Stage2_MoveToHorizontal,
    Stage3_MoveTop,
    Stage4_MoveToVerticalStack,
    Stage5_MoveToInitialPosition,
    Finished
};

struct Block {
    RectangleShape shape;
    Vector2f basePosition;
    Vector2f baseSize;
    Color baseColor = DEFAULT_COLOR;
    float phaseOffset{};

    AnimationStage stage = AnimationStage::Stage0_MoveRight;
    float stageStartTime = 0.f;

    size_t index{};
    Vector2f stageStartPos; // начало этапа
    Vector2f stageStartSize;
};

// lerp
template<typename T>
T linearInterpolation(
    const T &a,
    const T &b,
    const float time
) {
    return a + (b - a) * time;
}

void createBlock(
    vector<Block> &blocks
) {
    for (size_t i = 0; i < BLOCKS_COUNT; ++i) {
        Block b;
        b.basePosition = {INITIAL_POSITION.x, INITIAL_POSITION.y + i * SPACING};
        b.baseSize = {BASE_SIDE, BASE_SIDE};
        b.phaseOffset = 0.f;

        b.shape.setPosition(b.basePosition);
        b.shape.setSize(b.baseSize);
        b.shape.setFillColor(b.baseColor);
        b.shape.setOrigin(b.baseSize / 2.f);

        b.index = i;
        b.stageStartPos = b.basePosition;
        b.stageStartSize = b.baseSize;

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
    const float duration
) {
    block.shape.setPosition(
        linearInterpolation(
            block.stageStartPos,
            endPosition,
            duration
        )
    );
}

void updateSize(
    Block &block,
    const Vector2f &startSize,
    const Vector2f &endSize,
    const float t
) {
    block.shape.setSize(
        linearInterpolation(
            startSize,
            endSize,
            t
        )
    );
}

void updateAlpha(
    Block &block,
    const uint8_t &startA,
    const uint8_t &endA,
    const float duration
) {
    const auto currentA = static_cast<uint8_t>(
        startA + duration * (static_cast<float>(endA) - static_cast<float>(startA))
    );

    Color currentColor = block.baseColor;
    currentColor.a = currentA;
    block.shape.setFillColor(currentColor);
}

void toNextStage(
    Block &block,
    const AnimationStage nextStage,
    const float totalTime
) {
    block.stage = nextStage;
    block.stageStartTime = totalTime;
    block.stageStartPos = block.shape.getPosition();
    block.stageStartSize = block.shape.getSize();
}

void toFinishStage(
    Block &block
) {
    block.stage = AnimationStage::Finished;
}

void animateStage0_MoveRight(
    Block &block,
    const float totalTime
) {
    constexpr float RIGHT_SHIFT = 200.f;
    const float time = getNormalizedTime(block, totalTime);
    const Vector2f endPos = {block.basePosition.x + RIGHT_SHIFT, block.basePosition.y};
    updatePosition(block, endPos, time);

    if (time >= ANIMATION_DURATION) {
        toNextStage(block, AnimationStage::Stage1_MoveToWindowCenter, totalTime);
    }
}

void animateStage1_MoveToWindowCenter(
    Block &block,
    const float totalTime
) {
    constexpr int COLOR_COMPENSATION_FACTOR = 2;
    const float time = getNormalizedTime(block, totalTime);

    // Pos
    const float totalHeight = (BLOCKS_COUNT - 1) * SPACING;
    const float startY = WINDOW_CENTER.y - totalHeight * HALF_COMPENSATION_FACTOR;
    const float targetY = startY + static_cast<float>(block.index) * SPACING;
    const Vector2f endPos = {WINDOW_CENTER.x, targetY};
    updatePosition(block, endPos, time);
    updateAlpha(block, block.baseColor.a, block.baseColor.a / COLOR_COMPENSATION_FACTOR, time);

    if (time >= ANIMATION_DURATION) {
        toNextStage(block, AnimationStage::Stage2_MoveToHorizontal, totalTime);
    }
}

void animateStage2_MoveToHorizontal(
    Block &block,
    const float totalTime
) {
    const float time = getNormalizedTime(block, totalTime);

    const float totalWidth = (BLOCKS_COUNT - 1) * SPACING;
    const float startX = WINDOW_CENTER.x - totalWidth / 2.f;
    const float targetX = startX + static_cast<float>(block.index) * SPACING;
    const Vector2f endPos = {targetX, WINDOW_CENTER.y};

    updatePosition(block, endPos, time);


    if (time >= ANIMATION_DURATION) {
        toNextStage(block, AnimationStage::Stage3_MoveTop, totalTime);
    }
}

void animateStage3_MoveTop(
    Block &block,
    const float totalTime
) {
    constexpr float VERTICAL_SHIFT = 200.f;
    const float time = getNormalizedTime(block, totalTime);

    const Vector2f endPos = {block.stageStartPos.x, block.stageStartPos.y - VERTICAL_SHIFT};
    updatePosition(block, endPos, time);

    // режем высоту в 2
    const Vector2f startSize = block.stageStartSize;
    const Vector2f endSize = {block.baseSize.x, block.baseSize.y * HALF_COMPENSATION_FACTOR};
    updateSize(block, startSize, endSize, time);

    if (time >= ANIMATION_DURATION) {
        toNextStage(block, AnimationStage::Stage4_MoveToVerticalStack, totalTime);
    }
}

void animateStage4_MoveToVerticalStack(
    Block &block,
    const float totalTime
) {
    const float time = getNormalizedTime(block, totalTime);

    constexpr float newHeight = BASE_SIDE * HALF_COMPENSATION_FACTOR;
    constexpr float spacingY = newHeight + SPACING;

    constexpr float totalWidth = (BLOCKS_COUNT - 1) * SPACING;
    constexpr float firstBlockX = WINDOW_CENTER.x - totalWidth * HALF_COMPENSATION_FACTOR;

    const float startY = block.stageStartPos.y;
    const float targetY = startY + static_cast<float>(block.index) * spacingY;
    const Vector2f endPos = {firstBlockX, targetY};
    updatePosition(block, endPos, time);

    if (time >= ANIMATION_DURATION) {
        toNextStage(block, AnimationStage::Stage5_MoveToInitialPosition, totalTime);
    }
}

void animateStage5_MoveToInitialPosition(
    Block &block,
    const float totalTime
) {
    const float time = getNormalizedTime(block, totalTime);

    const Vector2f endPos = block.basePosition;
    updatePosition(block, endPos, time);

    const Vector2f startSize = block.stageStartSize;
    const Vector2f endSize = block.baseSize;
    updateSize(block, startSize, endSize, time);

    updateAlpha(block, block.baseColor.a / 2, block.baseColor.a, time);

    if (time >= ANIMATION_DURATION) {
        toFinishStage(block);
    }
}

void resetAnimation(
    Block &block,
    const float totalTime
) {
    block.shape.setPosition(block.basePosition);
    block.shape.setSize(block.baseSize);
    block.shape.setFillColor(block.baseColor);

    block.stage = AnimationStage::Stage0_MoveRight;
    block.stageStartPos = block.basePosition;
    block.stageStartSize = block.baseSize;
    block.stageStartTime = totalTime;
    block.phaseOffset = totalTime;
}

void update(
    vector<Block> &blocks,
    const Clock &clock
) {
    const float totalTime = clock.getElapsedTime().asSeconds();

    for (auto &block: blocks) {
        // сценарий
        switch (block.stage) {
            case AnimationStage::Stage0_MoveRight: {
                animateStage0_MoveRight(block, totalTime);
                break;
            }
            case AnimationStage::Stage1_MoveToWindowCenter: {
                animateStage1_MoveToWindowCenter(block, totalTime);
                break;
            }
            case AnimationStage::Stage2_MoveToHorizontal: {
                animateStage2_MoveToHorizontal(block, totalTime);
                break;
            }
            case AnimationStage::Stage3_MoveTop: {
                animateStage3_MoveTop(block, totalTime);
                break;
            }
            case AnimationStage::Stage4_MoveToVerticalStack: {
                animateStage4_MoveToVerticalStack(block, totalTime);
                break;
            }
            case AnimationStage::Stage5_MoveToInitialPosition: {
                animateStage5_MoveToInitialPosition(block, totalTime);
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
