#include "ShipMovementHandler_PG.h"

using namespace PirateGame;

void ShipMovementHandler::move(const sf::Vector2f& sailDirection, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed) {
	// Calculate the direction based on the ship's current rotation
	sf::Vector2f direction = vm::angleDegreesToVector(sprite.getRotation() - 90.f); // Subtract 90 degrees to align with SFML's rotation

	// Update the position based on the direction and speed
	updateVelocity(direction, dt, sailDirection, windDirection, windSpeed);
	sprite.move(velocity);
	setSpriteRotation();
}

void ShipMovementHandler::updateVelocity(const sf::Vector2f& direction, const sf::Time& elapsedTime, const sf::Vector2f& sailDirection, const sf::Vector2f& windDirection, const float windSpeed) {
	if (isColliding && speed > 0) speed -= 10.f;
	else if (!dropAnchor) {
		// Use the sail rotation to determine potency of wind effect.
		float sailRotationEffect = std::max(0.f, vm::dot(vm::normalize(sailDirection), vm::normalize(windDirection))); // The closer to 1, the more effective the wind is

		// Calculate the final wind effect
		sailRotationEffect *= windSpeed; // The wind speed will determine the final wind effect

		// Gradually increase the speed to the base speed, multiplied by the wind effect
		const float acceleration = std::max(5.f, sailRotationEffect); // The acceleration factor
		if (speed < (baseSpeed + sailRotationEffect)) speed += acceleration * elapsedTime.asSeconds();
		else if (speed > (baseSpeed + sailRotationEffect)) speed -= acceleration * elapsedTime.asSeconds();

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

		if (speed > 0) speed -= deceleration; // Gradually decrease the speed to 0
		else if (speed < 0.01f && !anchorPushBack && speed > speedMin) { // Start pull back when the speed is close to 0
			speed -= deceleration;
			if (speed < speedMin) anchorPushBack = true;
		}
		else if (speed < 0) speed += reacceleration * elapsedTime.asSeconds(); // This will bring the speed back up to 0	
	}

	velocity = direction * speed * elapsedTime.asSeconds(); // Set the velocity based on the speed
}

void ShipMovementHandler::rotateTowards(float targetAngle) {
	// Calculate the difference between the target and current angle
	float angleDifference = vm::normalizeAngle(targetAngle - sprite.getRotation(), -180.f, 180.f);

	// Calculate the extra rotational acceleration based on the angle difference
	// Also, the accel is based on the speed of the ship
	float accel = abs(10 * angleDifference / 180.f * speed / baseSpeed);

	// Limit the turning speed
	angleDifference = std::clamp(angleDifference, -turningSpeed * turningMultiplier, turningSpeed * turningMultiplier);

	// Set the new rotation
	sprite.rotate(accel * angleDifference);
}

void ShipMovementHandler::collisionMovement(const sf::Sprite& collidingSprite) {
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
	sprite.move(normal * separationDistance);

	// Update the ship's velocity
	velocity = dampedVelocity;

	// Additional: Ensure separation based on direction of approach
	ensureSeparation(normal, collidingSprite);
}

void ShipMovementHandler::ensureSeparation(const sf::Vector2f& normal, const sf::Sprite& collidingSprite) {
	// Calculate a push-out vector based on normal and ship's approach direction
	sf::Vector2f pushOutVector = normal * pushOutDistance;

	// Check the direction of approach and adjust the pushOutVector accordingly
	sf::Vector2f approachVector = sprite.getPosition() - collidingSprite.getPosition();
	if (vm::dot(approachVector, normal) < 0) {
		pushOutVector = -pushOutVector; // Invert the push-out direction for opposite approach
	}

	// Apply the push-out vector to position to ensure separation
	sprite.move(pushOutVector);
}

void ShipMovementHandler::addCannonRecoil(const sf::Vector2f& direction, const float recoil) {
	// Apply the recoil to the ship's position
	sprite.move(direction * recoil);
}
