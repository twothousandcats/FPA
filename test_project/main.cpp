// Обход ошибки совместимости SFML 2.5 с Clang на macOS
#include <string>

namespace std {
    template<>
    struct char_traits<unsigned int> {
        using char_type = unsigned int;
        using int_type = unsigned long;
        using off_type = long;
        using pos_type = unsigned long;
        using state_type = mbstate_t;

        static void assign(char_type& __c1, const char_type& __c2) noexcept { __c1 = __c2; }
        static bool eq(const char_type& __c1, const char_type& __c2) noexcept { return __c1 == __c2; }
        static bool lt(const char_type& __c1, const char_type& __c2) noexcept { return __c1 < __c2; }
        static int compare(const char_type* __s1, const char_type* __s2, size_t __n) {
            for (size_t i = 0; i < __n; ++i) {
                if (__s1[i] < __s2[i]) return -1;
                if (__s1[i] > __s2[i]) return 1;
            }
            return 0;
        }
        static size_t length(const char_type* __s) {
            size_t i = 0;
            while (__s[i] != 0) ++i;
            return i;
        }
        static const char_type* find(const char_type* __s, size_t __n, const char_type& __a) {
            for (size_t i = 0; i < __n; ++i)
                if (__s[i] == __a) return __s + i;
            return nullptr;
        }
        static char_type* move(char_type* __s1, const char_type* __s2, size_t __n) {
            if (__s1 == __s2) return __s1;
            if (__s1 < __s2) {
                for (size_t i = 0; i < __n; ++i) __s1[i] = __s2[i];
            } else {
                for (size_t i = __n; i > 0; --i) __s1[i-1] = __s2[i-1];
            }
            return __s1;
        }
        static char_type* copy(char_type* __s1, const char_type* __s2, size_t __n) {
            for (size_t i = 0; i < __n; ++i) __s1[i] = __s2[i];
            return __s1;
        }
        static char_type* assign(char_type* __s, size_t __n, char_type __a) {
            for (size_t i = 0; i < __n; ++i) __s[i] = __a;
            return __s;
        }
        static constexpr bool eq_int_type(const int_type& __c1, const int_type& __c2) noexcept {
            return __c1 == __c2;
        }
        static constexpr int_type eof() noexcept { return static_cast<int_type>(-1); }
        static constexpr int_type not_eof(const int_type& __c) noexcept {
            return (__c == eof()) ? 0 : __c;
        }
        static constexpr char_type to_char_type(const int_type& __c) noexcept {
            return static_cast<char_type>(__c);
        }
        static constexpr int_type to_int_type(const char_type& __c) noexcept {
            return static_cast<int_type>(__c);
        }
    };
}
#include <SFML/Graphics.hpp>

sf::RectangleShape createRectangle(
    const sf::Vector2f& size,
    const sf::Vector2f& position,
    const sf::Color& color // rgb
) {
    sf::RectangleShape rect(size);
    rect.setFillColor(color);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2.f);
    rect.setPosition(position);
    rect.setRotation(0.f);
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

    trapeze.setPoint(0, sf::Vector2f(-topHalf, 0));          // верхний левый
    trapeze.setPoint(1, sf::Vector2f(+topHalf, 0));          // верхний правый
    trapeze.setPoint(2, sf::Vector2f(+bottomHalf, height));  // нижний правый
    trapeze.setPoint(3, sf::Vector2f(-bottomHalf, height));  // нижний левый

    trapeze.setOutlineColor(sf::Color::White);
    trapeze.setOutlineThickness(2.f);
    trapeze.setFillColor(color);
    trapeze.setPosition(position);
    return trapeze;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "House");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        // wall
        window.draw(createRectangle(
            sf::Vector2f(500, 250),
            sf::Vector2f(300, 400),
            sf::Color(77, 47, 10)
        ));
        // door
        window.draw(createRectangle(
            sf::Vector2f(70, 140),
            sf::Vector2f(340, 510),
            sf::Color::Black
        ));
        // pipe
        window.draw(createRectangle(
            sf::Vector2f(30, 80),
            sf::Vector2f(600, 280),
            sf::Color(60, 56, 56)
        ));
        window.draw(createRectangle(
            sf::Vector2f(50, 40),
            sf::Vector2f(590, 240),
            sf::Color(60, 56, 56)
        ));
        // roof
        window.draw(createRoof(
            200.f,
            600.f,
            100.f,
            sf::Vector2f(550, 300),
            sf::Color(93, 30, 22)
        ));
        // smoke
        window.draw(createCircle(20.f, sf::Color(191, 191, 191), sf::Vector2f(610, 200)));
        window.draw(createCircle(25.f, sf::Color(191, 191, 191), sf::Vector2f(620, 180)));
        window.draw(createCircle(30.f, sf::Color(191, 191, 191), sf::Vector2f(640, 160)));
        window.draw(createCircle(35.f, sf::Color(191, 191, 191), sf::Vector2f(650, 140)));
        // window
        window.draw(createRectangle(
            sf::Vector2f(80, 80),
            sf::Vector2f(550, 520),
            sf::Color(42, 122, 226)
        ));
        window.display();
    }

    return 0;
}