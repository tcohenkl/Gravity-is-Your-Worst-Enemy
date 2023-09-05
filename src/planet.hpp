#pragma once
#include <SFML/Graphics.hpp>

class Planet {
public:
    Planet();

    // Setters
    void setPosition(const sf::Vector2f& position);

    // Core functions
    void update(); // Updates the state of the planet
    void draw(sf::RenderWindow& window) const; // Draws the planet on the given window

    // Getters
    const sf::Sprite& getCollisionSprite() const;
    sf::Vector2f getPosition() const;
    float getRotationSpeed() const;

    // Physics functions
    bool isWithinGravityField(const sf::Vector2f& position) const;  // Checks if given position lies within planet's gravity field
    sf::Vector2f computeGravityForce(const sf::Vector2f& position) const; // Computes the gravity force exerted by the planet on a position

private:
    sf::Sprite sprite;
    sf::Texture texture; 
    sf::Sprite collisionSprite;
    sf::CircleShape gravityField; // Represents the translucent ring for gravity
    const float rotationSpeed; 
};
