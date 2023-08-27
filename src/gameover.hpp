#pragma once

#include <SFML/Graphics.hpp>

class GameOver {
private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text restartText;

public:
    GameOver();

    void draw(sf::RenderWindow& window);
    bool checkForRestart(const sf::Event& event);
};
