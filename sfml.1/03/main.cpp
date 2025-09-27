#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({500, 500}), "Convex Shapes");

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, {250, 100});
    triangle.setPoint(1, {200, 200});
    triangle.setPoint(2, {300, 200});
    triangle.setFillColor(sf::Color::Yellow);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(triangle);
        window.display();
    }

    return 0;
}