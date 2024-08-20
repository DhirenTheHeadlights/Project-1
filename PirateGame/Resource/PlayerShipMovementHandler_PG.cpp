#include "PlayerShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship
void PlayerShipMovementHandler::update(const sf::Vector2f& sailDirection, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed) {
	move(sailDirection, dt, windDirection, windSpeed);
}


void PlayerShipMovementHandler::setSpriteRotation() {
	if (getIsColliding() || getStopShipRotationFlag()) return;

	// Calculate the direction to the mouse
	sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f directionToMouse = vm::normalize(mousePosition - sprite.getPosition());

	// Rotate the sprite using conversion from vector to angle with atan2 and normalize the angle
	rotateTowards(vm::normalizeAngle(vm::vectorToAngleDegrees(directionToMouse) + 90.f));
}
