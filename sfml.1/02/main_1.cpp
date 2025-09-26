#include <SFML/Graphics.hpp>

sf::RectangleShape createTrafficLightBody() {
    sf::RectangleShape body({210, 80});
    body.setFillColor(sf::Color(128, 128, 128)); // Серый
    body.setPosition({100, 100});
    body.setRotation(sf::degrees(90));
    return body;
}

sf::CircleShape createSignal(float radius, const sf::Color& color, const sf::Vector2f& position) {
    sf::CircleShape signal(radius);
    signal.setFillColor(color);
    signal.setPosition(position);
    return signal;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 400}), "Traffic light");

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(createTrafficLightBody());
        window.draw(createSignal(30.f, sf::Color::Green, {30, 110}));
        window.draw(createSignal(30.f, sf::Color::Yellow, {30, 175}));
        window.draw(createSignal(30.f, sf::Color::Red, {30, 240}));
        window.display();
    }

    return 0;
}