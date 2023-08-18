#pragma once
#include <SFML/Graphics.hpp>

struct CollisionDetail {
    bool hasCollided;
    bool isFatalCollision;
    sf::Vector2f collisionPoint;
};

class Rocket {
public:
    Rocket();
    
    void handleInput();
    void update();
    void draw(sf::RenderWindow &window);
    void setPosition(const sf::Vector2f &pos);
    void setVelocity(const sf::Vector2f &velocity);
    void applyForce(const sf::Vector2f& force);
    CollisionDetail checkCollision(const sf::Sprite& planetSprite);
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;

private:
    sf::Texture texture;          // rocket texture w/o thrust
    sf::Texture thrustTexture;    // rocket texture w thrust
    sf::Sprite sprite;

    const float rotationSpeed;
    const float acceleration;
    const float maxVelocity;
    sf::Vector2f velocity;

    bool isThrusting;

    float magnitude(const sf::Vector2f& vec);
    sf::Vector2f normalize(const sf::Vector2f& vec);
};
