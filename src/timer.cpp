    #include "timer.hpp"


    Timer::Timer()
        : gameplayDuration(0.0f),
          displayTimer(false)
    {
        if (!font.loadFromFile("assets/fonts/Orbitron-Bold.ttf")) {
            std::cerr << "Failed to load font" << std::endl;
        }
        
        timerText.setFont(font);
        timerText.setCharacterSize(24); // choose appropriate size
        timerText.setFillColor(sf::Color::White);
    }

    void Timer::reset() {
        gameplayDuration = 0.0f;
    }

    void Timer::update(float elapsedSeconds) {
        gameplayDuration += elapsedSeconds;

        int minutes = static_cast<int>(gameplayDuration) / 60;
        int seconds = static_cast<int>(gameplayDuration) % 60;

        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
        timerText.setString(ss.str());

        // Position it at the top-center
        sf::FloatRect textRect = timerText.getLocalBounds();
        timerText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top);
        timerText.setPosition(sf::Vector2f(256, 10)); // Assuming a window width of 800, adjust as per your needs
    }


    void Timer::draw(sf::RenderWindow& window) {
        if(displayTimer) {
            window.draw(timerText);
        }
    }

    void Timer::show() { displayTimer = true; }
    void Timer::hide() { displayTimer = false; }

