#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Convex Shapes");

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, {250, 100});
    triangle.setPoint(1, {200, 200});
    triangle.setPoint(2, {300, 200});
    triangle.setFillColor(sf::Color::Yellow);

    sf::ConvexShape trapeze;
    trapeze.setPointCount(4);
    trapeze.setPosition({400, 300});
    trapeze.setPoint(0, {-90, 0});
    trapeze.setPoint(1, {+90, 0});
    trapeze.setPoint(2, {+120, 60});
    trapeze.setPoint(3, {-120, 60});
    trapeze.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(triangle);
        window.draw(trapeze);
        window.display();
    }

    return 0;
}