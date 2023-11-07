#include "Room.h"

// Default constructor
Room::Room() {
    // Set default size
    this->roomShape.setSize(sf::Vector2f(100.f, 100.f)); // Just as an example
    this->roomShape.setFillColor(sf::Color::White);
    this->roomShape.setOutlineColor(sf::Color::Black);
    this->roomShape.setOutlineThickness(5.f);
}

// Constructor with parameters
Room::Room(float width, float height) {
    this->roomShape.setSize(sf::Vector2f(width, height));
    this->roomShape.setFillColor(sf::Color::White);
    this->roomShape.setOutlineColor(sf::Color::Black);
    this->roomShape.setOutlineThickness(5.f);
}

// Other member functions...
void Room::setPosition(float x, float y) {
	this->roomShape.setPosition(x, y);
}

void Room::draw(sf::RenderWindow& window) {
	window.draw(this->roomShape);
}

sf::FloatRect Room::getBounds() const {
    return this->roomShape.getGlobalBounds();
}


