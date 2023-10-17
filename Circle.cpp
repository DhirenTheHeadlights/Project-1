#include "Circle.h"


Circle::Circle(float radius) : shape(radius), circlesize(radius) { // Initiallizes the circle with a radius
    shape.setFillColor(sf::Color::Blue);
}

void Circle::move(double radius, double moveSpeed, double MaxSpeed, Map& map, sf::RenderWindow& window) {
    float elapsed = deltaTime.restart().asSeconds();

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f viewPos = window.mapPixelToCoords(pixelPos);
    sf::Vector2f dirToMouse = viewPos - (shape.getPosition() + sf::Vector2f(circlesize, circlesize));

    if (dirToMouse.x == 0 && dirToMouse.y == 0) return; // Return if the mouse is already on the circle.
    float length = sqrt(dirToMouse.x * dirToMouse.x + dirToMouse.y * dirToMouse.y);
    dirToMouse.x /= length;
    dirToMouse.y /= length;
    direction(dirToMouse.x * static_cast<float>(moveSpeed), dirToMouse.y * static_cast<float>(moveSpeed), MaxSpeed, elapsed, map);
}

void Circle::direction(float dx, float dy, double MaxSpeed, float elapsed, Map& map) {
    x += dx * static_cast<float>(acceleration(MaxSpeed)) * elapsed;
    y += dy * static_cast<float>(acceleration(MaxSpeed)) * elapsed;
    // Boundary checks
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > map.getLength() - 2 * circlesize) x = map.getLength() - 2 * circlesize;
    if (y > map.getLength() - 2 * circlesize) y = map.getLength() - 2 * circlesize;
    shape.setPosition(x - circlesize, y - circlesize);
}


double Circle::acceleration(double MaxSpeed) { // Adds to speed you keep moving
    double maxSpeed = MaxSpeed * 1 / (0.25 * getCircleSize());
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

void Circle::setPosition(float x, float y) {
	shape.setPosition(x, y); // Set the position
}

float Circle::getCircleSize() const {
    return circlesize; // Return the size
}

sf::Vector2f Circle::getPosition() const {
    return shape.getPosition(); // Get the position
}


