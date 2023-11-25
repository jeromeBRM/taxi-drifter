// Libraries
#include <reactphysics3d/reactphysics3d.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cassert>

int main() {
    sf::RenderWindow sfmlWin(sf::VideoMode(600, 360), "TaxiDrifter SFML debugger");
    sf::Font font;

    if (!font.loadFromFile("myfont.ttf")) {
        return -1;
    }
    sf::Text message(L"\u6f22\u5b57", font);

    sf::RectangleShape rectangle(sf::Vector2f(120, 50));

    int pos = 0;

    while (sfmlWin.isOpen()) {

        sf::Event e;
        while (sfmlWin.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            case sf::Event::MouseMoved:
                rectangle.setPosition(e.mouseMove.x, e.mouseMove.y);
                break;
            }

        }

        sfmlWin.clear();
        sfmlWin.draw(message);
        sfmlWin.draw(rectangle);
        sfmlWin.display();
    }
    return 0;
}