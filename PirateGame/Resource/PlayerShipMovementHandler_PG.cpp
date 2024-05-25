#include "PlayerShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship
void PlayerShipMovementHandler::update(float baseSpeed, sf::Vector2f sailDirection) {
	setBaseSpeed(baseSpeed);

	move(baseSpeed, sailDirection);
}


void PlayerShipMovementHandler::setSpriteRotation() {
	if (getIsColliding() || getStopShipRotationFlag()) return;

	// Calculate the direction to the mouse
	sf::Vector2f mousePosition = GlobalValues::getInstance().getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*GlobalValues::getInstance().getWindow()));
	sf::Vector2f directionToMouse = vm::normalize(mousePosition - getSprite().getPosition());

	// Rotate the sprite using conversion from vector to angle with atan2 and normalize the angle
	float targetAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(directionToMouse) + 90.f);

	rotateTowards(targetAngle);
}
