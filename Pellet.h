#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class Pellet {
public:
    Pellet(float x, float y);  // constructor to initialize position
    void draw(sf::RenderWindow& window);  // method to draw the pellet on the screen
    void RemovePellet(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    int getRadius() const;
private:
    sf::CircleShape shape;
    int radius = 5;
    double x;
    double y;
};