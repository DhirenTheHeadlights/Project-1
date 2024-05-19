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
	else {
		travelDirection = vm::normalize(travelDirection);
	}

	//Check for landmass collision
	travelDirection = deflectTravelDirection(getNearbyLandmasses(), deflectionDistanceLandmass, travelDirection);
	travelDirection = deflectTravelDirection(getNearbyShips(), deflectionDistanceShip, travelDirection);

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

sf::Vector2f EnemyShipMovementHandler::deflectTravelDirection(std::vector<sf::Sprite>& sprites, float deflectionDistance, sf::Vector2f travelDirection) {
	for (auto& sprite : sprites) {
		// Sprite X is within vector X
		sf::Vector2f containVector = (position + sf::Vector2f(travelDirection.x * deflectionDistance, travelDirection.y * deflectionDistance));
		if (abs(containVector.x) - abs(sprite.getPosition().x) > 0.f && sprite.getPosition() != destination) {
			// Sprite Y is within vector Y, indicating it is within the vector
			if (abs(containVector.y) - abs(sprite.getPosition().y) > 0.f) {
				sf::FloatRect rect = sprite.getGlobalBounds();
				// Calculate new width and heightt
				float newWidth = rect.width * islandDeflectionPaddingScale;
				float newHeight = rect.height * islandDeflectionPaddingScale;

				// Calculate new position to keep the rectangle centered
				float newLeft = rect.left - (newWidth - rect.width) / 2;
				float newTop = rect.top - (newHeight - rect.height) / 2;

				sf::FloatRect deflectionBounds = sf::FloatRect(newLeft, newTop, newWidth, newHeight);

				std::vector<sf::Vector2f> corners{
					sf::Vector2f(deflectionBounds.left, deflectionBounds.top),
					sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top),
					sf::Vector2f(deflectionBounds.left, deflectionBounds.top + deflectionBounds.height),
					sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top + deflectionBounds.height)
				};

				float cornerToShip = 10000;
				sf::Vector2f closestCorner;
				for (auto& corner : corners) {
					float nextCornerToShip = vm::distance(position, corner);
					if (nextCornerToShip < cornerToShip) {
						cornerToShip = nextCornerToShip;
						closestCorner = corner;
					}
				}

				travelDirection = closestCorner;
			}
		}
	}
	return travelDirection;
}