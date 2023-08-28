#include "rocket.hpp"
#include "planet.hpp"
#include "gameover.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>

/// STARS //////////////////////////////////////////////////

const int NUM_STARS = 200;
const float STAR_MAX_SIZE = 3.0f;
const int NUM_COLORS = 4;
sf::Color STAR_COLORS[NUM_COLORS] = {sf::Color(255, 204, 111), sf::Color(155, 176, 255), 
                                     sf::Color(255,114,118), sf::Color::White};

struct Star {
    sf::Vector2f position;
    float size;
    sf::Color color;
};

Star generateStarForScreen(const sf::Vector2f& rocketPosition) {
    float offset = 512.0f;
    Star star;
    star.position = sf::Vector2f(rocketPosition.x + (std::rand() % (int)offset) - offset/2,
                                 rocketPosition.y + (std::rand() % (int)offset) - offset/2);
    star.size = (std::rand() % (int)STAR_MAX_SIZE) + 1.0f;
    star.color = STAR_COLORS[std::rand() % NUM_COLORS];
    return star;
}

////////////////////////////////////////////////////////////////////////////////////////////////

enum GameState {
    PlayingState,   
    GameOverState
};


int main() {
    sf::RenderWindow window(sf::VideoMode(512, 512), "Rocket Game", sf::Style::Titlebar | sf::Style::Close);

    Rocket rocket;
    rocket.setPosition({256, 256});
    Planet planet;
    planet.setPosition({400,400});
    GameOver gameOverScreen;

    sf::View camera(sf::FloatRect(0, 0, 512, 512));
    std::vector<Star> stars;
    GameState currentState = PlayingState;

    for (int i = 0; i < NUM_STARS; ++i) {
        stars.push_back(generateStarForScreen(rocket.getPosition()));
    }

    while (window.isOpen()) {
        sf::Event evnt;

        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
                window.close();

            if (currentState == GameOverState && evnt.type == sf::Event::KeyPressed && evnt.key.code == sf::Keyboard::R) {
                currentState = PlayingState;
                rocket.setPosition({256, 256});
                rocket.setVelocity({0,0});
                rocket.takeOff();
            }
        }

        if (currentState == PlayingState) {
            rocket.handleInput();
            rocket.update();
            planet.update();

            if (planet.isWithinGravityField(rocket.getPosition())) {
                sf::Vector2f gravityForce = planet.computeGravityForce(rocket.getPosition());
                rocket.applyForce(gravityForce);
            }

            if (rocket.isLanded()) {
                sf::Vector2f diff = rocket.getPosition() - planet.getPosition();
                float angleRad = planet.getRotationSpeed() * (3.14159265 / 180); // Convert rotation speed to radians.

                // Rotate the difference vector.
                sf::Vector2f newDiff(
                    diff.x * cos(angleRad) - diff.y * sin(angleRad),
                    diff.x * sin(angleRad) + diff.y * cos(angleRad)
                );

                // Set the new rocket position.
                rocket.setPosition(planet.getPosition() + newDiff);

                // Compute rotation angle for the rocket.
                float rocketAngle = atan2(newDiff.y, newDiff.x) * (180 / 3.14159265); // Convert radians to degrees.
                rocket.setRotation(rocketAngle + 90); // +90 because rocket's initial orientation is assumed to be up.
            }

           
            CollisionDetail collisionDetail = rocket.checkCollision(planet.getCollisionSprite());
            if (collisionDetail.hasCollided) {
                if (collisionDetail.isFatalCollision) {
                    rocket.setVelocity({0,0});
                    gameOverScreen.resetAlpha();
                    currentState = GameOverState;
                } else {
                    rocket.land();
                }
            } else if (rocket.isLanded()) {
                rocket.takeOff();
            }

            for (int i = 0; i < NUM_STARS; ++i) {
                stars[i].position -= rocket.getVelocity();
                sf::Vector2f diff = stars[i].position - rocket.getPosition();
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance > 400) {
                    stars[i] = generateStarForScreen(rocket.getPosition());
                }
            }

            camera.setCenter(rocket.getPosition());
            window.setView(camera);
            window.clear();

            for (int i = 0; i < NUM_STARS; ++i) {
                sf::CircleShape starShape(stars[i].size);
                starShape.setPosition(stars[i].position);
                starShape.setFillColor(stars[i].color);
                window.draw(starShape);
            }

            rocket.draw(window);
            planet.draw(window);
            

        } else if (currentState == GameOverState) {
            camera.setCenter(256, 256);
            window.setView(camera);
            window.clear();
            gameOverScreen.fadeIn();
            gameOverScreen.draw(window);
        }

        window.display();
    }
}
