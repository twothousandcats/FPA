#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Several circles");

    window.clear();

    sf::CircleShape shape1(40.f);
    shape1.setFillColor(sf::Color::Red);
    shape1.setPosition({200, 160});

    sf::CircleShape shape2(48.f);
    shape2.setFillColor(sf::Color::Green);
    shape2.setPosition({260, 120});

    sf::CircleShape shape3(60.f);
    shape3.setFillColor(sf::Color::Yellow);
    shape3.setPosition({320, 160});

    sf::CircleShape shape4(75.f);
    shape4.setFillColor(sf::Color::Blue);
    shape4.setPosition({330, 220});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.draw(shape1);
        window.draw(shape2);
        window.draw(shape3);
        window.draw(shape4);
        window.display();
    }

    return 0;
}