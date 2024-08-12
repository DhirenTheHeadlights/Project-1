#include "PlayerShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship
void PlayerShipMovementHandler::update(float baseSpeed, sf::Vector2f sailDirection, float dt, sf::Vector2f windDirection, float windSpeed) {
	setBaseSpeed(baseSpeed);

	move(baseSpeed, sailDirection, dt, windDirection, windSpeed);
}


void PlayerShipMovementHandler::setSpriteRotation() {
	if (getIsColliding() || getStopShipRotationFlag()) return;

	// Calculate the direction to the mouse
	sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f directionToMouse = vm::normalize(mousePosition - getSprite().getPosition());

	// Rotate the sprite using conversion from vector to angle with atan2 and normalize the angle
	float targetAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(directionToMouse) + 90.f);

	rotateTowards(targetAngle);
}
