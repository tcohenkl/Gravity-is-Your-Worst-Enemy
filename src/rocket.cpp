#include "rocket.hpp"
#include <cmath>
#include <iostream>
#include "Collision.h"


CollisionDetail Rocket::checkCollision(const sf::Sprite& planetSprite) {
    CollisionDetail detail = {false, false, {0, 0}};
    const float SPEED_THRESHOLD = 0.04f; 
    
    if (Collision::pixelPerfectTest(this->sprite, planetSprite)) {
        detail.hasCollided = true;

        // Calculate the approximate collision point as the closest point on the rocket to the planet's center.
        sf::Vector2f direction = planetSprite.getPosition() - this->getPosition();
        detail.collisionPoint = this->getPosition() + normalize(direction) * static_cast<float>(this->texture.getSize().x) / 2.0f;
        
        // Check collision speed
        float speed = magnitude(velocity);
        if (speed > SPEED_THRESHOLD) {
            detail.isFatalCollision = true;
            return detail;
        }

        // Check if nose collided (by comparing collision point with sprite's rotation)
        float radianAngle = (sprite.getRotation() - 90.0f) * (M_PI / 180.f);
        sf::Vector2f noseDirection(cos(radianAngle), sin(radianAngle));
        float dotProduct = direction.x * noseDirection.x + direction.y * noseDirection.y;

        float noseMagnitude = std::sqrt(noseDirection.x * noseDirection.x + noseDirection.y * noseDirection.y);
        float directionMagnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Compute the angle between nose direction and collision direction
        float angleBetween = acos(dotProduct / (noseMagnitude * directionMagnitude)) * (180.0f / M_PI);

        // Threshold can be adjusted based on how strict you want the check to be
        float thresholdAngle = 165.0f;  // this means if the angle is less than 165 degrees, it's a fatal collision (15 deg)
        if (angleBetween < thresholdAngle) {
            detail.isFatalCollision = true;
            std::cout << "Crash due to improper landing." << std::endl;
            return detail;
        }

    }
        return detail;
}


Rocket::Rocket() : 
rotationSpeed(0.05f), 
acceleration(0.00001f), 
maxVelocity(0.20f),
isThrusting(false),
canRotate(true), 
landed(false) // minimum velocity magnitude before we consider the rocket settled
{

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

    if(canRotate) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            sprite.rotate(-rotationSpeed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            sprite.rotate(rotationSpeed);
        }
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

sf::Vector2f Rocket::getForwardDirection() const {
    float radianAngle = (sprite.getRotation() - 90.0f) * (3.14159f / 180.f);
    return sf::Vector2f(cos(radianAngle), sin(radianAngle));
}

void Rocket::applyForce(const sf::Vector2f& force) {
    velocity += force;
}

void Rocket::setRotation(float angle) {
    sprite.setRotation(angle);
}

void Rocket::enableRotation() { canRotate = true; }
void Rocket::disableRotation() { canRotate = false; }

void Rocket::land() {
    landed = true;
    disableRotation();
    setVelocity({0,0});
}

void Rocket::takeOff() {
    landed = false;
    enableRotation();
}

bool Rocket::isLanded() const {
    return landed;
}








 










