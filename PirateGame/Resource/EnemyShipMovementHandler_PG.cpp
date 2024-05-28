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
    GlobalValues::getInstance().getWindow()->draw(vm::createVectorLine(getSprite().getPosition(), directionToDestination * 100.f, sf::Color::Green));
    GlobalValues::getInstance().getWindow()->draw(vm::createVectorLine(getSprite().getPosition(), direction * 100.f, sf::Color::Red));
    GlobalValues::getInstance().displayText("Num nearby landmass: " + std::to_string(getNearbyLandmasses().size()), getSprite().getPosition() + sf::Vector2f(0.f, 50.f), sf::Color::White);
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
    for (const auto& sprite : sprites) {
        // Get the sprite's bounds and create a padded bounding box for deflection
        sf::FloatRect spriteBounds = sprite.getGlobalBounds();
        float newWidth = spriteBounds.width * deflectionPaddingScale;
        float newHeight = spriteBounds.height * deflectionPaddingScale;
        float newLeft = spriteBounds.left - (newWidth - spriteBounds.width) / 2.f;
        float newTop = spriteBounds.top - (newHeight - spriteBounds.height) / 2.f;
        sf::FloatRect deflectionBounds(newLeft, newTop, newWidth, newHeight);

        // Check if the point is within the sprite's bounds
        if (checkForCollision(deflectionBounds, travelDirection, deflectionDistance)) {
            // Draw the deflection bounds
            sf::RectangleShape deflectionRect(sf::Vector2f(deflectionBounds.width, deflectionBounds.height));
            deflectionRect.setPosition(deflectionBounds.left, deflectionBounds.top);
            deflectionRect.setFillColor(sf::Color::Transparent);
            deflectionRect.setOutlineColor(sf::Color::Red);
            deflectionRect.setOutlineThickness(1.f);
            GlobalValues::getInstance().getWindow()->draw(deflectionRect);

            if (deflectionSprite.getGlobalBounds() != spriteBounds) {
                deflectionSprite = sprite;
                hasPickedFirstCorner = false;
            }
            return calculateDeflectionVector(deflectionBounds, travelDirection, deflectionDistance);
        }
    }

    // Otherwise, return the travel direction
    hasPickedFirstCorner = false;
    return travelDirection;
}

bool EnemyShipMovementHandler::checkForCollision(const sf::FloatRect& deflectionBounds, const sf::Vector2f travelDirection, const float deflectionDistance) {
    // Grab all points in regular intervals along the travel direction
    for (float i = 0; i < vm::magnitude(travelDirection * deflectionDistance); i += deflectionVectorCheckInterval) {
        // Now, we will check if the point is within the bounds of any of the sprites
        sf::Vector2f point = getSprite().getPosition() + vm::normalize(travelDirection) * i;

        // Check if the point is within the sprite's bounds
        if (deflectionBounds.contains(point)) {
            return true;
        }
    }
    return false;
}

sf::Vector2f EnemyShipMovementHandler::calculateDeflectionVector(const sf::FloatRect& deflectionBounds, const sf::Vector2f travelDirection, const float deflectionDistance) {
    // Create a vector of the corners of the deflection bounds
    std::vector<sf::Vector2f> corners {
        sf::Vector2f(deflectionBounds.left, deflectionBounds.top),
        sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top),
        sf::Vector2f(deflectionBounds.left, deflectionBounds.top + deflectionBounds.height),
        sf::Vector2f(deflectionBounds.left + deflectionBounds.width, deflectionBounds.top + deflectionBounds.height)
    };

    // Create a vector of all edges of the deflection bounds
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edges {
		{corners[0], corners[1]},
		{corners[0], corners[2]},
		{corners[1], corners[3]},
		{corners[2], corners[3]}
	};

    // Find the first corner and move towards it
    float closestDistance = std::numeric_limits<float>::max();
    if (!hasPickedFirstCorner) {
        for (const auto& corner : corners) {
            float distanceToCorner = vm::distance(getSprite().getPosition(), corner);
            if (distanceToCorner < closestDistance) {
                closestDistance = distanceToCorner;
                deflectionTarget = corner;
            }
        }
    }

    // Remove a corner if the ship is clear of it and the bounds are not in the way. If it is the first corner, set hasPickedFirstCorner to true.
    corners.erase(std::remove_if(corners.begin(), corners.end(), [&](const sf::Vector2f& corner) {
        bool isInFront = vm::isInFront(getSprite().getPosition(), corner, vm::normalize(destination - getSprite().getPosition()));
        bool inCollisionPath = checkForCollision(deflectionBounds, vm::normalize(corner - getSprite().getPosition()), deflectionDistance);
        if (isInFront && !inCollisionPath) {
            if (!hasPickedFirstCorner) {
                hasPickedFirstCorner = true;
            }
            return true;  // Remove corner if passed
        }
        return false;  // Keep corner otherwise
    }), corners.end());

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

    // draw it
    sf::CircleShape farthestCircle(5.f);
    farthestCircle.setFillColor(sf::Color::Green);
    farthestCircle.setPosition(farthestCorner);
    farthestCircle.setOrigin(farthestCircle.getRadius(), farthestCircle.getRadius());
    GlobalValues::getInstance().getWindow()->draw(farthestCircle);

    bool onEdge = false;

    // See if the ship is on an edge
    for (const auto& edge : edges) {
		if (vm::distanceToLine(getSprite().getPosition(), edge.first, edge.second) < onEdgeThreshold) {
			onEdge = true;
			break;
		}
	}

    // Find the closest corner to the destination if the first corner has been picked
    if (hasPickedFirstCorner && onEdge) {
        // Find the closest corner to the destination
        for (const auto& corner : corners) {
            float distanceToCorner = vm::distance(destination, corner);
            if (distanceToCorner < closestDistance && corner != farthestCorner) {
                closestDistance = distanceToCorner;
                deflectionTarget = corner;
            }
        }
    }
    else if (hasPickedFirstCorner && !onEdge) {
        // Go towards the nearest edge
        for (const auto& edge : edges) {
			float distanceToEdge = vm::distanceToLine(edge.first, edge.second, getSprite().getPosition());
			if (distanceToEdge < closestDistance) {
				closestDistance = distanceToEdge;
				deflectionTarget = vm::closestPointOnLine(edge.first, edge.second, getSprite().getPosition());
			}
		}
	}

    // Draw the deflection target
    sf::CircleShape deflectionCircle(5.f);
    deflectionCircle.setFillColor(sf::Color::Red);
    deflectionCircle.setPosition(deflectionTarget);
    deflectionCircle.setOrigin(deflectionCircle.getRadius(), deflectionCircle.getRadius());
    GlobalValues::getInstance().getWindow()->draw(deflectionCircle);

    // Update travel direction to steer towards the deflection target
    return vm::normalize(deflectionTarget - getSprite().getPosition());
}

