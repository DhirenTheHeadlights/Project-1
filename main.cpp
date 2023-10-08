#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "World.h"
#include "Globals.h"


int main() { 
    initializeGlobals();
    std::srand(std::time(0));
    sf::Font font;
    sf::Text text;
    World world;

    map.grid(10000, 10000, 10);
    
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Close window on Esc key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        window.clear();
        text.setString("Speed: ");
        window.draw(text);
        window.display();
    }

    return 0;
}
