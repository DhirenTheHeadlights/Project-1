#include "Circle.h"


Circle::Circle(float radius) : shape(radius), circlesize(radius) { // Initiallizes the circle with a radius
    float x = 0;
    float y = 0;
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Blue);
}

void Circle::move(double radius, double moveSpeed, double MaxSpeed, sf::RenderWindow& window) {
    float elapsed = deltaTime.restart().asSeconds();
    sf::Vector2f dirToMouse = sf::Vector2f(sf::Mouse::getPosition(window)) - shape.getPosition();
    if (dirToMouse.x == 0 && dirToMouse.y == 0) return; // Return if the mouse is already on the circle.
    float length = sqrt(dirToMouse.x * dirToMouse.x + dirToMouse.y * dirToMouse.y);
    dirToMouse.x /= length;
    dirToMouse.y /= length;
    direction(dirToMouse.x * moveSpeed, dirToMouse.y * moveSpeed, MaxSpeed, window, elapsed);
}

void Circle::direction(float dx, float dy, double MaxSpeed, sf::RenderWindow& window, float elapsed) {
    x += dx * acceleration(MaxSpeed) * elapsed;
    y += dy * acceleration(MaxSpeed) * elapsed;
    // Boundary checks
    if (x < circlesize) x = circlesize;
    if (y < circlesize) y = circlesize;
    if (x > window.getSize().x - circlesize) x = window.getSize().x - circlesize;
    if (y > window.getSize().y - circlesize) y = window.getSize().y - circlesize;
    shape.setPosition(x, y);
}


double Circle::acceleration(double MaxSpeed) { // Adds to speed you keep moving
    double maxSpeed = MaxSpeed * 1 / (0.25 * getCirclesize());
    if (currentSpeed < maxSpeed) currentSpeed += 0.001;
    deltaTime.restart();
    return currentSpeed;
}

double Circle::getCurrentSpeed() const {
    return currentSpeed;  // Return the current speed value
}

void Circle::draw(sf::RenderWindow& window) {
    window.draw(shape); // Draw the shape
}

void Circle::setCircleSize(float newSize) {
    circlesize = newSize;
    shape.setRadius(newSize); 
}

float Circle::getCirclesize() const {
    return circlesize; // Return the size
}

sf::Vector2f Circle::getPosition() const {
    return shape.getPosition(); // Get the position
}


