#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.h"
#include <cmath>

class Circle {
public:
    Circle(float radius);
    void move(double moveSpeed, sf::Vector2f dir, Map& map, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void setCircleSize(float newSize);
    void setPosition(float x, float y);
    float getCircleSize() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getDirection() const;
    bool operator==(const Circle& other) const;
    bool checkCollision(const Circle& other) const;
    void setColor(sf::Color color);
private:
    sf::CircleShape shape;
    float circlesize;
    float x = 0, y = 0;
    sf::Clock deltaTime;
    sf::Vector2f dirToMouse;
    void direction(float dx, float dy, float elapsed, Map& map);
};


