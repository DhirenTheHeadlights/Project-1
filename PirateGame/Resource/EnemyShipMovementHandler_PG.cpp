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
	directionToDestination = deflectTravelDirection(getNearbyLandmasses(), directionToDestination, deflectionDistanceLandmass);
	directionToDestination = deflectTravelDirection(getNearbyShips(), directionToDestination, deflectionDistanceShip);

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

	GlobalValues::getInstance().getWindow()->draw(vm::createVector(getSprite().getPosition(), directionToDestination * deflectionDistanceLandmass, sf::Color::Red));
	GlobalValues::getInstance().displayText(getAimTowardsTarget() ? "True" : "False", getSprite().getPosition() + sf::Vector2f(0, 50), sf::Color::White);
	GlobalValues::getInstance().displayText("Destination: " + std::to_string(destination.x) + ", " + std::to_string(destination.y), getSprite().getPosition() + sf::Vector2f(0, 100), sf::Color::White);
	GlobalValues::getInstance().displayText("Position: " + std::to_string(getSprite().getPosition().x) + ", " + std::to_string(getSprite().getPosition().y), getSprite().getPosition() + sf::Vector2f(0, 150), sf::Color::White);

    setTurningMultiplier(activeTowardsTarget ? 1.5f : 1.f);

	float targetAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(directionToDestination) + 90.f); // +90 to account for sprite rotation
    rotateTowards(targetAngle);
}

sf::Vector2f EnemyShipMovementHandler::deflectTravelDirection(const std::vector<sf::Sprite>& sprites, sf::Vector2f travelDirection, float deflectionDistance) {
    // Check if there is a possible collision
    sf::Sprite deflectionSprite;
    for (float i = 0; i < vm::magnitude(travelDirection * deflectionDistance); i += deflectionVectorCheckInterval) {
        sf::Vector2f checkPosition = getSprite().getPosition() + travelDirection;
        for (const auto& sprite : sprites) {
            sf::FloatRect spriteBounds = sprite.getGlobalBounds();
            // Check if the point is within the sprite's bounds
            if (spriteBounds.contains(checkPosition)) {
                calculateDeflectionVector(sprite, travelDirection, deflectionDistance);
            }
        }
    }
    hasDeflectionTarget = false;
    return travelDirection;
}

sf::Vector2f EnemyShipMovementHandler::calculateDeflectionVector(const sf::Sprite& deflectionSprite, const sf::Vector2f travelDirection, const float deflectionDistance) {
    // Calculate the padded bounding box for deflection
    sf::FloatRect spriteBounds = deflectionSprite.getGlobalBounds();
    float newWidth = spriteBounds.width * islandDeflectionPaddingScale;
    float newHeight = spriteBounds.height * islandDeflectionPaddingScale;
    float newLeft = spriteBounds.left - (newWidth - spriteBounds.width) / 2.f;
    float newTop = spriteBounds.top - (newHeight - spriteBounds.height) / 2.f;
    sf::FloatRect deflectionBounds(newLeft, newTop, newWidth, newHeight);

    // If there's no current deflection target or the ship is clear of the corner, find a new corner.
    sf::Vector2f spriteCenter = sf::Vector2f(deflectionBounds.left + deflectionBounds.width / 2.f, deflectionBounds.top + deflectionBounds.height / 2.f);
    if (vm::isInFront(getSprite().getPosition(), spriteCenter, deflectionTarget)) hasDeflectionTarget = false;

    GlobalValues::getInstance().displayText(std::to_string(vm::isInFront(getSprite().getPosition(), spriteCenter, deflectionTarget)), getSprite().getPosition() + sf::Vector2f(20.f, 50.f), sf::Color::White);
    if (!hasDeflectionTarget) {
        std::vector<sf::Vector2f> corners{
            sf::Vector2f(deflectionBounds.left, deflectionBounds.top),
            sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top),
            sf::Vector2f(deflectionBounds.left, deflectionBounds.top + deflectionBounds.height),
            sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top + deflectionBounds.height)
        };

        float closestDistance = std::numeric_limits<float>::max();

        for (const auto& corner : corners) {
            float distanceToCorner = vm::distance(getSprite().getPosition(), corner);
            if (distanceToCorner < closestDistance) {
                closestDistance = distanceToCorner;
                deflectionTarget = corner;
            }
        }

        hasDeflectionTarget = true;
    }

    // Update travel direction to steer towards the deflection target
    return vm::normalize(deflectionTarget);
}

