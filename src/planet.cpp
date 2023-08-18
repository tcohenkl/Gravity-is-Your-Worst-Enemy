#include "planet.hpp"
#include <iostream>

Planet::Planet() {
    if (!texture.loadFromFile("assets/sprites/planet.png")) {
        std::cerr << "Failed to load planet texture." << std::endl;
    } else {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        sprite.setScale({0.5,0.5});
    }
}

void Planet::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
}

void Planet::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

const sf::Sprite& Planet::getSprite() const {
    return sprite;
}
