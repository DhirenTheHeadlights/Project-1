#pragma once
#include <SFML/Graphics.hpp>

class Pellet {
public:
    Pellet(float x, float y);  // constructor to initialize position
    void draw(sf::RenderWindow& window);  // method to draw the pellet on the screen
private:
    sf::CircleShape shape;
};