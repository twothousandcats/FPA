#include <SFML/Graphics.hpp>

sf::RectangleShape createWhiteRectangle(
    const sf::Vector2f& size,
    const sf::Vector2f& position,
    float rotationDegrees = 0.f
) {
    sf::RectangleShape rect(size);
    rect.setFillColor(sf::Color::White);
    rect.setPosition(position);
    rect.setRotation(sf::degrees(rotationDegrees));
    return rect;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({300, 300}), "MVP");

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();

        // M
        window.draw(createWhiteRectangle({10, 100}, {50, 100}));
        window.draw(createWhiteRectangle({10, 40}, {50, 106}, -45));
        window.draw(createWhiteRectangle({10, 38}, {100, 101}, 45));
        window.draw(createWhiteRectangle({10, 100}, {100, 100}));

        // V
        window.draw(createWhiteRectangle({10, 100}, {120, 100}, -14));
        window.draw(createWhiteRectangle({10, 100}, {170, 98}, 14));

        // P
        window.draw(createWhiteRectangle({10, 90}, {190, 110}));
        window.draw(createWhiteRectangle({40, 10}, {240, 110}, -180));
        window.draw(createWhiteRectangle({40, 10}, {240, 160}, 180));
        window.draw(createWhiteRectangle({10, 40}, {240, 110}));

        window.display();
    }

    return 0;
}