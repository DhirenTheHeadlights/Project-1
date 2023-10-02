#pragma once
#include <SFML/Graphics.hpp>

class Circle {
public:
    Circle(float radius);
    void move(double radius, double moveSpeed, double MaxSpeed);
    void direction(float dx, float dy, double MaxSpeed);
    double acceleration(double MaxSpeed);
    void draw(sf::RenderWindow& window);
    float getCirclesize() const;
    double getCurrentSpeed() const;
private:
    sf::CircleShape shape;
    sf::Clock accelClock;
    sf::Clock deltaTime;
    float x{ 0 };
    float y{ 0 };
    float circlesize;
    double currentSpeed = 1.0;
    int lastDirection = 0;
};


