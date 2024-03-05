#include "Player_Plat.h"

using namespace PlatformerGame;


const sf::Vector2f Player::GRAVITY = sf::Vector2f(0, 0.05f);  // Gravity value

Player::Player(sf::Vector2f& map, sf::RenderWindow& window) : window(window) {
	// Initialize the player sprite
	playerSprite.setPosition(100.f, 100.f);
	playerSprite.setScale(spriteScale);
	playerSprite.setTexture(texture.grabPlayerTexture(0));
	playerSprite.setOrigin(playerSprite.getLocalBounds().width / 2.0f, playerSprite.getLocalBounds().height / 2.0f);
	std::cout<< "Sprite size: " << playerSprite.getGlobalBounds().width << ", " << playerSprite.getGlobalBounds().height << std::endl;

	leftBoundary = 0.f;
	rightBoundary = 100000; // windowSize should be passed to the Player or known globally
	topBoundary = 0.f;
	bottomBoundary = 1000;
}

void Player::move() {

	elapsed = frametimeClock.restart().asMilliseconds();

	//Horizontal movement logic
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity.x = -6.f;
		playerSprite.setScale(-spriteScale.x, spriteScale.y);
		
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity.x = 6.f;
		playerSprite.setScale(spriteScale);

	}
	else {
		velocity.x = 0;
	}
	if (isOnGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		velocity.y = -4.f;
		isOnGround = false;
	}

	// Bounds check
	position = playerSprite.getPosition() + velocity;
	if (position.x < leftBoundary) position.x = leftBoundary;
	if (position.x > rightBoundary - playerSprite.getGlobalBounds().width) position.x = rightBoundary - playerSprite.getGlobalBounds().width;
	if (position.y < topBoundary) position.y = topBoundary;
	if (position.y > bottomBoundary - playerSprite.getGlobalBounds().height) position.y = bottomBoundary - playerSprite.getGlobalBounds().height;

	playerSprite.setPosition(position);
	//std::cout << "Player position: " << playerSprite.getPosition().x << ", " << playerSprite.getPosition().y << std::endl;
}

void Player::applyGravity() {
	if (!isOnGround) {
		velocity += GRAVITY;  // Apply gravity to the velocity
	}
}

void Player::draw() {
	window.draw(playerSprite);
}

void Player::handlePlayerState() {
	if (playerSprite.getPosition().y == 1000.f - playerSprite.getGlobalBounds().height) {
		isOnGround = true;
		velocity.y = 0.f;
	}
	else {
		isOnGround = false;
	}

}