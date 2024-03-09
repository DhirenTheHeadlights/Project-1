#include "ShipMovementHandler_PG.h"

using namespace PirateGame;

void ShipMovementHandler::setInitialPosition() {
	if (!getInitialPositionSet()) {
		position = sprite.getPosition();
		initialPositionSet = true;
	}
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
	if (isColliding && speed > 0) speed -= 10.f;
	else if (!dropAnchor) {
		// Calculate wind effect
		WindController& windController = GlobalValues::getInstance().getWindController();
		sf::Vector2f windDirection = normalize(windController.getWindDirection()); // Ensure wind direction is normalized
		float windEffect = dot(windDirection, direction) * windController.getWindSpeed();

		// Gradually increase the speed to the base speed, multiplied by the wind effect
		const float acceleration = std::max(1.f, 1.f * windEffect); // The acceleration factor
		if (speed < (baseSpeed + windEffect)) speed += acceleration * elapsedTime;
		else if (speed > (baseSpeed + windEffect)) speed -= acceleration * elapsedTime;

		velocity = sf::Vector2f(direction.x * speed, direction.y * speed);

		// Set the anchor push back flag to be false. This will reset the flag so it can
		// be set to true again if the anchor is dropped.
		anchorPushBack = false;
	}

	if (dropAnchor) {
		// Gradually decrease the speed to 0
		const float deceleration = 20.f; // The deceleration factor
		// Here, we are going to specify a speed value that will be subtracted from the speed when 
		// the speed goes to 0. This will create a "backwards" effect as the negative speed will
		// slowly come back up to 0. This is to simulate the ship pulling back from the anchor
		const float speedMin = -20.f;
		const float reacceleration = 10.f;
		if (speed > 0) speed -= deceleration * elapsedTime;

		else if (speed < 0.01f && !anchorPushBack) {
			speed = speedMin;
			anchorPushBack = true;
			std::cout << "Speed: " << speed << std::endl;
		}

		else if (speed < 0) speed += reacceleration * elapsedTime; // This will bring the speed back up to 0
		velocity = sf::Vector2f(direction.x * speed, direction.y * speed);
	}

	position += velocity * elapsedTime;
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
