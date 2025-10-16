#include <SFML/Graphics.hpp>

using namespace sf;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

void pollEvents(
    RenderWindow &window
) {
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
    }
}

void render(
    RenderWindow &window,
    const Sprite &cat
) {
    window.clear();
    window.draw(cat);
    window.display();
}

int main() {
    RenderWindow window(
        VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Cat Sprite"
    );
    const std::string SPRITE_NAME = "cat.png";

    // Load a sprite to display
    const Texture texture(SPRITE_NAME);
    const Sprite cat(texture);

    while (window.isOpen()) {
        pollEvents(window);
        render(window, cat);
    }

    return 0;
}
