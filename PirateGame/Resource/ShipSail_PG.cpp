#include "ShipSail_PG.h"

using namespace PirateGame;

void Sail::updateSail(const sf::Sprite& shipSprite, const sf::Vector2f shipDirection, const float maxRotationOffset) {
	// Calculate the sail's position based on the ship's rotation
	float rotation = shipSprite.getRotation();
	sf::Transform rotationTransform;
	rotationTransform.rotate(rotation, shipSprite.getPosition());

	sf::Vector2f rotationPoint(shipSprite.getPosition() + offset);
	sf::Vector2f sailPosition = rotationTransform.transformPoint(rotationPoint);
	sailSprite.setPosition(sailPosition);

	// Find the rotation of the sail with respect to the ship
	float currRotationOffset = calculateAngleRelativeToShip(shipDirection);
	currRotationOffset = vm::normalizeAngle(currRotationOffset, -180.f, 180.f); // Ensure angle is within -180 to 180 range

	// Dynamically adjust rotationOffset to approach currRotationOffset within max limits
	if (currRotationOffset > maxRotationOffset) {
		rotationOffset = maxRotationOffset;
	}
	else if (currRotationOffset < -maxRotationOffset) {
		rotationOffset = -maxRotationOffset;
	}

	// Apply new rotation
	sailSprite.setRotation(rotation + rotationOffset);
}

float Sail::calculateAngleRelativeToShip(const sf::Vector2f& shipDirection) const {
	// Normalize the ship direction
	sf::Vector2f normShipDirection = vm::normalize(shipDirection);
	sf::Vector2f normSailDirection = vm::normalize(getDirectionVector());

	float ShipAngle = vm::vectorToAngleDegrees(normShipDirection);
	float SailAngle = vm::vectorToAngleDegrees(normSailDirection);
	return SailAngle - ShipAngle; // Sail angle relative to the ship
}

void Sail::updateSailLeftRightAutomatically(const sf::Vector2f& windDirection, const sf::Vector2f& shipDirection, float rotationSpeed) {
    // Normalize the ship and wind directions
    sf::Vector2f normShipDirection = vm::normalize(shipDirection);
    sf::Vector2f normWindDirection = vm::normalize(windDirection);

    // Convert directions to angles with respect to the global coordinate system
    float shipAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(normShipDirection));
    float windAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(normWindDirection));

    // Calculate wind angle relative to the ship
    float relativeWindAngle = windAngle - shipAngle;

    // Current sail angle relative to the ship
    float currentSailAngle = calculateAngleRelativeToShip(shipDirection);
    currentSailAngle = vm::normalizeAngle(currentSailAngle);

    // Calculate angle difference and normalize
    float angleDiff = vm::normalizeAngle(relativeWindAngle - currentSailAngle);
	if (angleDiff > 180.f) { // If the angle difference is greater than 180, subtract 360
		angleDiff -= 360.f;  // This is to ensure the sail rotates in the fastest direction
	}

    // Find the fastest direction to rotate the sail
	if (angleDiff > 0) {
		rotationOffset += rotationSpeed;
	}
	else if (angleDiff < 0) {
		rotationOffset -= rotationSpeed;
	}
}
