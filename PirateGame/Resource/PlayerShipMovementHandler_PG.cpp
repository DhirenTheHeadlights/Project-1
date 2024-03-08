#include "PlayerShipMovementHandler_PG.h"

using namespace PirateGame;

// Move the ship
void PlayerShipMovementHandler::move(float baseSpeed) {
	// Initialize the window and map size
	window = GlobalValues::getInstance().getWindow();
	sf::Vector2f map = GlobalValues::getInstance().getMapSize();
	this->baseSpeed = baseSpeed;

	if (!initialPositionSet) {
		position = sprite.getPosition(); // This is here as a temporary fix for the ship's position being
		// reset to (0, 0) after its set to a random position in world
		initialPositionSet = true;
	}

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

void PlayerShipMovementHandler::applyBoundaryConstraints(sf::Vector2f& position, const sf::Vector2f& mapSize) {
	float sizeX = sprite.getGlobalBounds().width;
	float sizeY = sprite.getGlobalBounds().height;

	// If the ship is out of bounds, set the position to the boundary
	position.x = std::max(0.f, std::min(position.x, mapSize.x - sizeX));
	position.y = std::max(0.f, std::min(position.y, mapSize.y - sizeY));
}

void PlayerShipMovementHandler::updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed) {
	if (isColliding) speed -= 10.f;
	else {
		// Calculate wind effect
		WindController& windController = GlobalValues::getInstance().getWindController();
		sf::Vector2f windDirection = normalize(windController.getWindDirection()); // Ensure wind direction is normalized
		float windEffect = dot(windDirection, direction) * windController.getWindSpeed();

		// Gradually increase the speed to the base speed
		const float acceleration = 0.1f; // The acceleration factor
		if (speed < (baseSpeed + windEffect)) speed += acceleration;
		if (speed > (baseSpeed + windEffect)) speed -= acceleration;

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
