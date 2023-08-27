#include "rocket.hpp"
#include "planet.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>

/// STARS //////////////////////////////////////////////////

const int NUM_STARS = 200;  // num of stars that appear on screen
const float STAR_MAX_SIZE = 3.0f; // 3 pixel max size
const int NUM_COLORS = 4;  // number of  star colors
sf::Color STAR_COLORS[NUM_COLORS] = {sf::Color(255, 204, 111), sf::Color(155, 176, 255), 
                                     sf::Color(255,114,118), sf::Color::White};

struct Star {
    sf::Vector2f position;
    float size;
    sf::Color color;
};

// generates the stars on the screen next to rocket
Star generateStarForScreen(const sf::Vector2f& rocketPosition) {
    float offset = 512.0f;  // screen width/height
    Star star;
    star.position = sf::Vector2f(rocketPosition.x + (std::rand() % (int)offset) - offset/2,
                                 rocketPosition.y + (std::rand() % (int)offset) - offset/2);
    star.size = (std::rand() % (int)STAR_MAX_SIZE) + 1.0f;
    star.color = STAR_COLORS[std::rand() % NUM_COLORS];  // Randomly pick a color for the star
    return star;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    sf::RenderWindow window(sf::VideoMode(512, 512), "Rocket Game", sf::Style::Titlebar | sf::Style::Close);

    Rocket rocket;
    rocket.setPosition({256, 256});
    Planet planet; 
    planet.setPosition({400,400});

    sf::View camera(sf::FloatRect(0, 0, 512, 512));
    std::vector<Star> stars;

    // Initialize stars covering the rocket's screen
    for (int i = 0; i < NUM_STARS; ++i) {
        stars.push_back(generateStarForScreen(rocket.getPosition()));
    }

    while (window.isOpen()) {
        sf::Event evnt;

        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
                window.close();
        }

        rocket.handleInput();

        if (planet.isWithinGravityField(rocket.getPosition())) {
            sf::Vector2f gravityForce = planet.computeGravityForce(rocket.getPosition());
            rocket.applyForce(gravityForce);
        }

        rocket.update();

        // Handle collisions
        CollisionDetail collisionDetail = rocket.checkCollision(planet.getCollisionSprite());
        if (collisionDetail.hasCollided) {
            if (collisionDetail.isFatalCollision) {
                std::cout << "CRASH!" << std::endl;
                rocket.setVelocity({0,0});
            } else {
                rocket.setVelocity({0,0});
            }
        }

        // update stars based on rocket velocity
        for (int i = 0; i < NUM_STARS; ++i) {
            stars[i].position -= rocket.getVelocity();

            // if star too far, replace with a new star
            sf::Vector2f diff = stars[i].position - rocket.getPosition();
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (distance > 400) { // you can adjust this 
                stars[i] = generateStarForScreen(rocket.getPosition());
            }
        }

        //updates camera to follow rocket
        camera.setCenter(rocket.getPosition());
        window.setView(camera);
        window.clear();

        // Drawing stars
        for (int i = 0; i < NUM_STARS; ++i) {
            sf::CircleShape starShape(stars[i].size);
            starShape.setPosition(stars[i].position);
            starShape.setFillColor(stars[i].color);  // Set the star's color
            window.draw(starShape);
        }
        
        rocket.draw(window);
        planet.draw(window);
        window.display();
    }
}
