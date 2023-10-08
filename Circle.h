#pragma once
#include <SFML/Graphics.hpp>

class Circle {
public:
    Circle(float radius);
    void move(double radius, double moveSpeed, double MaxSpeed, sf::RenderWindow& window);
    void direction(float dx, float dy, double MaxSpeed, sf::RenderWindow& window);
    double acceleration(double MaxSpeed);
    void draw(sf::RenderWindow& window);
    float getCirclesize() const;
    double getCurrentSpeed() const;
    sf::Vector2f getPosition() const;
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


