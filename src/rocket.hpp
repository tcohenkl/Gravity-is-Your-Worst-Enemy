#pragma once
#include <SFML/Graphics.hpp>



// Collision Structure with information about collision:
// Includes: 
//         * Whether the rocket has collided
//         * The rocket passed the criteria for a fatal collision
//         * Where the rocket collided with said object 
struct CollisionDetail {
    bool hasCollided;
    bool isFatalCollision;
    sf::Vector2f collisionPoint;
};


// Rocket Class (Controlled by player)
class Rocket {
public:
    Rocket();
   
    // handleInput() Handles keyboard presses by the player to navigate the Rocket
    void handleInput();

    // update() Moves the rocket sprite
    void update();
 
    // draw(window) Draws the rocket onto the window
    void draw(sf::RenderWindow &window);

    // setPosition(pos) Sets the position of the rocket anywhere in the window
    void setPosition(const sf::Vector2f &pos);

    // setVelocity(velocity) Changes the velocity of the rocket
    void setVelocity(const sf::Vector2f &velocity);
   
   // applyForce(force) Applies a force to the rocket
    void applyForce(const sf::Vector2f& force);
   
    CollisionDetail checkCollision(const sf::Sprite& planetSprite);
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    sf::Vector2f getForwardDirection() const;
    void setRotation(float angle);
    void enableRotation();
    void disableRotation();
    void land();
    void takeOff();
    bool isLanded() const;
    void reset();  

private:
    sf::Texture texture;          // rocket texture w/o thrust
    sf::Texture thrustTexture;    // rocket texture w thrust
    sf::Sprite sprite;
    

    const float rotationSpeed;
    const float acceleration;
    const float maxVelocity;
    bool isThrusting;
    sf::Vector2f velocity;

    float magnitude(const sf::Vector2f& vec);
    sf::Vector2f normalize(const sf::Vector2f& vec);

    bool canRotate;
    bool landed; 
};
