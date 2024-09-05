#include "ShipSail_PG.h"

using namespace PirateGame;

void Sail::updateSail(const sf::Sprite& shipSprite, const sf::Vector2f& shipDirection, const float maxRotationOffset) {
	// Calculate the sail's position based on the ship's rotation
	sailSprite.setPosition(vm::relativeRotationTransformedPosition(shipSprite.getPosition(), offset, shipSprite.getRotation()));

	// Find the rotation of the sail with respect to the ship
	float currRotationOffset = sailSprite.getRotation() - shipSprite.getRotation();
	currRotationOffset = vm::normalizeAngle(currRotationOffset, -180.f, 180.f); // Ensure angle is within -180 to 180 range

	// Dynamically adjust rotationOffset to approach currRotationOffset within max limits
	rotationOffset = std::clamp(currRotationOffset, -maxRotationOffset, maxRotationOffset);

	// Apply new rotation
	sailSprite.setRotation(shipSprite.getRotation() + rotationOffset);
}

void Sail::updateSailLeftRightAutomatically(const sf::Vector2f& windDirection, const float shipRotation, float rotationSpeed) {
    // Find the angle of the wind direction relative to the ship
	sf::Vector2f normWindDirection = vm::normalize(windDirection);
    float windAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(normWindDirection));
	float relativeWindAngle = windAngle - shipRotation;

    // Current sail angle relative to the ship
    float currentSailAngle = vm::normalizeAngle(sailSprite.getRotation() - shipRotation);

    // Calculate angle difference and normalize
    float angleDiff = vm::normalizeAngle(relativeWindAngle - currentSailAngle);
	if (angleDiff > 180.f) { // If the angle difference is greater than 180, subtract 360
		angleDiff -= 360.f;  // This is to ensure the sail rotates in the fastest direction
	}

    // Find the fastest direction to rotate the sail
	if (angleDiff > 0) {
		rotationOffset += rotationSpeed * 10;
	}
	else if (angleDiff < 0) {
		rotationOffset -= rotationSpeed * 10;
	}
}
