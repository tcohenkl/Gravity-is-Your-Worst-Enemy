#include "rocket.hpp"
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(512, 512), "Rocket Game", sf::Style::Titlebar | sf::Style::Close);

    Rocket rocket;
    rocket.setPosition({256, 256});  // Center of the window for simplicity

    while (window.isOpen()) {
        sf::Event evnt;

        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
                window.close();
        }

        rocket.handleInput();
        rocket.update();

        window.clear();
        rocket.draw(window);
        window.display();
    }

}
