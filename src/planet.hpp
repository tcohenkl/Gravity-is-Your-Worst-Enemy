#pragma once
#include <SFML/Graphics.hpp>

class Planet {
public:
    Planet();

    void setPosition(const sf::Vector2f& position);
    void draw(sf::RenderWindow& window) const;
    const sf::Sprite& getSprite() const;
    const sf::Sprite& getCollisionSprite() const;

    bool isWithinGravityField(const sf::Vector2f& position) const;
    sf::Vector2f computeGravityForce(const sf::Vector2f& position) const;

private:
    sf::Sprite sprite;
    sf::Texture texture; 
    sf::Sprite collisionSprite;
    sf::CircleShape gravityField; // Represents the translucent ring for gravity
};
