#include "rocket.hpp"
#include "planet.hpp"
#include "gameover.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <memory>

// Constants for the stars in the game.
const int NUM_STARS = 200;
const float STAR_MAX_SIZE = 3.0f;
const int NUM_COLORS = 4;
sf::Color STAR_COLORS[NUM_COLORS] = {
    sf::Color(255, 204, 111),
    sf::Color(155, 176, 255),
    sf::Color(255,114,118),
    sf::Color::White
};

struct Star {
    sf::Vector2f position;
    float size;
    sf::Color color;
};

Star generateStarForScreen(const sf::Vector2f& rocketPosition) {
    float offset = 512.0f;
    Star star;
    star.position = sf::Vector2f(
        rocketPosition.x + (std::rand() % (int)offset) - offset/2,
        rocketPosition.y + (std::rand() % (int)offset) - offset/2
    );
    star.size = (std::rand() % (int)STAR_MAX_SIZE) + 1.0f;
    star.color = STAR_COLORS[std::rand() % NUM_COLORS];
    return star;
}

sf::Vector2f getRandomOffscreenPosition(const sf::Vector2f& rocketPos) {
    float angle = (rand() % 360) * (3.14159265 / 180);
    float distance = 100 + (rand() % 300);

    sf::Vector2f newPos;
    newPos.x = rocketPos.x + distance * cos(angle);
    newPos.y = rocketPos.y + distance * sin(angle);

    return newPos;
}

enum GameState {
    PlayingState,   
    GameOverState
};



int main() {
    // Random Seed 
     srand(static_cast<unsigned int>(time(nullptr)));
    
    
    // Initialize window and game state
    sf::RenderWindow window(sf::VideoMode(512, 512), "Rocket Game", sf::Style::Titlebar | sf::Style::Close);
    GameState currentState = PlayingState;
    
    Rocket rocket;
    rocket.setPosition({256, 256});

    GameOver gameOverScreen;

    sf::View camera(sf::FloatRect(0, 0, 512, 512));
    std::vector<Star> stars;
    for (int i = 0; i < NUM_STARS; ++i) {
        stars.push_back(generateStarForScreen(rocket.getPosition()));
    }

    // Initialize explosion related elements
    std::vector<sf::Texture> explosionTextures;
    for (int i = 1; i <= 2; ++i) {
        sf::Texture tex;
        if (!tex.loadFromFile("assets/sprites/explosion" + std::to_string(i) + ".png")) {
            std::cerr << "Failed to load explosion frame " << i << std::endl;
        } else {
            explosionTextures.push_back(tex); // array 
        }
    }

    sf::Sprite explosionSprite;
    sf::Clock deltaClock;
    float explosionFrameTimer = 0.0f; 
    const float EXPLOSION_FRAME_DURATION = 0.2f;
    bool isExploding = false;
    int currentExplosionFrame = 0;
    bool triggerGameOver = false;


    std::vector<std::shared_ptr<Planet>> planets;


    while (window.isOpen()) {

        sf::Event evnt;
    
        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
                window.close();

            if (currentState == GameOverState && evnt.type == sf::Event::KeyPressed && 
                evnt.key.code == sf::Keyboard::R) {
                    currentState = PlayingState;
                    rocket.setPosition({256, 256});
                    rocket.setVelocity({0,0});
                    rocket.takeOff();
            }
        }


        // Game logic and rendering
        float deltaTime = deltaClock.restart().asSeconds();


        if (currentState == PlayingState) {
            rocket.handleInput();
            rocket.update();
           //  planet.update();

            if (rand() % 10000 == 0 & planets.size() < 10) {
                auto newPlanet = std::make_shared<Planet>();
                sf::Vector2f pos = getRandomOffscreenPosition(rocket.getPosition());
                std::cout << "Spawning planet at: (" << pos.x << ", " << pos.y << ")" << std::endl;
                newPlanet->setPosition(pos);
                planets.push_back(newPlanet);
            }

            for (const auto& planetPtr : planets) {
                Planet& planet = *planetPtr;
                // 1. Update the planet's state.
                planet.update();

                // 2. Gravity effect.
                if (planet.isWithinGravityField(rocket.getPosition())) {
                    sf::Vector2f gravityForce = planet.computeGravityForce(rocket.getPosition());
                    rocket.applyForce(gravityForce);
                }

                // 3. Check for collisions between the rocket and the planet.
                CollisionDetail collisionDetail = rocket.checkCollision(planet.getCollisionSprite());
                if (collisionDetail.hasCollided && !isExploding) {
                    if (collisionDetail.isFatalCollision) {
                        // Handle fatal collision: 
                        // - Stop the rocket.
                        // - Initiate the explosion sequence.
                        rocket.setVelocity({0,0});
                        isExploding = true;
                        explosionSprite.setTexture(explosionTextures[currentExplosionFrame]);
                        explosionSprite.setOrigin(explosionTextures[currentExplosionFrame].getSize().x / 2, explosionTextures[currentExplosionFrame].getSize().y / 2);
                        explosionSprite.setPosition(rocket.getPosition());
                        explosionSprite.setScale(0.3, 0.3);
                    } else {
                        // Handle non-fatal collision:
                        // - Make the rocket land on the planet.
                        rocket.land();
                    }
                } else if (rocket.isLanded() && !isExploding) {
                rocket.takeOff();
                }

                // 4. Adjust rocket position and rotation if it has landed on this planet.
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
            for (const auto& planetPtr : planets) {
                Planet& planet = *planetPtr;  // Dereference to get the Planet object
                planet.draw(window);
            }


            if (isExploding && currentExplosionFrame < explosionTextures.size()) {
                explosionFrameTimer += deltaTime; // Update the explosion timer

                if (explosionFrameTimer >= EXPLOSION_FRAME_DURATION) {
                    explosionFrameTimer = 0.0f; // Reset the timer
                    currentExplosionFrame++; // Go to next frame
            
                    if (currentExplosionFrame >= explosionTextures.size()) {
                        isExploding = false;
                        triggerGameOver = true;
                        currentExplosionFrame = 0; // Reset the explosion frame for the next explosion
                    } else {
                        explosionSprite.setTexture(explosionTextures[currentExplosionFrame]);
                    }
                }
                    window.draw(explosionSprite);
            } 

            if (triggerGameOver && !isExploding) {
                gameOverScreen.resetAlpha();
                currentState = GameOverState;
                triggerGameOver = false;
            }

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