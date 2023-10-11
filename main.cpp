#include <iostream>
#include "Globals.h"
#include "World.h"


int main() { 
    initializeGlobals();
    World world(map, window);

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
        world.createWorld(window, map);
        window.display();
    }
    return 0;
}
