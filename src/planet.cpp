#include "planet.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>  

Planet::Planet():
rotationSpeed(0.002) 
{
    // Load the planet texture
    if (!texture.loadFromFile("assets/sprites/planet.png")) {
        std::cerr << "Failed to load planet texture." << std::endl;
    } else {
        // Set texture and origin for sprite
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

        // Randomize sprite scale and color
        float randomScale = 0.3f + static_cast<float>(rand()) / RAND_MAX * 0.5f;
        sprite.setScale({randomScale, randomScale});
        sprite.setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));

        // Set texture, origin, and scale for collision sprite
        collisionSprite.setTexture(texture);
        collisionSprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        collisionSprite.setScale({randomScale * 0.98f, randomScale * 0.98f}); 

        // Configure the gravity field
        gravityField.setRadius(texture.getSize().x / 2 * 1.1); 
        gravityField.setOrigin(gravityField.getRadius(), gravityField.getRadius());
        gravityField.setFillColor(sf::Color(0, 0, 255, 50)); 
    }
}

void Planet::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
    collisionSprite.setPosition(position);
    gravityField.setPosition(position); 
}

void Planet::update() {
    // Rotate planet, collision sprite, and gravity field
    sprite.rotate(rotationSpeed);
    collisionSprite.rotate(rotationSpeed);
    gravityField.rotate(rotationSpeed);
}

void Planet::draw(sf::RenderWindow& window) const {
    window.draw(gravityField); 
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
    
    // Compute gravitational pull towards the center of the planet
    const float gravitationalConstant = 0.000006f; // Can be adjusted for strength
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