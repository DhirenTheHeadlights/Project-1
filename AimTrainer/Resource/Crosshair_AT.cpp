#include "Crosshair_AT.h"
#include <iostream>

Crosshair::Crosshair(sf::RenderWindow& window) : window(window) {
}

// Draw the crosshair
void Crosshair::draw(bool visible) {
	if (visible) {
		window.setMouseCursorVisible(false);
		setPosition();
		window.draw(horizontal);
		window.draw(vertical);
	}
	else {
		window.setMouseCursorVisible(true);

	}
}

// Set the position of the crosshair
void Crosshair::setPosition() {
	mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
	horizontal.setSize(sf::Vector2f(length, thickness));
	horizontal.setFillColor(colour);
	horizontal.setPosition(mousePos.x - length / 2, mousePos.y);
	vertical.setSize(sf::Vector2f(thickness, height));
	vertical.setFillColor(colour);
	vertical.setPosition(mousePos.x, mousePos.y - height / 2);
}

// Set the length of the crosshair
void Crosshair::setLength(float length) {
	this->length = length;
}

// Set the height of the crosshair
void Crosshair::setHeight(float height) {
	this->height = height;
}

// Set the thickness of the crosshair
void Crosshair::setThickness(float thickness) {
	this->thickness = thickness;
}

// Set the colour of the crosshair
void Crosshair::setColour(sf::Color colour) {
	this->colour = colour;
}