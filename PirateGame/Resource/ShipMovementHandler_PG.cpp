#include "ShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship
void PlayerShipMovementHandler::move(float baseSpeed) {
	// Initialize the window and map size
	window = GlobalValues::getInstance().getWindow();
	sf::Vector2f map = GlobalValues::getInstance().getMapSize();
	this->baseSpeed = baseSpeed;
	position = sprite.getPosition(); // This is here as a temporary fix for the ship's position being
	// reset to (0, 0) after its set to a random position in world

	float elapsed = deltaTime.restart().asSeconds();

	// Calculate the direction based on the ship's current rotation
	float rotationInRadians = (sprite.getRotation() - 90.f) * pi / 180.f; // Subtract 90 degrees to align with SFML's rotation
	sf::Vector2f direction(std::cos(rotationInRadians), std::sin(rotationInRadians));

	updateVelocity(direction, elapsed, baseSpeed);
	setSpriteRotation(direction);
	applyBoundaryConstraints(position, map);

	// Set the new position
	sprite.setPosition(position);
}


sf::Vector2f PlayerShipMovementHandler::normalize(sf::Vector2f vector) {
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length == 0.f) return sf::Vector2f(0.f, 0.f);
	return vector / length;
}

void PlayerShipMovementHandler::applyBoundaryConstraints(sf::Vector2f& position, const sf::Vector2f& mapSize) {
	float sizeX = sprite.getGlobalBounds().width;
	float sizeY = sprite.getGlobalBounds().height;

	// If the ship is out of bounds, set the position to the boundary
	position.x = std::max(0.f, std::min(position.x, mapSize.x - sizeX));
	position.y = std::max(0.f, std::min(position.y, mapSize.y - sizeY));
}

void PlayerShipMovementHandler::updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed) {
	if (isColliding) speed = baseSpeed * frictionCoefficient;
	else {
		// Calculate wind effect
		WindController& windController = GlobalValues::getInstance().getWindController();
		sf::Vector2f windDirection = normalize(windController.getWindDirection()); // Ensure wind direction is normalized
		float windEffect = dot(windDirection, direction) * windController.getWindSpeed();

		// Gradually increase the speed to the base speed
		const float acceleration = 0.1f; // The acceleration factor
		if (speed < (baseSpeed + windEffect)) speed += acceleration;
		if (speed > (baseSpeed + windEffect)) speed = (baseSpeed + windEffect);
		else speed = baseSpeed;

		velocity = sf::Vector2f(direction.x * speed, direction.y * speed);
	}

	if (stopShipFlag) {
		// Gradually decrease the speed to 0
		const float deceleration = 0.1f; // The deceleration factor
		if (speed > 0) speed -= deceleration;
		else speed = 0;
		velocity = sf::Vector2f(direction.x * speed, direction.y * speed);
	}

	position += velocity * elapsedTime;
}


void PlayerShipMovementHandler::setSpriteRotation(sf::Vector2f& direction) {
	if (isColliding || stopShipRotationFlag) return;
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
	// Also, the accel is based on the speed of the ship
	float accel = abs(10 * angleDifference / 180.f * speed / baseSpeed);

	// Limit the turning speed
	angleDifference = std::clamp(angleDifference, -turningSpeed, turningSpeed);

	// Set the new rotation
	sprite.setRotation(currentAngle + (accel * angleDifference));
}

void PlayerShipMovementHandler::collisionMovement(sf::Sprite& collidingSprite) {
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

void PlayerShipMovementHandler::ensureSeparation(sf::Vector2f& position, const sf::Vector2f& normal, const sf::Sprite& collidingSprite) {
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

void PlayerShipMovementHandler::addCannonRecoil(sf::Vector2f direction, float recoil) {
	// Apply the recoil to the ship's position
	position += direction * recoil;
}

// Helper function to calculate dot product
float PlayerShipMovementHandler::dot(const sf::Vector2f& a, const sf::Vector2f& b) {
	return a.x * b.x + a.y * b.y;
}
