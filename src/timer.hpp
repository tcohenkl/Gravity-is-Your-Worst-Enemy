#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

class Timer {
public:
    Timer();

    // Resets the timer
    void reset();

    // Updates the timer's value
    void update(float elapsedSeconds);

    // Draws the timer to the window
    void draw(sf::RenderWindow& window);

    // To show and Hide the Timer at the top of the screen
    void show(); 
    void hide(); 

private:
    float gameplayDuration; // in seconds

    // Texts to be displayed on the game screen.
    sf::Font font;
    sf::Text timerText;
    bool displayTimer;
};
