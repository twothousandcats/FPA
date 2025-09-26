#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 400}), "Rectangles");

    sf::RectangleShape rect1({100, 50});
    rect1.setFillColor(sf::Color::Red);
    rect1.setPosition({100, 100});
    rect1.setRotation(sf::degrees(30)); // в градусах или радианах v3.x.x

    sf::RectangleShape rect2({80, 40});
    rect2.setFillColor(sf::Color::Blue);
    rect2.setPosition({300, 200});
    rect2.setOrigin({40, 20});
    rect2.setRotation(sf::degrees(45));

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(rect1);
        window.draw(rect2);
        window.display();
    }

    return 0;
}