#include "ShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship
void ShipMovementHandler::move(float baseSpeed) {
	// Initialize the window and map size
	window = GlobalValues::getInstance().getWindow();
	sf::Vector2f map = GlobalValues::getInstance().getMapSize();

	float elapsed = deltaTime.restart().asSeconds();
	speed = baseSpeed;

	// Calculate the direction based on the ship's current rotation
	float rotationInRadians = (sprite.getRotation() - 90.f) * pi / 180.f; // Subtract 90 degrees to align with SFML's rotation
	sf::Vector2f direction(std::cos(rotationInRadians), std::sin(rotationInRadians));

	updateVelocity(direction, elapsed, baseSpeed);
	setSpriteRotation(direction);
	applyBoundaryConstraints(position, map);

	// Set the new position
	sprite.setPosition(position);
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
	if (isColliding) speed = baseSpeed * frictionCoefficient;
	else velocity = sf::Vector2f(direction.x * speed, direction.y * speed);
	if (stopShipFlag) velocity = sf::Vector2f(0, 0);

	// Update the position
	position += velocity * elapsedTime;
}

void ShipMovementHandler::setSpriteRotation(sf::Vector2f& direction) {
	if (isColliding|| stopShipRotationFlag) return;
	// Calculate the direction to the mouse
	sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	sf::Vector2f directionToMouse = normalize(mousePosition - sprite.getPosition());

	// Rotate the sprite using conversion from vector to angle with atan2
	float targetAngle = std::atan2(directionToMouse.y, directionToMouse.x) * 180.f / pi + 90.f;

	// Normalize the target angle to the range [0, 360]
	if (targetAngle < 0) targetAngle += 360;

	// Get the current angle
	float currentAngle = sprite.getRotation();

	// Calculate the difference between the target and current angle
	float angleDifference = targetAngle - currentAngle;
	
	// Normalize the angle difference to the range [-180, 180]
	while (angleDifference < -180) angleDifference += 360;
	while (angleDifference > 180) angleDifference -= 360;

	// Calculate the extra rotational acceleration based on the angle difference
	float accel = abs(angleDifference / 180.f);

	// Limit the turning speed
	angleDifference = std::clamp(angleDifference, -turningSpeed, turningSpeed);

	// Set the new rotation
	sprite.setRotation(currentAngle + accel*angleDifference);
}

void ShipMovementHandler::collisionMovement(sf::Sprite& collidingSprite) {
	isColliding = true;

	// Calculate the normalized normal vector from the ship's center to the colliding sprite's center
	sf::Vector2f normal = sf::Vector2f(
		sprite.getPosition().x + sprite.getGlobalBounds().width / 2 -
		(collidingSprite.getPosition().x + collidingSprite.getGlobalBounds().width / 2),
		sprite.getPosition().y + sprite.getGlobalBounds().height / 2 -
		(collidingSprite.getPosition().y + collidingSprite.getGlobalBounds().height / 2)
	);
	normal = normalize(normal);

	// Apply a damping factor to the velocity to simulate friction and prevent oscillations
	sf::Vector2f dampedVelocity = velocity - normal * dot(velocity, normal) * dampingFactor;

	// Ensure the ship is moved slightly away from the colliding object to prevent sticking
	position += normal * separationDistance;

	// Update the ship's velocity
	velocity = dampedVelocity;

	// Additional: Ensure separation based on direction of approach
	ensureSeparation(position, normal, collidingSprite);
}

void ShipMovementHandler::ensureSeparation(sf::Vector2f& position, const sf::Vector2f& normal, const sf::Sprite& collidingSprite) {
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

void ShipMovementHandler::addCannonRecoil(sf::Vector2f direction, float recoil) {
	// Apply the recoil to the ship's position
	position += direction * recoil;
}

// Helper function to calculate dot product
float ShipMovementHandler::dot(const sf::Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.x + a.y * b.y;
}
