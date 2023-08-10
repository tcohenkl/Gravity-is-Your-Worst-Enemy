#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window (sf::VideoMode(512, 512),"Rocket Game", sf::Style::Titlebar | sf::Style::Close);
    
    // Player triangle
    sf::CircleShape player(80,3);
    player.setFillColor(sf::Color::Red);

    while (window.isOpen()){
        sf::Event evnt;
        
        while (window.pollEvent(evnt)){
            
            if (evnt.type == sf::Event::Closed)
                window.close();
        }

       // player movement 

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
            player.move(0.0f, -0.1f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
            player.move(-0.1f, 0.0f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
            player.move(0.0f, 0.1f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
            player.move(0.1f, 0.0f);
        }

        window.clear(); 
        window.draw(player);
        window.display();
    }

}
