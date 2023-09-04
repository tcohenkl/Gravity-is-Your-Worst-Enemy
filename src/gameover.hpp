#pragma once

#include <SFML/Graphics.hpp>
#include <sstream> 

class GameOver {
private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text restartText;
    sf::Text survivalText; 
    sf::Text distanceText;
    float fadeAlpha; 

public:
    GameOver();

    void setSurvivalTime(float seconds);
    void setDistance(float distance);
    void draw(sf::RenderWindow& window);
    void setAlpha(sf::Uint8 alpha);
    void fadeIn(); 
    bool checkForRestart(const sf::Event& event);
    void resetAlpha();
};
