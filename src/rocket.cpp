#include "rocket.hpp"
#include <cmath>
#include <iostream>
#include "Collision.h"


CollisionDetail Rocket::checkCollision(const sf::Sprite& planetSprite) {
    CollisionDetail detail = {false, false, {0, 0}};
    const float SPEED_THRESHOLD = 0.025f; 
    
    if (Collision::pixelPerfectTest(this->sprite, planetSprite)) {
        detail.hasCollided = true;

        // Calculate the approximate collision point as the closest point on the rocket to the planet's center.
        // This might not be the perfect approach, but should work in most scenarios.
        sf::Vector2f direction = planetSprite.getPosition() - this->getPosition();
        detail.collisionPoint = this->getPosition() + normalize(direction) * static_cast<float>(this->texture.getSize().x) / 2.0f;
        
        // Check collision speed
        float speed = magnitude(velocity);
        if (speed > SPEED_THRESHOLD) {  // define SPEED_THRESHOLD as per your game's requirement, e.g., 0.15f
            detail.isFatalCollision = true;
            return detail;
        }

        // Check if nose collided (by comparing collision point with sprite's rotation)
        float radianAngle = (sprite.getRotation() - 90.0f) * (3.14159f / 180.f);
        sf::Vector2f noseDirection(cos(radianAngle), sin(radianAngle));
        float dotProduct = direction.x * noseDirection.x + direction.y * noseDirection.y;

        if (dotProduct > 0) {
            // The nose is facing the planet at the collision point
            detail.isFatalCollision = true;
            return detail;
        }
    }

        return detail;
}



Rocket::Rocket() : 
rotationSpeed(0.05f), 
acceleration(0.00001f), 
maxVelocity(0.20f),
isThrusting(false) {

    if (texture.loadFromFile("assets/sprites/rocket.png")) {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        sprite.setScale(0.12f, 0.12f);
    } else {
        std::cerr << "failed to load rocket texture." << std::endl;
    }

    if (!thrustTexture.loadFromFile("assets/sprites/rocket_thrust.png")) {
        std::cerr << "failed to load rocket thrust texture." << std::endl;
    }
}

void Rocket::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        if(!isThrusting) {
            sprite.setTexture(thrustTexture);
            isThrusting = true;
        }

        float radianAngle = (sprite.getRotation() - 90.0f) * (3.14159f / 180.f);
        sf::Vector2f thrust(acceleration * cos(radianAngle), acceleration * sin(radianAngle));

        velocity += thrust;

        if (magnitude(velocity) > maxVelocity) {
            velocity = normalize(velocity) * maxVelocity; //keep direction
        }
    }
    else if(isThrusting) {
        sprite.setTexture(texture);
        isThrusting = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        sprite.rotate(-rotationSpeed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        sprite.rotate(rotationSpeed);
    }
}

void Rocket::update() {
    sprite.move(velocity);
}


void Rocket::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}

void Rocket::setPosition(const sf::Vector2f &pos) {
    sprite.setPosition(pos);
}

void Rocket::setVelocity(const sf::Vector2f &vel) {
    velocity = vel; 
}

// never thought i'd use it 
float Rocket::magnitude(const sf::Vector2f& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f Rocket::normalize(const sf::Vector2f& vec) {
    float mag = magnitude(vec);
    return sf::Vector2f(vec.x / mag, vec.y / mag);
}

sf::Vector2f Rocket::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2f Rocket::getVelocity() const {
    return velocity;
}

void Rocket::applyForce(const sf::Vector2f& force) {
    velocity += force;
}







