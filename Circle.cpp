#include "Circle.h"


Circle::Circle(float radius) : shape(radius), circlesize(radius) { // Initiallizes the circle with a radius
    float x = 0;
    float y = 0;
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Blue);
}

void Circle::move(double radius, double moveSpeed, double MaxSpeed, sf::RenderWindow& window) {
    float elapsed = deltaTime.restart().asSeconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && y > 0)
        Circle::direction(0, -moveSpeed, MaxSpeed, window, elapsed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && y < window.getSize().y - circlesize)
        Circle::direction(0, moveSpeed, MaxSpeed, window, elapsed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && x > 0)
        Circle::direction(-moveSpeed, 0, MaxSpeed, window, elapsed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && x < window.getSize().x - circlesize)
        Circle::direction(moveSpeed, 0, MaxSpeed, window, elapsed);
}

void Circle::direction(float dx, float dy, double MaxSpeed, sf::RenderWindow& window, float elapsed) {
    int currentDirection = (dx != 0) ? (dx > 0 ? 1 : -1) : (dy > 0 ? 2 : -2);  // 1:right, -1:left, 2:down, -2:up
    if (currentDirection + lastDirection == 0) {  // Direction changed
        currentSpeed = 1.0;
    }
    lastDirection = currentDirection;
    x += dx * acceleration(MaxSpeed) * elapsed;
    y += dy * acceleration(MaxSpeed) * elapsed;
    // Boundary checks so circle does not go out of bounds
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > window.getSize().x - 2 * circlesize) x = window.getSize().x - 2 * circlesize;
    if (y > window.getSize().y - 2 * circlesize) y = window.getSize().y - 2 * circlesize;
    shape.setPosition(x, y);
}

double Circle::acceleration(double MaxSpeed) { // Adds to speed you keep moving
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (currentSpeed < MaxSpeed) {
            currentSpeed += 0.001;
        }
        deltaTime.restart();
    }
    else if (deltaTime.getElapsedTime().asSeconds() >= 1) { // If you hold still for a second, the circle stops moving.
        currentSpeed = 1.0;
    }
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


