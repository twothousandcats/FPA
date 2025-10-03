#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

void redrawWindow(RenderWindow &window) {
    window.clear();
    window.display();
}

int main() {
    RenderWindow window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                        "Mouse Events To Terminal");
    while (window.isOpen()) {
        while (auto const event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (const auto mouseButtonPressed = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == Mouse::Button::Left) {
                    cout << "the left button was pressed" << endl;
                    cout << "mouse x: " << mouseButtonPressed->position.x << endl;
                    cout << "mouse y: " << mouseButtonPressed->position.y << endl;
                } else if (mouseButtonPressed->button == Mouse::Button::Right) {
                    cout << "the right button was pressed" << endl;
                    cout << "mouse x: " << mouseButtonPressed->position.x << endl;
                    cout << "mouse y: " << mouseButtonPressed->position.y << endl;
                }
            }
        }

        redrawWindow(window);
    }
}
