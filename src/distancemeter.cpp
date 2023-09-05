#include "distancemeter.hpp"
#include <SFML/Graphics.hpp>
#include <cmath> 
#include <iostream> 
#include <iomanip>
#include <sstream>


DistanceMeter::DistanceMeter() : totalDistance(0.0f), displayMeter(false)
{
    if (!font.loadFromFile("assets/fonts/Orbitron-Bold.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    distanceText.setFont(font);
    distanceText.setCharacterSize(12); // choose appropriate size
    distanceText.setFillColor(sf::Color::White);
}

// Reset the distance meter and set the initial position of the rocket
void DistanceMeter::reset(const sf::Vector2f& startPosition) {
    totalDistance = 0.0f;
    initialPosition = startPosition;
}

// Update the total distance based on the rocket's current position
void DistanceMeter::update(const sf::Vector2f& currentPosition) {
    float dx = currentPosition.x - initialPosition.x;
    float dy = currentPosition.y - initialPosition.y;

    // Add the distance traveled in this frame to the total distance
    totalDistance += std::sqrt(dx * dx + dy * dy);

    // Update the initial position for the next frame
    initialPosition = currentPosition;

    float distanceInKm = totalDistance / 1000.0f; 

    std::stringstream ss;
    ss << "Distance: " << std::fixed << std::setprecision(1) << distanceInKm << " km";  // Display up to one decimal point
    distanceText.setString(ss.str());

    // Position below the timer
    distanceText.setPosition(sf::Vector2f(198, 40));
}

// Get total distance traveled
float DistanceMeter::getTotalDistance() const {
    float distanceInKm = totalDistance / 1000.0f; // Convert meters to kilometers
    return std::round(distanceInKm * 10.0f) / 10.0f; // Round to one decimal point
}


void DistanceMeter::draw(sf::RenderWindow& window) {
    if (displayMeter){
        window.draw(distanceText);
    }
}

void DistanceMeter::show() { displayMeter = true; }
void DistanceMeter::hide() { displayMeter = false; }
