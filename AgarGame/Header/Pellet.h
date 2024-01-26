#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Pellet {
public:
    Pellet(float x, float y);
    sf::Vector2f getPosition() const;
    int getRadius() const;
    void draw(sf::RenderWindow& window);
    bool deActivate();
    bool isActive() const;
private:
    sf::CircleShape shape;
    int radius = std::rand() % 5 + 2;
    bool active = true;
    static const std::vector<sf::Color> colors;
};