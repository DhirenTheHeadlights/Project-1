#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.h"
#include <cmath>

class Circle {
public:
    Circle(float radius);
    void move(double radius, double moveSpeed, double MaxSpeed, Map& map, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    float getCircleSize() const;
    sf::Vector2f getPosition() const;
    double getCurrentSpeed() const;
    void setCircleSize(float newSize);
private:
    sf::CircleShape shape;
    float circlesize;
    float x = 0, y = 0;
    double currentSpeed = 1.0;
    int lastDirection = 0;
    sf::Clock deltaTime;
    void direction(float dx, float dy, double MaxSpeed, float elapsed, Map& map);
    double acceleration(double MaxSpeed);
};


