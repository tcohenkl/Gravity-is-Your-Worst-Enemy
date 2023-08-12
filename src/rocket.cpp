#include "rocket.hpp"
#include <cmath>
#include <iostream>

Rocket::Rocket() : 
rotationSpeed(0.05f), 
acceleration(0.00001f), 
maxVelocity(0.25f),
isThrusting(false) {

    if (texture.loadFromFile("assets/sprites/rocket.png")) {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        sprite.setScale(0.2f, 0.2f);
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

