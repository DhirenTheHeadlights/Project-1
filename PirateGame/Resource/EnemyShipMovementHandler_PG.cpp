#include "EnemyShipMovementHandler_PG.h"


using namespace PirateGame;

void EnemyShipMovementHandler::update(float baseSpeed, sf::Vector2f sailDirection) {
	setBaseSpeed(baseSpeed * enemySpeedMultiplier);

	move(baseSpeed, sailDirection);
}

void EnemyShipMovementHandler::setSpriteRotation() {
	if (getStopShipRotationFlag()) return;

	// Determine the sprite rotation by determining the active movement vector
	sf::Vector2f directionToDestination = vm::normalize(destination - getSprite().getPosition());

	// Check for possible collisions
    sf::Vector2f direction; // Here, islands are the first priority and ships are the second
    direction = deflectTravelDirection(getNearbyLandmasses(), directionToDestination, deflectionDistanceLandmass, vm::randomFloat(islandDeflectionPaddingScaleMin, islandDeflectionPaddingScaleMax));
    if (direction == directionToDestination) direction = deflectTravelDirection(getNearbyShips(), directionToDestination, deflectionDistanceShip, shipDeflectionPaddingScale);

	// Check if the ship is active towards the target
	if (activeTowardsTarget) {
		float distance = vm::magnitude(destination - getSprite().getPosition());
		if (distance < broadsideDistance) { // Go perpendicular
			directionToDestination = vm::normalize(sf::Vector2f(directionToDestination.y, -directionToDestination.x));
		}
		else {
			directionToDestination = vm::normalize(directionToDestination);
		}
	}

    setTurningMultiplier(activeTowardsTarget ? 1.5f : 1.f);

	float targetAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(direction) + 90.f); // +90 to account for sprite rotation
    rotateTowards(targetAngle);
}

sf::Vector2f EnemyShipMovementHandler::deflectTravelDirection(const std::vector<sf::Sprite>& sprites, const sf::Vector2f travelDirection, const float deflectionDistance, const float deflectionPaddingScale) {
    // Grab all points in regular intervals along the travel direction
    for (float i = 0; i < vm::magnitude(travelDirection * deflectionDistance); i += deflectionVectorCheckInterval) {
        // Now, we will check if the point is within the bounds of any of the sprites
        sf::Vector2f point = getSprite().getPosition() + vm::normalize(travelDirection) * i;
        for (const auto& sprite : sprites) {
            // Get the sprite's bounds and create a padded bounding box for deflection
            sf::FloatRect spriteBounds = sprite.getGlobalBounds();
            float paddingWidth = spriteBounds.width * (deflectionPaddingScale - 1.f) / 2.f;
            float paddingHeight = spriteBounds.height * (deflectionPaddingScale - 1.f) / 2.f;

            sf::FloatRect deflectionBounds(
                spriteBounds.left - paddingWidth,
                spriteBounds.top - paddingHeight,
                spriteBounds.width + 2 * paddingWidth,
                spriteBounds.height + 2 * paddingHeight);

            // Check if the point is within the sprite's bounds
            if (deflectionBounds.contains(point)) {
                if (deflectionSprite.getGlobalBounds() != spriteBounds) {
                    deflectionSprite = sprite;
                    hasPickedFirstCorner = false;
                }
                return calculateDeflectionVector(deflectionBounds, travelDirection, deflectionDistance);
            }
        }
	}

    // Otherwise, return the travel direction
    hasPickedFirstCorner = false;
    return travelDirection;
}

sf::Vector2f EnemyShipMovementHandler::calculateDeflectionVector(const sf::FloatRect& deflectionBounds, const sf::Vector2f travelDirection, const float deflectionDistance) {
    // Create a vector of the corners of the deflection bounds
    std::vector<sf::Vector2f> corners {
        sf::Vector2f(deflectionBounds.left, deflectionBounds.top),
        sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top),
        sf::Vector2f(deflectionBounds.left, deflectionBounds.top + deflectionBounds.height),
        sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top + deflectionBounds.height)
    };

    // Remove a corner if the ship is clear of it. If it is the first corner, set hasPickedFirstCorner to true
    corners.erase(std::remove_if(corners.begin(), corners.end(), [&](const sf::Vector2f& corner) {
        if (vm::isInFront(getSprite().getPosition(), corner, vm::normalize(destination - getSprite().getPosition()))) {
            if (!hasPickedFirstCorner) {
                hasPickedFirstCorner = true;
            }
            return true;  // Remove corner if passed
        }
        return false;  // Keep corner otherwise
    }), corners.end());

    float closestDistance = std::numeric_limits<float>::max();

    // Find the farthest corner
    sf::Vector2f farthestCorner;
    float farthestDistance = 0.f;
    for (auto& corner : corners) {
        float distance = vm::distance(getSprite().getPosition(), corner);
        if (distance > farthestDistance) {
            farthestDistance = distance;
            farthestCorner = corner;
        }
    }

    for (const auto& corner : corners) {
        float distanceToCorner = vm::distance(getSprite().getPosition(), corner);
        if (distanceToCorner < closestDistance && !hasPickedFirstCorner) {
            closestDistance = distanceToCorner;
            deflectionTarget = corner;
        }

        // Otherwise, randomly select one of the remaining corners, but not the farthest one
        if (hasPickedFirstCorner) {
			if (vm::distance(destination, corner) < closestDistance && corner != farthestCorner) {
				closestDistance = distanceToCorner;
				deflectionTarget = corner;
			}
		}
    }

    // Update travel direction to steer towards the deflection target
    return vm::normalize(deflectionTarget - getSprite().getPosition());
}

