#include "Target.h"

Target::Target(sf::Vector2f position, float radius) {
	this->position = position;
	shape.setRadius(radius);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(position);
}

void Target::setPos(sf::Vector2f position) {
	this->position = position;
	shape.setPosition(position);
}

sf::Vector2f Target::getPos() {
	return position;
}

sf::CircleShape Target::getShape() {
	return shape;
}

bool Target::isActive() {
	return activeStatus;
}

void Target::deActivate() {
	activeStatus = false;
}