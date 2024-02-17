#include "ShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship, adjusting for collisions
sf::Vector2f ShipMovementHandler::move(float speed) {
    sf::Vector2f map = GlobalValues::getInstance().getMapSize();
    window = GlobalValues::getInstance().getWindow();
    float baseSpeed = speed;

    // Get the position of the mouse and calculate direction
    sf::Vector2f viewPos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    sf::Vector2f dirToMouse = viewPos - sprite.getPosition();
    normalizeDirection(dirToMouse);

    // Adjust speed and velocity based on friction and collision
    adjustVelocity(dirToMouse, baseSpeed);

    // Apply rotation based on direction to mouse
    applyRotation(dirToMouse);

    // Determine new position and adjust for boundary constraints
    return adjustPositionAndBoundaryChecks(velocity, deltaTime.restart().asSeconds(), map);
}

// Normalize direction vector
void ShipMovementHandler::normalizeDirection(sf::Vector2f& direction) {
    if (direction.x == 0 && direction.y == 0) return; // Already on the ship
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;
}

// Adjust velocity based on friction, collision, and direction
void ShipMovementHandler::adjustVelocity(sf::Vector2f& dirToMouse, float baseSpeed) {
    if (isColliding) { // Check for collision flag here
        speed = baseSpeed * frictionSpeedCoeff;
        // Apply collision movement adjustments
        applyCollisionAdjustments();
    }
    else {
        velocity = sf::Vector2f(dirToMouse.x * speed, dirToMouse.y * speed);
    }
}

// Apply adjustments based on collision response
void ShipMovementHandler::applyCollisionAdjustments() {
    // Calculate the tangent vector by taking the perpendicular of the collision normal
    sf::Vector2f tangent(-normal.y, normal.x);

    // Project the ship's velocity onto the tangent vector
    float dotProduct = velocity.x * tangent.x + velocity.y * tangent.y;
    sf::Vector2f adjustedVelocity = tangent * dotProduct;

    // Apply friction to the adjusted velocity
    adjustedVelocity *= 0.000006f;

    // Update the ship's velocity
    velocity = adjustedVelocity;
}

// Apply rotation to sprite based on direction
void ShipMovementHandler::applyRotation(const sf::Vector2f& dirToMouse) {
    float angleRad = std::atan2(dirToMouse.y, dirToMouse.x);
    const float pi = 3.1415926f;
    rotation = angleRad * 180.0f / pi + 90;
    sprite.setRotation(rotation);
}

// Adjust position with boundary checks
sf::Vector2f ShipMovementHandler::adjustPositionAndBoundaryChecks(sf::Vector2f velocity, float elapsed, sf::Vector2f map) {
    // Update position with velocity
    position += velocity * elapsed;

    // Boundary checks
    constrainPositionToBounds(map);

    // Adjust sprite position
    sprite.setPosition(position.x, position.y);

    return position;
}

// Constrain the ship's position within map boundaries
void ShipMovementHandler::constrainPositionToBounds(sf::Vector2f map) {
    if (position.x < 0) position.x = 0;
    if (position.y < 0) position.y = 0;
    if (position.x > map.x) position.x = map.x;
    if (position.y > map.y) position.y = map.y;
}