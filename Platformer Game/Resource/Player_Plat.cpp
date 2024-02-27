#include "Player_Plat.h"
#include <iostream>

const sf::Vector2f Player_Plat::GRAVITY = sf::Vector2f(0, 2.f);  // Gravity value

Player_Plat::Player_Plat(sf::Vector2f& map, sf::RenderWindow& window) : window(window) {
	// Init player
	player.setSize(sf::Vector2f(100.f, 100.f));
	player.setFillColor(sf::Color::Red);
	player.setPosition(0.f, 980.f);

	maxVelocity = sf::Vector2f(30.f, 30.f);
	
	leftBoundary = 0.f;
	rightBoundary = 1820.f; // windowSize should be passed to the Player or known globally
	topBoundary = 0.f;
	bottomBoundary = 1080.f;
}

void Player_Plat::move() {

	elapsed = frametimeClock.restart().asMilliseconds();
	 //Horizontal movement logic
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity.x = -10.f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity.x = 10.f;
	}
	else {
		isMoving = false;
	}
	if (isOnGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		velocity.y = -20.f;
		isOnGround = false;
	}

	// Bounds check
	sf::Vector2f newPosition = player.getPosition() + velocity;
	if (newPosition.x < leftBoundary) newPosition.x = leftBoundary;
	if (newPosition.x > rightBoundary - player.getSize().x) newPosition.x = rightBoundary - player.getSize().x;
	if (newPosition.y < topBoundary) newPosition.y = topBoundary;
	if (newPosition.y > bottomBoundary - player.getSize().y) newPosition.y = bottomBoundary - player.getSize().y;

	player.setPosition(newPosition);
}

void Player_Plat::applyGravity() {
	if (!isOnGround) {
		velocity += GRAVITY;  // Apply gravity to the velocity
	}
}

void Player_Plat::draw() {
	window.draw(player);
}

void Player_Plat::handlePlayerState() {
	if (player.getPosition().y == window.getSize().y - player.getSize().y) {
		isOnGround = true;
		velocity.y = 0.f;
	}
	else {
		isOnGround = false;
	}

}