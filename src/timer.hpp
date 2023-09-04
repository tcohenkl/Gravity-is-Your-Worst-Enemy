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

private:
    float gameplayDuration; // in seconds
    sf::Font font;
    sf::Text timerText;
};
