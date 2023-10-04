#pragma once
#include <SFML/Graphics.hpp>

class Pellet {
public:
    Pellet(float x, float y);  // constructor to initialize position
    void draw(sf::RenderWindow& window);  // method to draw the pellet on the screen
    void RemovePellet(Pellet pellet);
    sf::Vector2f getPosition() const;
    int getRadius() const;
private:
    sf::CircleShape shape;
    int radius = 5;
    double x;
    double y;
};