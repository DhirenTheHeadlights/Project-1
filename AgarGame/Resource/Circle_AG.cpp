#include "Circle_AG.h"


Circle::Circle(float radius) : shape(radius), circlesize(radius) { // Initiallizes the circle with a radius
    shape.setFillColor(sf::Color::Blue);
}

void Circle::move(double moveSpeed, sf::Vector2f dir, Map& map, sf::RenderWindow& window) {
    dirToMouse = dir;
    float elapsed = deltaTime.restart().asSeconds();
    if (dirToMouse.x == 0 && dirToMouse.y == 0) return; // Return if the mouse is already on the circle.
    float length = sqrt(dirToMouse.x * dirToMouse.x + dirToMouse.y * dirToMouse.y);
    dirToMouse.x /= length;
    dirToMouse.y /= length;
    direction(dirToMouse.x * static_cast<float>(moveSpeed), dirToMouse.y * static_cast<float>(moveSpeed), elapsed, map);
}

void Circle::direction(float dx, float dy, float elapsed, Map& map) {
    x += dx * elapsed; //* static_cast<float>(1 / circlesize);
    y += dy * elapsed; //* static_cast<float>(1 / circlesize);
    // Boundary checks
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > map.getLength() - 2 * circlesize) x = map.getLength() - 2 * circlesize;
    if (y > map.getLength() - 2 * circlesize) y = map.getLength() - 2 * circlesize;
    shape.setPosition(x - circlesize, y - circlesize);
}

void Circle::draw(sf::RenderWindow& window) const {
    window.draw(shape); // Draw the shape
    //std::cout << "Drawing circle at: " << x << ", " << y << std::endl;
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
    return shape.getPosition() + sf::Vector2f(circlesize, circlesize);
}

sf::Vector2f Circle::getDirection() const {
    return dirToMouse;
}

bool Circle::operator==(const Circle& other) const {
    return this->getPosition() == other.getPosition() && this->circlesize == other.circlesize;
}

bool Circle::checkCollision(const Circle& other) const {
    float dx = other.getPosition().x - this->getPosition().x;
    float dy = other.getPosition().y - this->getPosition().y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (this->getCircleSize() + other.getCircleSize());
}

void Circle::setColor(sf::Color color) {
	shape.setFillColor(color);
}

