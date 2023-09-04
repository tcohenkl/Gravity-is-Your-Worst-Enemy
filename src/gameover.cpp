#include "gameover.hpp"
#include <iostream> 
#include <iomanip>  // for std::setprecision

GameOver::GameOver() :
    fadeAlpha(0.0)
 {
    if (!font.loadFromFile("assets/fonts/Orbitron-Bold.ttf")) {
        std::cerr << "failed to load font." << std::endl;
    }

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(90, 200); 
    
    restartText.setFont(font);
    restartText.setString("Press 'R' to Restart");
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(128, 300);

    survivalText.setFont(font);
    survivalText.setCharacterSize(18);
    survivalText.setFillColor(sf::Color::White);
    survivalText.setPosition(115, 140); 
}

void GameOver::setSurvivalTime(float seconds) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2); // This sets the decimal precision to 2 places.
    ss << "You survived: " << seconds << " seconds";
    survivalText.setString(ss.str());
}


void GameOver::draw(sf::RenderWindow& window) {
    window.draw(gameOverText);
    window.draw(restartText);
    window.draw(survivalText); 
}

void GameOver::setAlpha(sf::Uint8 alpha) {
    sf::Color gameOverTextColor = gameOverText.getFillColor();
    gameOverTextColor.a = alpha;
    gameOverText.setFillColor(gameOverTextColor);

    sf::Color restartTextColor = restartText.getFillColor();
    restartTextColor.a = alpha;
    restartText.setFillColor(restartTextColor);

    sf::Color survivalTextColor = survivalText.getFillColor(); 
    survivalTextColor.a = alpha; 
    survivalText.setFillColor(survivalTextColor);
}

 void GameOver::fadeIn() {
        fadeAlpha += 0.03;  // Adjust this value to change the speed of the fade-in
        if (fadeAlpha > 255) fadeAlpha = 255;
        setAlpha(static_cast<sf::Uint8>(fadeAlpha));
}

bool GameOver::checkForRestart(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
        return true;
    }
    return false;
}

void GameOver::resetAlpha() {
    fadeAlpha = 0.0;
    setAlpha(0); // set text alpha to fully transparent
}
