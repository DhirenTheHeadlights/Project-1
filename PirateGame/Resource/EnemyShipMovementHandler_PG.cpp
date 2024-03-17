#include "EnemyShipMovementHandler_PG.h"


using namespace PirateGame;

void EnemyShipMovementHandler::move(float baseSpeed) {

	sf::Vector2f map = GlobalMap::getInstance().getWorldMap();
	setBaseSpeed(baseSpeed * enemySpeedMultiplier);

	setInitialPosition();

	float elapsed = getDeltaTime().restart().asSeconds();

	// Calculate the direction based on the ship's current rotation
	float rotationInRadians = (getSprite().getRotation() - 90.f) * pi / 180.f; // Subtract 90 degrees to align with SFML's rotation
	sf::Vector2f direction(std::cos(rotationInRadians), std::sin(rotationInRadians));

	updateVelocity(direction, elapsed, baseSpeed);
	setSpriteRotation();
	applyBoundaryConstraints(position, map);

	// Set the new position
	getSprite().setPosition(position);
}

void EnemyShipMovementHandler::setSpriteRotation() {
	if (getIsColliding() || getStopShipRotationFlag()) return;

	// Grab window
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

	// Calculate the direction to the player
	sf::Vector2f directionToPlayer = playerPos - getSprite().getPosition();
	float distance = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);
	
	if (distance < static_cast<float>(4000)) {
		directionToPlayer = directionToPlayer + sf::Vector2f(playerVelocity.x * 0.33, playerVelocity.y * 0.33);
	}
	if (distance < static_cast<float>(2000)) {
		directionToPlayer = normalize(sf::Vector2f(directionToPlayer.y, -directionToPlayer.x));
	}
	else
	{
		directionToPlayer = normalize(directionToPlayer);
	}
	

	window->draw(GlobalValues::getInstance().createVector(getSprite().getPosition(), directionToPlayer * 100.f, sf::Color::Red));

	// Rotate the sprite using conversion from vector to angle with atan2
	float targetAngle = std::atan2(directionToPlayer.y, directionToPlayer.x) * 180.f / pi + 90.f;

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

	// Limit the turning speed
	angleDifference = std::clamp(angleDifference, -turningSpeed, turningSpeed);

	// Set the new rotation
	getSprite().setRotation(currentAngle + (accel * angleDifference));
}

