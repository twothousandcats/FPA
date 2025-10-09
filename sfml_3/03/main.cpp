#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;
using namespace sf;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

struct Arrow {
    ConvexShape head;
    RectangleShape stem;
    Vector2f position;
    float rotation = 0;
};

Vector2f toEuclidian(const float radius, const float angle) {
    return {
        radius * cos(angle),
        radius * sin(angle)
    };
}

float toDegrees(const float radians) {
    return static_cast<float>(static_cast<double>(radians) * 180 / M_PI);
}

void updateArrowElements(Arrow &arrow) {
    const Vector2f headOffset = toEuclidian(40.f, arrow.rotation);
    arrow.head.setPosition(arrow.position + headOffset);
    arrow.head.setRotation(degrees(toDegrees(arrow.rotation))); // Angle

    arrow.stem.setPosition(arrow.position);
    arrow.stem.setRotation(degrees(toDegrees(arrow.rotation))); // Angle
}

void initArrow(Arrow &arrow) {
    arrow.position = Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    arrow.head.setPointCount(3);
    arrow.head.setPoint(0, {30, 0});
    arrow.head.setPoint(1, {0, -20});
    arrow.head.setPoint(2, {0, 20});
    arrow.head.setFillColor(Color({0xFF, 0, 0}));

    arrow.stem.setSize({80, 20});
    arrow.stem.setOrigin({40, 10});
    arrow.stem.setFillColor(Color{0xF0, 0xA0, 0x00});

    updateArrowElements(arrow);
}

void onMouseMove(const Event::MouseMoved &evt, Vector2f &mousePosition) {
    mousePosition = Vector2f(evt.position);
}

void pollEvents(RenderWindow &window, Vector2f &mousePosition) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
        if (const auto mouseButtonMoved = event->getIf<Event::MouseMoved>()) {
            onMouseMove(*mouseButtonMoved, mousePosition);
        }
    }
}

void update(const Vector2f &mousePosition, Arrow &arrow) {
    const Vector2f delta = mousePosition - arrow.position;
    arrow.rotation = atan2(delta.y, delta.x);
    updateArrowElements(arrow);
}

void redraw(RenderWindow &window, const Arrow &arrow) {
    window.clear();
    window.draw(arrow.head);
    window.draw(arrow.stem);
    window.display();
}

int main() {
    ContextSettings settings;
    settings.antiAliasingLevel = 8;
    RenderWindow window(
        VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Arrow follows mouse",
        Style::Default,
        State::Windowed,
        settings
    );

    Arrow arrow;
    Vector2f mousePosition;

    initArrow(arrow);
    while (window.isOpen()) {
        pollEvents(window, mousePosition);
        update(mousePosition, arrow);
        redraw(window, arrow);
    }
}
