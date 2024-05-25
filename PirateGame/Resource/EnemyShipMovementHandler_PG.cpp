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
    direction = deflectTravelDirection(getNearbyLandmasses(), directionToDestination, deflectionDistanceLandmass);
    if (direction == directionToDestination) direction = deflectTravelDirection(getNearbyShips(), directionToDestination, deflectionDistanceShip);


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

	GlobalValues::getInstance().getWindow()->draw(vm::createVector(getSprite().getPosition(), direction * deflectionDistanceLandmass, sf::Color::Red));
    GlobalValues::getInstance().getWindow()->draw(vm::createVector(getSprite().getPosition(), directionToDestination * deflectionDistanceLandmass, sf::Color::Green));
	GlobalValues::getInstance().displayText("AimTowardsTarget: " + getAimTowardsTarget() ? "True" : "False", getSprite().getPosition() + sf::Vector2f(0, 50), sf::Color::White);
	GlobalValues::getInstance().displayText("Direction: " + std::to_string(direction.x) + ", " + std::to_string(direction.y), getSprite().getPosition() + sf::Vector2f(0, 100), sf::Color::White);
	GlobalValues::getInstance().displayText("Position: " + std::to_string(getSprite().getPosition().x) + ", " + std::to_string(getSprite().getPosition().y), getSprite().getPosition() + sf::Vector2f(0, 150), sf::Color::White);
    GlobalValues::getInstance().displayText("Has Deflection Target: " + hasDeflectionTarget ? "True" : "False", getSprite().getPosition() + sf::Vector2f(0, 200), sf::Color::White);

    setTurningMultiplier(activeTowardsTarget ? 1.5f : 1.f);

	float targetAngle = vm::normalizeAngle(vm::vectorToAngleDegrees(direction) + 90.f); // +90 to account for sprite rotation
    rotateTowards(targetAngle);
}

sf::Vector2f EnemyShipMovementHandler::deflectTravelDirection(const std::vector<sf::Sprite>& sprites, sf::Vector2f travelDirection, float deflectionDistance) {
    // Check if there is a possible collision
    for (float i = 0; i < vm::magnitude(travelDirection * deflectionDistance); i += deflectionVectorCheckInterval) {
        sf::Vector2f checkPosition = getSprite().getPosition() + travelDirection * i;
        for (const auto& sprite : sprites) {
            sf::FloatRect spriteBounds = sprite.getGlobalBounds();
            // Check if the point is within the sprite's bounds
            if (spriteBounds.contains(checkPosition)) {
                // Draw a rect around the sprite
                sf::RectangleShape rect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
                rect.setPosition(spriteBounds.left, spriteBounds.top);
                rect.setFillColor(sf::Color::Transparent);
                rect.setOutlineColor(sf::Color::Red);
                rect.setOutlineThickness(2.f);
                GlobalValues::getInstance().getWindow()->draw(rect);

                if (deflectionSprite.getGlobalBounds() != spriteBounds) {
					deflectionSprite = sprite;
					hasDeflectionTarget = false;
				}

                return calculateDeflectionVector(deflectionSprite, travelDirection, deflectionDistance);
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

    std::vector<sf::Vector2f> corners{
        sf::Vector2f(deflectionBounds.left, deflectionBounds.top),
        sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top),
        sf::Vector2f(deflectionBounds.left, deflectionBounds.top + deflectionBounds.height),
        sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top + deflectionBounds.height)
    };

    // Remove a corner if the ship is clear of it
    corners.erase(std::remove_if(corners.begin(), corners.end(), [&](const sf::Vector2f& corner) {
        return !vm::isInFront(getSprite().getPosition(), corner, vm::normalize(destination - getSprite().getPosition()));
    }), corners.end());

    float closestDistance = std::numeric_limits<float>::max();

    for (const auto& corner : corners) {
        float distanceToCorner = vm::distance(getSprite().getPosition(), corner);
        if (distanceToCorner < closestDistance) {
            closestDistance = distanceToCorner;
            deflectionTarget = corner;
        }
    }

    // Draw a small circle at the deflection target
    sf::CircleShape circle(5.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(deflectionTarget);
    GlobalValues::getInstance().getWindow()->draw(circle);
    GlobalValues::getInstance().displayText("Deflection Target: " + std::to_string(deflectionTarget.x) + ", " + std::to_string(deflectionTarget.y), deflectionTarget + sf::Vector2f(0, 20.f), sf::Color::White);

    // Update travel direction to steer towards the deflection target
    return vm::normalize(deflectionTarget - getSprite().getPosition());
}

