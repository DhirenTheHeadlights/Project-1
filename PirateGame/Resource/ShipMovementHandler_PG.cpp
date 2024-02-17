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
	return sf::Vector2f(position.x * 0.5f, position.y * 0.5f);
}

sf::Vector2f ShipMovementHandler::normalize(sf::Vector2f vector) {
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length == 0.f) return sf::Vector2f(0.f, 0.f);
	return vector / length;
}

void ShipMovementHandler::applyBoundaryConstraints(sf::Vector2f& position, const sf::Vector2f& mapSize) {
	float sizeX = sprite.getGlobalBounds().width;
	float sizeY = sprite.getGlobalBounds().height;

	position.x = std::max(0.f, std::min(position.x, mapSize.x - sizeX));
	position.y = std::max(0.f, std::min(position.y, mapSize.y - sizeY));
}

void ShipMovementHandler::updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed) {
	// If friction is enabled, decrease the speed. Otherwise, set the velocity.
	if (friction) {
		speed = baseSpeed * frictionCoefficient;
	}
	else velocity = sf::Vector2f(direction.x * speed, direction.y * speed);

	// Update the position
	position += velocity * elapsedTime;
}

// Set the rotation of the sprite
void ShipMovementHandler::setSpriteRotation(sf::Vector2f& direction) {
	// Rotate the sprite using conversion from vector to angle with atan2
	const float pi = 3.14159265f;
	float angle = std::atan2(direction.y, direction.x) * 180.f / pi + 90.f;
	sprite.setRotation(angle);
}

// Apply collision movement to the ship, redirect velocity as a scaled cross product with normalized collision vector
sf::Vector2f ShipMovementHandler::collisionMovement(sf::Sprite& collidingSprite) {
	// NO MORE AXIS RAHHHH, set friction to true
	friction = true;

	// Determine the normal vector
	sf::Vector2f normal = sf::Vector2f((static_cast<float>(sprite.getTextureRect().getPosition().x + 0.5 * sprite.getTextureRect().getSize().x) - // Ship's center x coordinate
				(collidingSprite.getTextureRect().getPosition().x + 0.5 * collidingSprite.getTextureRect().getSize().x)),			 // Landmass i's center x cooordinate
				static_cast<float>((sprite.getTextureRect().getPosition().y + 0.5 * sprite.getTextureRect().getSize().y) -			 // Ship's center y coordinate
				(collidingSprite.getTextureRect().getPosition().y + 0.5 * collidingSprite.getTextureRect().getSize().y)));			 // Landmass i's center y coordinate

	// Scalar friction facotr; this changes the degree to which the ship is "reflected" off on collision
	float friction_factor = 0.000007;

	// Calculate dot product between velocity and normalized collision vector
	int dot_product = 2 * (velocity.x * normal.x + velocity.y * normal.y);

	// Reflect velocity
	sf::Vector2f reflected_vector = sf::Vector2f(velocity.x - dot_product * normal.x * friction_factor, velocity.y - dot_product * normal.y * friction_factor);
	velocity.x = reflected_vector.x;
	velocity.y = reflected_vector.y;

	// Set the position of the ship
	float sizeX = sprite.getGlobalBounds().width;
	float sizeY = sprite.getGlobalBounds().height;
	return sf::Vector2f(position.x - sizeX / 2, position.y - sizeY / 2);
}