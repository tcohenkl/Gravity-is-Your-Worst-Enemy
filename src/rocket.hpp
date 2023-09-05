#pragma once
#include <SFML/Graphics.hpp>

// This structure provides details about a collision event.
// hasCollided: Indicates if the rocket has collided with an object.
// isFatalCollision: Indicates if the collision should be considered fatal (leading to a crash).
// collisionPoint: Specifies the point where the rocket collided with the object.
struct CollisionDetail {
    bool hasCollided;
    bool isFatalCollision;
    sf::Vector2f collisionPoint;
};

// Rocket class represents the player-controlled rocket.
class Rocket {
public:
    Rocket();  // Constructor initializes the rocket properties.

    // User input handling: navigation controls for the rocket.
    void handleInput();

    // Updates the rocket's state based on current velocity.
    void update();

    // Renders the rocket to the provided render window.
    void draw(sf::RenderWindow &window);

    // Setters for rocket's position and velocity.
    void setPosition(const sf::Vector2f &pos);
    void setVelocity(const sf::Vector2f &velocity);

    // Applies an external force to the rocket, changing its velocity.
    void applyForce(const sf::Vector2f& force);

    // Checks if the rocket has collided with the given planet sprite and returns collision details.
    CollisionDetail checkCollision(const sf::Sprite& planetSprite);
    
    // Getters for rocket's position, velocity, and forward direction.
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    sf::Vector2f getForwardDirection() const;

    // Setters for rocket's rotation and methods to enable or disable rotation.
    void setRotation(float angle);
    void enableRotation();
    void disableRotation();

    // Methods to simulate the rocket's landing and take-off behaviors.
    void land();
    void takeOff();

    // Checks if the rocket has landed.
    bool isLanded() const;

    // Resets the rocket to its initial state.
    void reset();  

private:
    sf::Texture texture;          // Rocket's base texture without thrust.
    sf::Texture thrustTexture;    // Rocket's texture when thrust is applied.
    sf::Sprite sprite;            // Sprite used for rendering the rocket.
    
    const float rotationSpeed;    // Speed at which the rocket rotates.
    const float acceleration;     // Acceleration value applied when thrusting.
    const float maxVelocity;     // Maximum speed the rocket can achieve.
    bool isThrusting;             // Flag indicating if thrust is currently applied.
    sf::Vector2f velocity;        // Current velocity vector of the rocket.

    // Utility functions to calculate vector magnitude and normalize vectors.
    float magnitude(const sf::Vector2f& vec);
    sf::Vector2f normalize(const sf::Vector2f& vec);

    bool canRotate;               // Flag indicating if the rocket can rotate.
    bool landed;                  // Flag indicating if the rocket has landed.
};
