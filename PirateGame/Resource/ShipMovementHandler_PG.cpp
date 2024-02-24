#include "ShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship
sf::Vector2f ShipMovementHandler::move(float baseSpeed) {
	// Initialize the window and map size
	window = GlobalValues::getInstance().getWindow();
	sf::Vector2f map = GlobalValues::getInstance().getMapSize();
	sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f direction = normalize(mousePosition - sprite.getPosition());

	float elapsed = deltaTime.restart().asSeconds();
	speed = baseSpeed;

	updateVelocity(direction, elapsed, baseSpeed);
	setSpriteRotation(direction);
	applyBoundaryConstraints(position, map);

	// Return the position of the ship
	return sf::Vector2f(position);
}

sf::Vector2f ShipMovementHandler::normalize(sf::Vector2f vector) {
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length == 0.f) return sf::Vector2f(0.f, 0.f);
	return vector / length;
}

void ShipMovementHandler::applyBoundaryConstraints(sf::Vector2f& position, const sf::Vector2f& mapSize) {
	float sizeX = sprite.getGlobalBounds().width;
	float sizeY = sprite.getGlobalBounds().height;

	// If the ship is out of bounds, set the position to the boundary
	position.x = std::max(0.f, std::min(position.x, mapSize.x - sizeX));
	position.y = std::max(0.f, std::min(position.y, mapSize.y - sizeY));
}

void ShipMovementHandler::updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed) {
	// If friction is enabled, decrease the speed. Otherwise, set the velocity.
	if (isColliding) {
		speed = baseSpeed * frictionCoefficient;
	}
	else velocity = sf::Vector2f(direction.x * speed, direction.y * speed);

	// Update the position
	position += velocity * elapsedTime;
}

void ShipMovementHandler::setSpriteRotation(sf::Vector2f& direction) {
	if (isColliding) return;
	// Rotate the sprite using conversion from vector to angle with atan2
	const float pi = 3.14159265f;
	float angle = std::atan2(direction.y, direction.x) * 180.f / pi + 90.f;
	sprite.setRotation(angle);
}

void ShipMovementHandler::collisionMovement(sf::Sprite& collidingSprite) {
	isColliding = true;

	// Calculate the normalized normal vector from the ship's center to the colliding sprite's center
	sf::Vector2f normal = sf::Vector2f(
		static_cast<float>(sprite.getTextureRect().left + sprite.getTextureRect().width / 2) -
		(collidingSprite.getTextureRect().left + collidingSprite.getTextureRect().width / 2),
		static_cast<float>(sprite.getTextureRect().top + sprite.getTextureRect().height / 2) -
		(collidingSprite.getTextureRect().top + collidingSprite.getTextureRect().height / 2)
	);
	normal = normalize(normal);

	// Apply a damping factor to the velocity to simulate friction and prevent oscillations
	float dampingFactor = 0.5f; // Reduce velocity by half in the direction of the normal
	sf::Vector2f dampedVelocity = velocity - normal * dot(velocity, normal) * dampingFactor;

	// Ensure the ship is moved slightly away from the colliding object to prevent sticking
	float separationDistance = 5.0f; // Adjust as necessary
	position += normal * separationDistance;

	// Update the ship's velocity
	velocity = dampedVelocity;

	// Additional: Ensure separation based on direction of approach
	ensureSeparation(position, normal, collidingSprite);
}

void ShipMovementHandler::ensureSeparation(sf::Vector2f& position, const sf::Vector2f& normal, const sf::Sprite& collidingSprite) {
	float pushOutDistance = 2.0f; // Further adjust based on needs
	// Calculate a push-out vector based on normal and ship's approach direction
	sf::Vector2f pushOutVector = normal * pushOutDistance;

	// Check the direction of approach and adjust the pushOutVector accordingly
	sf::Vector2f approachVector = position - collidingSprite.getPosition();
	if (dot(approachVector, normal) < 0) {
		pushOutVector = -pushOutVector; // Invert the push-out direction for opposite approach
	}

	// Apply the push-out vector to position to ensure separation
	position += pushOutVector;
}


// Helper function to calculate dot product
float ShipMovementHandler::dot(const sf::Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.x + a.y * b.y;
}
