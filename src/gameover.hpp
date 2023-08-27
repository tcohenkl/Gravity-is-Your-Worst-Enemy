#pragma once

#include <SFML/Graphics.hpp>

class GameOver {
private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text restartText;
    float fadeAlpha; 

public:
    GameOver();

    void draw(sf::RenderWindow& window);
    void setAlpha(sf::Uint8 alpha);
    void fadeIn(); 
    bool checkForRestart(const sf::Event& event);
    void resetAlpha();
};
