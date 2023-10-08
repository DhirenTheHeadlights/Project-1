#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "Globals.h"
#include "World.h"


int main() { 
    initializeGlobals();
    World world(map, window);
    Hashmap hashmap(map, window);
    std::srand(std::time(0));

    sf::Font font;
    sf::Text text;
    

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        window.clear();
        text.setString("Speed: ");
        
        window.draw(text);
        world.drawCircle(window, hashmap, map);
        world.drawPellets(window, map, hashmap);
        window.display();
    }

    return 0;
}
