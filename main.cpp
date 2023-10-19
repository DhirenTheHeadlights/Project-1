#include <iostream>
#include "Globals.h"
#include "World.h"

int main() { 
    initializeGlobals();
    World world(window);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        window.clear();    
        world.createWorld(window, event);
        if (world.isGameOver() == true && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) world.restart();
        window.display();
    }
    return 0;
}
