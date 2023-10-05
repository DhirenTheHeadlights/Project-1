#include "Circle.h"
#include "World.h"


Circle::Circle(float radius) : shape(radius), circlesize(radius) { // Initiallizes the circle with a radius
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Blue);
}

void Circle::move(double radius, double moveSpeed, double MaxSpeed) { // Movement Controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && radius > 0) Circle::direction(0, -moveSpeed, MaxSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && radius < SCREEN_HEIGHT) Circle::direction(0, moveSpeed, MaxSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && radius > 0) Circle::direction(-moveSpeed, 0, MaxSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && radius < SCREEN_WIDTH) Circle::direction(moveSpeed, 0, MaxSpeed);
}

void Circle::direction(float dx, float dy, double MaxSpeed) {
    int currentDirection = (dx != 0) ? (dx > 0 ? 1 : -1) : (dy > 0 ? 2 : -2);  // 1:right, -1:left, 2:down, -2:up
    if (currentDirection + lastDirection == 0) {  // Direction changed
        currentSpeed = 1.0;
    }
    lastDirection = currentDirection;

    x += dx * acceleration(MaxSpeed);
    y += dy * acceleration(MaxSpeed);
    // Boundary checks so circle does not go out of bounds
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > SCREEN_WIDTH - 2 * circlesize) x = SCREEN_WIDTH - 2 * circlesize;
    if (y > SCREEN_HEIGHT - 2 * circlesize) y = SCREEN_HEIGHT - 2 * circlesize;
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

float Circle::getCirclesize() const {
    return circlesize; // Return the size
}

sf::Vector2f Circle::getPosition() const {
    return shape.getPosition(); // Get the position
}


