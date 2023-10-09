#include <iostream>
#include "Globals.h"
#include "World.h"


int main() { 
    initializeGlobals();
    World world(map, window);
    Hashmap hashmap(map, window);
    

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
        window.draw(text);
        world.createWorld(window, hashmap, map);
        text.setString("Size: " + std::to_string(world.getCircleSize()));
        window.display();
    }

    return 0;
}
