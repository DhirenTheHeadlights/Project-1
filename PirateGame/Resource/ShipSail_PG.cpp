#include "ShipSail_PG.h"

using namespace PirateGame;

void Sail::updateSail(sf::Sprite& shipSprite) {
	//Calculate the sail's position based on the ship's rotation
	float rotation = shipSprite.getRotation();
	sf::Transform rotationTransform;
	rotationTransform.rotate(rotation, shipSprite.getPosition());

	sf::Vector2f rotationPoint(shipSprite.getPosition() + offset);
	sf::Vector2f sailPosition = rotationTransform.transformPoint(rotationPoint);
	sailSprite.setPosition(sailPosition);

	// Rotate the sail based on the ship's rotation and the sail's rotation offset, but cap it to the max rotation offset
	sailSprite.setRotation(rotation + ((std::fabs(rotationOffset) > maxRotationOffset) ? maxRotationOffset : rotationOffset));
}

void Sail::updateSailLeftRight(sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey) {
	// If the key is pressed, update the sail rotation offset
	if (GlobalInputHandler::getInstance().isKeyHeld(leftKey)) {
		rotationOffset += 0.1f;
	}
	else if (GlobalInputHandler::getInstance().isKeyHeld(rightKey)) {
		rotationOffset -= 0.1f;
	}
}

void Sail::updateSailUpDown(sf::Keyboard::Key upKey, sf::Keyboard::Key downKey) {
	// TO DO: Implement the up and down movement of the sails
}