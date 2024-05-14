#include "ShipMovementHandler_PG.h"

using namespace PirateGame;

void ShipMovementHandler::setInitialPosition() {
	if (!getInitialPositionSet()) {
		position = sprite.getPosition();
		initialPositionSet = true;
	}
}

void ShipMovementHandler::updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed, sf::Vector2f sailDirection) {
	if (isColliding && speed > 0) speed -= 10.f;
	else if (!dropAnchor) {
		// Calculate wind effect
		GlobalWindController& windController = GlobalWindController::getInstance();
		sf::Vector2f windDirection = vm::normalize(windController.getWindDirection()); // Ensure wind direction is normalized
		float windEffect = vm::dot(windDirection, direction) * windController.getWindSpeed();

		// Normalize sail direction
		sailDirection = vm::normalize(sailDirection);

		// Use the sail rotation to determine potency of wind effect.
		float sailRotationEffect = 1 - std::fabs(vm::dot(sailDirection, windDirection)); // The closer to 1, the more effective the wind is

		// Calculate the final wind effect
		windEffect *= sailRotationEffect;

		GlobalValues::getInstance().displayText("Sail Rotation Effect: " + std::to_string(sailRotationEffect), sf::Vector2f(position.x + 10.f, position.y + 70.f), sf::Color::White);

		// Gradually increase the speed to the base speed, multiplied by the wind effect
		const float acceleration = std::max(1.f, 1.f * windEffect); // The acceleration factor
		if (speed < (baseSpeed + windEffect)) speed += acceleration * elapsedTime;
		else if (speed > (baseSpeed + windEffect)) speed -= acceleration * elapsedTime;

		velocity = sf::Vector2f(direction.x * speed, direction.y * speed);

		// Set the speed immediately before the anchor is dropped
		speedBeforeAnchorDrop = speed;

		// Set the anchor push back flag to be false. This will reset the flag so it can
		// be set to true again if the anchor is dropped.
		anchorPushBack = false;
	}

	if (dropAnchor) {
		// Gradually decrease the speed to 0
		const float deceleration = 20.f; // The deceleration factor, proportional to the speed

		// Here, we are going to specify a speed value that will be subtracted from the speed when 
		// the speed goes to 0. This will create a "backwards" effect as the negative speed will
		// slowly come back up to 0. This is to simulate the ship pulling back from the anchor.
		// Anchor will pull back more if the ship is moving faster.
		const float speedMin = -0.2f * speedBeforeAnchorDrop; 

		// The reacceleration factor, also proportional to the speed before anchor drop
		const float reacceleration = 0.2f * speedBeforeAnchorDrop;

		if (speed > 0) speed -= deceleration * elapsedTime; // Gradually decrease the speed to 0

		else if (speed < 0.01f && !anchorPushBack && speed > speedMin) { // Start pull back when the speed is close to 0
			speed -= deceleration;
			if (speed < speedMin) anchorPushBack = true;
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
	normal = vm::normalize(normal);

	// Apply a damping factor to the velocity to simulate friction and prevent oscillations
	sf::Vector2f dampedVelocity = velocity - normal * vm::dot(velocity, normal) * dampingFactor;

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
	if (vm::dot(approachVector, normal) < 0) {
		pushOutVector = -pushOutVector; // Invert the push-out direction for opposite approach
	}

	// Apply the push-out vector to position to ensure separation
	position += pushOutVector;
}

void ShipMovementHandler::addCannonRecoil(sf::Vector2f direction, float recoil) {
	// Apply the recoil to the ship's position
	position += direction * recoil;
}
