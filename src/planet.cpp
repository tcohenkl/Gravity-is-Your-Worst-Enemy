#include "planet.hpp"
#include <iostream>
#include <cmath>

Planet::Planet():
rotationSpeed(0.002) 
{
    if (!texture.loadFromFile("assets/sprites/planet.png")) {
        std::cerr << "Failed to load planet texture." << std::endl;
    } else {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        sprite.setScale({0.5,0.5});

        collisionSprite.setTexture(texture);
        collisionSprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        collisionSprite.setScale({0.47,0.47}); // 10% smaller than the original for collision testing

        // Setting up the gravity field
        gravityField.setRadius(texture.getSize().x / 2 * 1.1); // Assuming the gravity field has 1.5 times the radius of the planet
        gravityField.setOrigin(gravityField.getRadius(), gravityField.getRadius()); // Center the origin
        gravityField.setFillColor(sf::Color(0, 0, 255, 50)); // A translucent blue color
    }
}

void Planet::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
    collisionSprite.setPosition(position);
    gravityField.setPosition(position); // Set gravity field's position
}

void Planet::update() {
    sprite.rotate(rotationSpeed);
    collisionSprite.rotate(rotationSpeed);
    gravityField.rotate(rotationSpeed);
}

void Planet::draw(sf::RenderWindow& window) const {
    window.draw(gravityField); // Draw the gravity field first, so it's below the planet sprite
    window.draw(sprite);
}

const sf::Sprite& Planet::getCollisionSprite() const{
    return collisionSprite; 
}

bool Planet::isWithinGravityField(const sf::Vector2f& position) const {
    float distance = std::sqrt(std::pow(position.x - sprite.getPosition().x, 2) + 
                               std::pow(position.y - sprite.getPosition().y, 2));
    return distance <= gravityField.getRadius();
}

sf::Vector2f Planet::computeGravityForce(const sf::Vector2f& position) const {
    if (!isWithinGravityField(position)) {
        return {0.0f, 0.0f};
    }
    
    // Simple gravitational pull towards the center of the planet
    // You can adjust the gravitational constant as per your needs
    float gravitationalConstant = 0.000006f;
    sf::Vector2f direction = sprite.getPosition() - position;
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= magnitude; // Normalize direction

    return direction * gravitationalConstant;
}

sf::Vector2f Planet::getPosition() const {
    return sprite.getPosition();
}

float Planet::getRotationSpeed() const {
    return rotationSpeed;
}






