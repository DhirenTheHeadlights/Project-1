#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.h"
#include <cmath>

class Circle {
public:
    Circle(float radius);
    void move(double radius, double moveSpeed, Map& map, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void setCircleSize(float newSize);
    void setPosition(float x, float y);
    float getCircleSize() const;
    sf::Vector2f getPosition() const;
    double getCurrentSpeed() const;
private:
    sf::CircleShape shape;
    float circlesize;
    float x = 0, y = 0;
    double currentSpeed = 1.0;
    int lastDirection = 0;
    sf::Clock deltaTime;
    void direction(float dx, float dy, float elapsed, Map& map);
};


