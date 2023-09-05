#pragma once
#include <SFML/Graphics.hpp>


class Planet {
public:
    // Constructor initializes the planet's properties.
    Planet();

    // Sets the planet's position on the screen.
    void setPosition(const sf::Vector2f& position);

    // Updates the planet's state (like rotation).
    void update();

    // Renders the planet to the provided render window.
    void draw(sf::RenderWindow& window) const;

    // Getter methods:
    // Returns the sprite used for collision detection.
    const sf::Sprite& getCollisionSprite() const;
    
    // Gets the position of the planet.
    sf::Vector2f getPosition() const;

    // Retrieves the rotation speed of the planet.
    float getRotationSpeed() const;

    // Physics methods:
    // Checks if the provided position lies within the planet's gravity field.
    bool isWithinGravityField(const sf::Vector2f& position) const;

    // Computes the gravitational force exerted by the planet on a specific position.
    sf::Vector2f computeGravityForce(const sf::Vector2f& position) const;

private:
    // Visual representation of the planet.
    sf::Sprite sprite;

    // Texture used for the planet's sprite.
    sf::Texture texture;

    // Sprite used specifically for collision detection.
    sf::Sprite collisionSprite;

    // Visual representation of the planet's gravitational field.
    sf::CircleShape gravityField;

    // Speed at which the planet rotates.
    const float rotationSpeed;
};
