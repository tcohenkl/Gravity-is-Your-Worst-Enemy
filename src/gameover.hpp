#pragma once

#include <SFML/Graphics.hpp>
#include <sstream> 

class GameOver {

public:
    // Constructor initializes the game over screen properties.
    GameOver();

    // Setters:
    // Sets the survival time to display on the screen.
    void setSurvivalTime(float seconds);
    
    // Sets the distance traveled to display on the screen.
    void setDistance(float distance);
    
    // Renders the game over screen to the provided render window.
    void draw(sf::RenderWindow& window);
    
    // Sets the alpha (transparency) value for the game over texts.
    void setAlpha(sf::Uint8 alpha);
    
    // Gradually increases the transparency of the game over texts until fully visible.
    void fadeIn();
    
    // Checks if the player has pressed the 'R' key to restart the game.
    bool checkForRestart(const sf::Event& event);

    // Resets the alpha (transparency) value of the game over texts to be fully transparent.
    void resetAlpha();

private:
    // Font used for the game over screen texts.
    sf::Font font;

    // Texts to be displayed on the game over screen.
    sf::Text gameOverText;
    sf::Text restartText;
    sf::Text survivalText;
    sf::Text distanceText;

    // Current transparency value of the game over texts.
    float fadeAlpha;
};
