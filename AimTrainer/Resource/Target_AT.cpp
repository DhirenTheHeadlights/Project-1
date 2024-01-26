#include "Target_AT.h"

// Constructor for the target
Target::Target(sf::Vector2f position, float radius) {
	this->position = position;
	shape.setRadius(radius);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(position);
}

// Destructor for the target
Target::~Target() {
	//std::cout << "Target destroyed" << std::endl;
}

// Draw the target
void Target::setPos(sf::Vector2f position) {
	this->position = position;
	shape.setPosition(position);
}

// Get the position of the target
sf::Vector2f Target::getPos() {
	return shape.getPosition() + sf::Vector2f(shape.getRadius(), shape.getRadius());
}

// Get the shape of the target
sf::CircleShape Target::getShape() {
	return shape;
}

// Get the radius of the target
bool Target::isActive() {
	return activeStatus;
}

// Set the radius of the target
void Target::deActivate() {
	activeStatus = false;
}