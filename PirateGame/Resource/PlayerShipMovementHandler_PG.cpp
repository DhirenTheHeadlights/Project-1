#include "PlayerShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship
void PlayerShipMovementHandler::move(float baseSpeed, sf::Vector2f sailDirection) {
	setBaseSpeed(baseSpeed);

	float elapsed = getDeltaTime().restart().asSeconds();

	// Calculate the direction based on the ship's current rotation
	float rotationInRadians = vm::degreesToRadians(getSprite().getRotation() - 90.f); // Subtract 90 degrees to align with SFML's rotation
	sf::Vector2f direction(std::cos(rotationInRadians), std::sin(rotationInRadians));

	// Update the velocity based on the direction
	getSprite().setPosition(getSprite().getPosition() + updateVelocity(direction, elapsed, baseSpeed, sailDirection));
	setSpriteRotation();
}


void PlayerShipMovementHandler::setSpriteRotation() {
	if (getIsColliding() || getStopShipRotationFlag()) return;

	// Calculate the direction to the mouse
	sf::Vector2f mousePosition = GlobalValues::getInstance().getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*GlobalValues::getInstance().getWindow()));
	sf::Vector2f directionToMouse = vm::normalize(mousePosition - getSprite().getPosition());

	// Rotate the sprite using conversion from vector to angle with atan2 and normalize the angle
	float targetAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(directionToMouse) + 90.f);

	// Calculate the difference between the target and current angle
	float angleDifference = vm::normalizeAngle(targetAngle - getSprite().getRotation(), -180.f, 180.f);

	// Calculate the extra rotational acceleration based on the angle difference
	// Also, the accel is based on the speed of the ship
	float accel = abs(10 * angleDifference / 180.f * getSpeed() / getBaseSpeed());

	// Limit the turning speed
	angleDifference = std::clamp(angleDifference, -turningSpeed, turningSpeed);

	// Set the new rotation
	getSprite().rotate(accel * angleDifference);
}
