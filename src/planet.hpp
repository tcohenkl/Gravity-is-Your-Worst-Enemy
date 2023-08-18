#pragma once
#include <SFML/Graphics.hpp>

class Planet {
public:
    Planet();

    void setPosition(const sf::Vector2f& position);
    void draw(sf::RenderWindow& window) const;
    const sf::Sprite& getSprite() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;  // Added this line
};
