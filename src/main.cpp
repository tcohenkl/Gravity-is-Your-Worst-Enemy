#include "rocket.hpp"
#include "planet.hpp"
#include "gameover.hpp"
#include "timer.hpp"
#include "distancemeter.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <memory>

// Constants related to the stars in the game.
const int NUM_STARS = 200; // Number of stars in the game.
const float STAR_MAX_SIZE = 3.0f; // Maximum size for a star.
const int NUM_COLORS = 4; // Number of available star colours.
sf::Color STAR_COLORS[NUM_COLORS] = {
    sf::Color(255, 204, 111),  // A shade of yellow.
    sf::Color(155, 176, 255),  // A shade of blue.
    sf::Color(255,114,118),    // A shade of red.
    sf::Color::White           // White colour.
 /* These colours are actually real shades of star colours in the galaxy*/
};

// Struct to define the properties of a star.
struct Star {
    sf::Vector2f position;  // Position of the star in the game window.
    float size;             // Size of the star.
    sf::Color color;        // Colour of the star.
};

// Generates a star for the game screen based on the rocket's current position.
// The rocketPosition parameter represents the current position of the rocket.
// the function returns a generated star with random position, size, and colour.
Star generateStarForScreen(const sf::Vector2f& rocketPosition) {
    float offset = 512.0f;
    Star star;
    star.position = sf::Vector2f(
        rocketPosition.x + (std::rand() % (int)offset) - offset/2,
        rocketPosition.y + (std::rand() % (int)offset) - offset/2
    );
    star.size = (std::rand() % (int)STAR_MAX_SIZE) + 1.0f;  // Randomize star size within defined range.
    star.color = STAR_COLORS[std::rand() % NUM_COLORS];     // Randomly pick a color for the star.
    return star;
}


// Get a random offscreen position based on the current position of the rocket.
// * The function ensures that the generated position is not too close or too far from the rocket,
// .. and is not within a certain rectangular area around the rocket.
// rocketPos represents the the current position of the rocket.
// The function returns a a randomly generated offscreen position.
sf::Vector2f getRandomOffscreenPosition(const sf::Vector2f& rocketPos) {
    float angle;         // Angle for calculating the new position.
    float distance;      // Distance for calculating the new position.
    sf::Vector2f newPos; // New generated position.

    float actualDistance;

    // Adjusted values
    const float MIN_SAFE_DISTANCE = 200.0f; // Minimum distance from the rocket for the new position.
    const float MAX_DISTANCE = 300.0f;      // Maximum distance from the rocket for the new position.
    const float RECT_WIDTH = 150.0f;        // Width of the rectangular exclusion zone around the rocket.
    const float RECT_HEIGHT = 150.0f;       // Height of the rectangular exclusion zone around the rocket.

    do {
        angle = (rand() % 360) * (3.14159265 / 180);  // Generate a random angle.
        distance = MIN_SAFE_DISTANCE + (rand() % (int)(MAX_DISTANCE - MIN_SAFE_DISTANCE + 1)); // Randomize distance within the safe range.

        // Calculate the new position based on angle and distance.
        newPos.x = rocketPos.x + distance * cos(angle);
        newPos.y = rocketPos.y + distance * sin(angle);

        // Calculate the actual distance from the rocket to the new position.
        actualDistance = std::sqrt(std::pow(newPos.x - rocketPos.x, 2) + std::pow(newPos.y - rocketPos.y, 2));

    } while (
        actualDistance < MIN_SAFE_DISTANCE || 
        actualDistance > MAX_DISTANCE || 
        (newPos.x > rocketPos.x - RECT_WIDTH/2 && newPos.x < rocketPos.x + RECT_WIDTH/2 &&
         newPos.y > rocketPos.y - RECT_HEIGHT/2 && newPos.y < rocketPos.y + RECT_HEIGHT/2)
    ); // Ensure the new position meets the criteria.

    return newPos;  // Return the new generated position.
}

// Enumeration to represent the game states.
enum GameState {
    PlayingState,   // State when the game is in progress.
    GameOverState   // State when the game is over.
};


int main() {
    // 1. Initialization
    // ==========================

    // 1.1. Random Seed Initialization
    // --------------------------
    // Ensuring randomness in game elements based on the current time.
    srand(static_cast<unsigned int>(time(nullptr)));

    // 1.2. Window and Game State Initialization
    // --------------------------
    // Setting up the main game window and initial game state.
    sf::RenderWindow window(sf::VideoMode(512, 512), "Gravity is Your Worst Enemy", sf::Style::Titlebar | sf::Style::Close);
    sf::View defaultView = window.getDefaultView();

    GameState currentState = PlayingState;

    // 1.3. Rocket Initialization
    // --------------------------
    // Positioning the rocket at the center of the window.
    Rocket rocket;
    rocket.setPosition({256, 256});
    sf::View camera(sf::FloatRect(0, 0, 512, 512));

    // 1.4. Game Over Screen Initialization
    // --------------------------
    GameOver gameOverScreen;

    // 1.5. Star Initialization
    // --------------------------
    // Generating stars for the background.
    std::vector<Star> stars;
    for (int i = 0; i < NUM_STARS; ++i) {
        stars.push_back(generateStarForScreen(rocket.getPosition()));
    }

    // 1.6. Explosion Elements Initialization
    // --------------------------
    // Load explosion animation frames and setup related variables.
    std::vector<sf::Texture> explosionTextures;
    for (int i = 1; i <= 2; ++i) {
        sf::Texture tex;
        if (!tex.loadFromFile("assets/sprites/explosion" + std::to_string(i) + ".png")) {
            std::cerr << "Failed to load explosion frame " << i << std::endl;
        } else {
            explosionTextures.push_back(tex);
        }
    }
    sf::Sprite explosionSprite;
    sf::Clock deltaClock;
    float explosionFrameTimer = 0.0f; 
    const float EXPLOSION_FRAME_DURATION = 0.2f;
    bool isExploding = false;
    int currentExplosionFrame = 0;
    bool triggerGameOver = false;

    // 1.7. Planet Spawning/Generation Initialization
    // --------------------------
    // Setup planet-related timers, constants, and initial gameplay duration.
    std::vector<std::shared_ptr<Planet>> planets;
    float planetSpawnTimer = 0.0f;
    const float PLANET_DELETION_DISTANCE = 450.0f;
    float PLANET_SPAWN_INTERVAL = 3.0f;
    const float MIN_PLANET_SPAWN_INTERVAL = 1.0f;
    float gameplayDuration = 0.0f;
    const float DURATION_BEFORE_SPAWN_INTERVAL_REDUCTION = 30.0f;

    // 1.8. Timer Initialization
    // --------------------------
    // Setting up the main game timer.
    Timer gameTimer; 
    bool isTimerStarted = false; 
    float gameplayDurationInterval = 0.0f; 

    // 1.9. Distance Meter Initialization
    // --------------------------
    // Initialize the distance meter that tracks the rocket's journey.
    DistanceMeter distanceMeter;
    distanceMeter.reset(rocket.getPosition());

    // ==========================

    // 2. Main Game Loop
    // ==========================
    // The main loop where event handling, game logic, updates, and rendering occur.
    while (window.isOpen()) {

        // 2.1. Event Processing
        // --------------------------
        // Handle window and keyboard events.
        sf::Event evnt;
    
        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
                window.close();
            
            // Handle 'W' keypress during PlayingState to initiate timer.
            if (currentState == PlayingState && evnt.type == sf::Event::KeyPressed && evnt.key.code == sf::Keyboard::W) {
                isTimerStarted = true;
                gameTimer.show();
                distanceMeter.show(); 
            }

            // Handle 'R' keypress during GameOverState to restart the game.
            if (currentState == GameOverState && evnt.type == sf::Event::KeyPressed && 
                evnt.key.code == sf::Keyboard::R) {
                    // Reset gameplay attributes for a fresh start
                    currentState = PlayingState;
                    rocket.reset(); 
                    distanceMeter.reset(rocket.getPosition());
                    distanceMeter.hide(); 
                    planets.clear(); 
                    PLANET_SPAWN_INTERVAL = 3.0f;
                    gameplayDuration = 0.0f; 
                    gameTimer.reset();
                    gameTimer.hide();
                    isTimerStarted = false; 
            }
        }


        // 2.2. Time Update
        // --------------------------
        // Calculate elapsed time since the last frame for gameplay updates.
        float deltaTime = deltaClock.restart().asSeconds();

        // 2.3. Game State Logic
        // --------------------------
        if (currentState == PlayingState) {

            // Timer and gameplay duration update.
            if (isTimerStarted) {
                gameTimer.update(deltaTime);
                gameplayDuration += deltaTime;
            }

            // Handle rocket's user inputs and update its state.
            rocket.handleInput();
            rocket.update();
            distanceMeter.update(rocket.getPosition());
           
            // Planet spawning and management.
            planetSpawnTimer += deltaTime; 
            
            // Planet generation logic, ensuring a valid position.
            if (planetSpawnTimer >= PLANET_SPAWN_INTERVAL && planets.size() < 10) {
                sf::Vector2f pos;
                bool validPosition;
                int maxAttempts = 5;
                
                do {
                    pos = getRandomOffscreenPosition(rocket.getPosition());
                    validPosition = true;
                    // Ensure this position isn't too close to any existing planet
                    for (const auto& planetPtr : planets) {
                        float dist = std::sqrt(std::pow(planetPtr->getPosition().x - pos.x, 2) + std::pow(planetPtr->getPosition().y - pos.y, 2));
                        if (dist < planetPtr->getCollisionSprite().getGlobalBounds().width) {
                            validPosition = false;
                            break;
                        }
                    }
                    maxAttempts--;
                } while (!validPosition && maxAttempts > 0);

                if (validPosition) {
                    auto newPlanet = std::make_shared<Planet>();
                    newPlanet->setPosition(pos);
                    planets.push_back(newPlanet);
                    planetSpawnTimer = 0; // Reset the timer
                }

                    gameplayDurationInterval = gameplayDuration; 
                if (gameplayDurationInterval >= DURATION_BEFORE_SPAWN_INTERVAL_REDUCTION) {
                    PLANET_SPAWN_INTERVAL = std::max(PLANET_SPAWN_INTERVAL - 1.0f, MIN_PLANET_SPAWN_INTERVAL); // Reduce the spawn interval by 1
                    gameplayDurationInterval = DURATION_BEFORE_SPAWN_INTERVAL_REDUCTION; // Reset the gameplay duration timer for the next reduction
                }
            }

            // Logic for updating and managing gravity, collisions, and planet cleanup.
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
                        // * Stop the rocket.
                        // * Initiate explosion sequence.
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

            for (auto it = planets.begin(); it != planets.end(); ) {
                sf::Vector2f diff = rocket.getPosition() - (*it)->getPosition();
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance > PLANET_DELETION_DISTANCE) {
                    it = planets.erase(it); // Erase returns the next valid iterator
                } else {
                    ++it;
                }
            }
       
            // Handle star generation and update based on rocket's movement.
            for (int i = 0; i < NUM_STARS; ++i) {
                stars[i].position -= rocket.getVelocity();
                sf::Vector2f diff = stars[i].position - rocket.getPosition();
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (distance > 400) {
                    stars[i] = generateStarForScreen(rocket.getPosition());
                }
            }
            
            // Set the 'camera' to follow the rocket.
            camera.setCenter(rocket.getPosition());
            window.setView(camera);
            window.clear();

            // Render Stars
            for (int i = 0; i < NUM_STARS; ++i) {
                sf::CircleShape starShape(stars[i].size);
                starShape.setPosition(stars[i].position);
                starShape.setFillColor(stars[i].color);
                window.draw(starShape);
            }

            // Render rocket and planets
            rocket.draw(window);
            for (const auto& planetPtr : planets) {
                Planet& planet = *planetPtr;  // Dereference to get the Planet object
                planet.draw(window);
            }

            // Handle the explosion sequence
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

            // Transition to GameOver State if required
            if (triggerGameOver && !isExploding) {
                gameOverScreen.resetAlpha();
                currentState = GameOverState;
                triggerGameOver = false;
            }
        }

        // 2.4. Rendering UI and Game Over Screen
        // --------------------------
        window.setView(defaultView);
        if (currentState == PlayingState) {   // Draw the timer only in the playing state
            gameTimer.draw(window);
            distanceMeter.draw(window);
        }

        if (currentState == GameOverState) {
            camera.setCenter(256, 256);
            window.setView(camera);
            window.clear();
            // Set and manage the Game Over screen attributes and fade-ins.
            gameOverScreen.setDistance(distanceMeter.getTotalDistance()); 
            gameOverScreen.setSurvivalTime(gameplayDuration);
            gameOverScreen.fadeIn();
            gameOverScreen.draw(window);
        }

        window.setView(camera);
        window.display();
    }
// ==========================

}