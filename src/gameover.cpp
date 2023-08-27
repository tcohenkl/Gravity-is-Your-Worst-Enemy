#include "gameover.hpp"
#include <iostream> 

GameOver::GameOver() {
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
}

void GameOver::draw(sf::RenderWindow& window) {
    window.draw(gameOverText);
    window.draw(restartText);
}

bool GameOver::checkForRestart(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
        return true;
    }
    return false;
}
