#include "EnemyShipMovementHandler_PG.h"


using namespace PirateGame;

void EnemyShipMovementHandler::move(float baseSpeed, sf::Vector2f sailDirection) {
	setBaseSpeed(baseSpeed * enemySpeedMultiplier);

	setInitialPosition();

	float elapsed = getDeltaTime().restart().asSeconds();

	// Calculate the direction based on the ship's current rotation
	float rotationInRadians = (getSprite().getRotation() - 90.f) * pi / 180.f; // Subtract 90 degrees to align with SFML's rotation
	sf::Vector2f direction(std::cos(rotationInRadians), std::sin(rotationInRadians));

	updateVelocity(direction, elapsed, baseSpeed, sailDirection);
	setSpriteRotation();

	// Set the new position
	getSprite().setPosition(position);
}

void EnemyShipMovementHandler::setSpriteRotation() {
	if (getStopShipRotationFlag()) return;

	// Grab window
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

	// Calculate the direction to the target. Use playerPos if active towards target, else use destination.
	sf::Vector2f travelDirection = (isActiveTowardsTarget ? targetPos : destination) - position;
	float distance = vm::magnitude(travelDirection);

	if (isActiveTowardsTarget) {
		if (distance < broadsideDistance) {
			travelDirection = vm::normalize(sf::Vector2f(travelDirection.y, -travelDirection.x));
		}
		else {
			travelDirection = vm::normalize(travelDirection);
		}
	}

	// If not active towards player, head directly towards destination
	else if (!hasSetDeflectionDirectionEnemyShip && !hasSetDeflectionDirectionLandmass) {
		travelDirection = vm::normalize(travelDirection);
	}

	// Check for landmass and enemy ship collisions and adjust travel direction
	if (!hasSetDeflectionDirectionLandmass) {
		auto deflectionPair = deflectTravelDirection(getNearbyLandmasses(), deflectionDistanceLandmass, travelDirection);
		if (deflectionPair.second != travelDirection) {
			closestLandmass = deflectionPair.first;
			travelDirection = deflectionPair.second;
			hasSetDeflectionDirectionLandmass = true;
		}
	}

	if (!hasSetDeflectionDirectionEnemyShip) {
		auto deflectionPair = deflectTravelDirection(getNearbyShips(), deflectionDistanceShip, travelDirection);
		if (deflectionPair.second != travelDirection) {
			closestEnemyShip = deflectionPair.first;
			travelDirection = deflectionPair.second;
			hasSetDeflectionDirectionEnemyShip = true;
		}
	}

	// Check if either the nearest landmass or enemy ship is far enough away to reset the deflection direction
	if (hasSetDeflectionDirectionLandmass && vm::distance(position, closestLandmass.getPosition()) > deflectionDistanceLandmass) {
		hasSetDeflectionDirectionLandmass = false; // Reset the deflection direction, will check again next frame
	}
	if (hasSetDeflectionDirectionEnemyShip && vm::distance(position, closestEnemyShip.getPosition()) > deflectionDistanceShip) {
		hasSetDeflectionDirectionEnemyShip = false; // Reset the deflection direction, will check again next frame
	}

	GlobalValues::getInstance().getWindow()->draw(vm::createVector(position, travelDirection * deflectionDistanceLandmass, sf::Color::Red));
	GlobalValues::getInstance().displayText("Dist to landmass: " + std::to_string(vm::distance(position, closestLandmass.getPosition())), position + sf::Vector2f(0, 50), sf::Color::White);
	GlobalValues::getInstance().displayText("Dist to enemy ship: " + std::to_string(vm::distance(position, closestEnemyShip.getPosition())), position + sf::Vector2f(0, 100), sf::Color::White);
	std::string hasSetDeflectionDirectionLandmassStr = hasSetDeflectionDirectionLandmass ? "true" : "false";
	std::string hasSetDeflectionDirectionEnemyShipStr = hasSetDeflectionDirectionEnemyShip ? "true" : "false";
	GlobalValues::getInstance().displayText(hasSetDeflectionDirectionLandmassStr, position + sf::Vector2f(0, 150), sf::Color::White);
	GlobalValues::getInstance().displayText(hasSetDeflectionDirectionEnemyShipStr, position + sf::Vector2f(0, 200), sf::Color::White);

	// Rotate the sprite using conversion from vector to angle with atan2
	float targetAngle = std::atan2(travelDirection.y, travelDirection.x) * 180.f / pi + 90.f;

	// Normalize the target angle to the range [0, 360]
	if (targetAngle < 0) targetAngle += 360;

	// Get the current angle
	float currentAngle = getSprite().getRotation();

	// Calculate the difference between the target and current angle
	float angleDifference = targetAngle - currentAngle;

	// Normalize the angle difference to the range [-180, 180]
	while (angleDifference < -180) angleDifference += 360;
	while (angleDifference > 180) angleDifference -= 360;

	// Calculate the extra rotational acceleration based on the angle difference
	// Also, the accel is based on the speed of the ship
	float accel = abs(10 * angleDifference / 180.f * getSpeed() / getBaseSpeed());
	float turningMultiplier = isActiveTowardsTarget ? 1.5f : 1.f;

	// Limit the turning speed
	angleDifference = std::clamp(angleDifference, -turningMultiplier * turningSpeed, turningMultiplier * turningSpeed);

	// Set the new rotation
	getSprite().setRotation(currentAngle + (accel * angleDifference));
}

std::pair<sf::Sprite, sf::Vector2f> EnemyShipMovementHandler::deflectTravelDirection(const std::vector<sf::Sprite>& sprites, float deflectionDistance, sf::Vector2f travelDirection) {
	sf::Sprite deflectionSprite;
	for (float i = 0; i < vm::magnitude(travelDirection * deflectionDistance); i += deflectionVectorCheckInterval) {
		sf::Vector2f checkPosition = position + travelDirection * i;
		for (const auto& sprite : sprites) {
			sf::FloatRect spriteBounds = sprite.getGlobalBounds();
			// Check if the point is within the sprite's bounds
			if (spriteBounds.contains(checkPosition)) {
				// Calculate the padded bounding box for deflection
				float newWidth = spriteBounds.width * islandDeflectionPaddingScale;
				float newHeight = spriteBounds.height * islandDeflectionPaddingScale;
				float newLeft = spriteBounds.left - (newWidth - spriteBounds.width) / 2;
				float newTop = spriteBounds.top - (newHeight - spriteBounds.height) / 2;
				sf::FloatRect deflectionBounds(newLeft, newTop, newWidth, newHeight);

				// Find the closest corner of the deflection bounds to the ship
				std::vector<sf::Vector2f> corners{
					sf::Vector2f(deflectionBounds.left, deflectionBounds.top),
					sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top),
					sf::Vector2f(deflectionBounds.left, deflectionBounds.top + deflectionBounds.height),
					sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top + deflectionBounds.height)
				};

				float closestDistance = std::numeric_limits<float>::max();
				sf::Vector2f closestCorner;

				for (const auto& corner : corners) {
					float distanceToCorner = vm::distance(position, corner);
					if (distanceToCorner < closestDistance) {
						closestDistance = distanceToCorner;
						closestCorner = corner;
					}
				}

				// Update travel direction to steer away from the obstacle
				sf::Vector2f deflectDirection = vm::normalize(position - closestCorner);
				travelDirection += deflectDirection; // Combine current direction with deflection direction
				travelDirection = vm::normalize(travelDirection); // Normalize the result
				deflectionSprite = sprite; // Set the deflection sprite
				break; // Exit the loop since we have found a collision
			}
		}
	}
	return std::make_pair(deflectionSprite, travelDirection);
}
