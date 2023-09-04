#pragma once
#include <SFML/Graphics.hpp>

class DistanceMeter {
private:
    float totalDistance;  // Store the total distance traveled
    sf::Vector2f initialPosition;  // Store the initial starting position
    sf::Font font;
    sf::Text distanceText;

public:
    // Constructor
    DistanceMeter();

    // Reset the distance meter and set the initial position of the rocket
    void reset(const sf::Vector2f& startPosition);

    // Update the total distance based on the rocket's current position
    void update(const sf::Vector2f& currentPosition);

    // Get the total distance traveled
    float getTotalDistance() const;

    void draw(sf::RenderWindow& window);

};
