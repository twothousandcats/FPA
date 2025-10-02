#include <SFML/Graphics.hpp>

sf::RectangleShape createRectangle(
    const sf::Vector2f& size,
    const sf::Vector2f& position,
    const sf::Color& color // rgb
) {
    sf::RectangleShape rect(size);
    rect.setFillColor(sf::Color(color));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2.f);
    rect.setPosition(position);
    rect.setRotation(sf::degrees(0.f));
    return rect;
}

sf::CircleShape createCircle(
	float radius,
	const sf::Color& color,
	const sf::Vector2f& position
) {
    sf::CircleShape signal(radius);
    signal.setFillColor(color);
    signal.setPosition(position);
    return signal;
}

sf::ConvexShape createRoof(
    float topWidth,
    float bottomWidth,
    float height,
    const sf::Vector2f& position,
    const sf::Color& color
) {
    sf::ConvexShape trapeze;
    trapeze.setPointCount(4);

    float topHalf = topWidth / 2.0f;
    float bottomHalf = bottomWidth / 2.0f;

    trapeze.setPosition(position);

    trapeze.setPoint(0, sf::Vector2f(-topHalf, 0));          // верхний левый
    trapeze.setPoint(1, sf::Vector2f(+topHalf, 0));          // верхний правый
    trapeze.setPoint(2, sf::Vector2f(+bottomHalf, height));  // нижний правый
    trapeze.setPoint(3, sf::Vector2f(-bottomHalf, height));  // нижний левый
    trapeze.setOutlineColor(sf::Color::White);
    trapeze.setOutlineThickness(2.f);

    trapeze.setFillColor(sf::Color(color));
    return trapeze;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 800}), "House");

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        // wall
        window.draw(createRectangle(
            {500, 250},        // размер: ширина, высота
            {300, 400},        // позиция (левый верхний угол)
            {77, 47, 10}       // без поворота
        ));
        // door
        window.draw(createRectangle(
            {70, 140},
            {340, 510},
            {0, 0, 0}
        ));
        // roof
        window.draw(createRoof(
            200.f,             // ширина верха крыши
            600.f,             // ширина низа крыши
            100.f,             // высота крыши
            {550, 300},        // позиция
            {93, 30, 22}       // цвет крыши
        ));
        // pipe
        window.draw(createRectangle(
            {30, 80},          // размер: ширина, высота
            {600, 280},        // позиция (левый верхний угол)
            {60, 56, 56}
        ));
        window.draw(createRectangle(
            {50, 40},          // размер: ширина, высота
            {590, 240},        // позиция (левый верхний угол)
            {60, 56, 56}
        ));
        // smoke
        window.draw(createCircle(
            20.f,                          // радиус
            {191, 191, 191},               // цвет
            {610, 200}                     // позиция центра
        ));
        window.draw(createCircle(
            25.f,                          // радиус
            {191, 191, 191},               // цвет
            {620, 180}                     // позиция центра
        ));
        window.draw(createCircle(
            30.f,                          // радиус
            {191, 191, 191},               // цвет
            {640, 160}                     // позиция центра
        ));
        window.draw(createCircle(
            35.f,                          // радиус
            {191, 191, 191},               // цвет
            {650, 140}                     // позиция центра
        ));
		// window
        window.draw(createRectangle(
            {80, 80},        // размер: ширина, высота
            {550, 520},      // позиция (левый верхний угол)
            {42, 122, 226}
        ));
        window.display();
    }

    return 0;
}