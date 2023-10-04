#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "Circle.h"
#include "World.h"
#include "Globals.h"
#include "Hashmap.h"


int main() { 
    initializeGlobals();
    std::srand(std::time(0));

    Circle circle(30.f);
    sf::Font font;
    sf::Text text;
    World world;
    Map map;
    Hashmap hashmap;

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
        
        circle.move(circle.getCirclesize(), 0.01, 2);

        // Close window on Esc key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        window.clear();
        circle.draw(window);
        text.setString("Speed: " + std::to_string(circle.getCurrentSpeed()));
        window.draw(text);
        world.drawPellets(window, hashmap);
        hashmap.assignCircle(circle);
        window.display();
    }

    return 0;
}
