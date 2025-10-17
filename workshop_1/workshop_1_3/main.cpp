#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

void initCat(
    Sprite &cat,
    const Texture &texture
) {
    cat.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });

    const Vector2u textureSize = texture.getSize();
    cat.setOrigin({ textureSize.x / 2.f, textureSize.y / 2.f });
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

void render(
    RenderWindow &window,
    const Sprite &cat
) {
    window.clear(Color::White);
    window.draw(cat);
    window.display();
}

int main() {
    const string SPRITE_NAME = "cat.png";

    try {
        const Texture texture(SPRITE_NAME);
        Sprite cat(texture);
        initCat(cat, texture);

        RenderWindow window(
            VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
            "Cat Sprite"
        );

        while (window.isOpen()) {
            pollEvents(window);
            render(window, cat);
        }
        
    } catch (const sf::Exception& error) {
        cerr << "SFML Error: " << error.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
