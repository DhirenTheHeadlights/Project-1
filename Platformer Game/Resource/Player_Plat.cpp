#include "Player_Plat.h"

using namespace PlatformerGame;


const sf::Vector2f Player::GRAVITY = sf::Vector2f(0, 0.05f);  // Gravity value

Player::Player(sf::Vector2f& map, sf::RenderWindow& window) : window(window) {
	// Init player

	//player.setSize(sf::Vector2f(1.f, 1.f));
	//player.setFillColor(sf::Color::Red);
	//player.setPosition(90.f, 90.f);
	playerSprite.setPosition(100.f, 100.f);
	playerSprite.setScale(2.0f, 2.0f);
	playerSprite.setTexture(texture.grabPlayerTexture(0));


	sf::Vector2u textureSize = playerSprite.getTexture()->getSize();
	playerSprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);



	maxVelocity = sf::Vector2f(30.f, 30.f);

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
		playerSprite.setScale(-2.f, 2.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity.x = 6.f;
		playerSprite.setScale(2.f, 2.f);
	}
	else {
		velocity.x = 0;
	}
	if (isOnGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		velocity.y = -4.f;
		isOnGround = false;
	}

	// Bounds check
	sf::Vector2f newPosition = player.getPosition() + velocity;
	if (newPosition.x < leftBoundary) newPosition.x = leftBoundary;
	if (newPosition.x > rightBoundary - player.getSize().x) newPosition.x = rightBoundary - player.getSize().x;
	if (newPosition.y < topBoundary) newPosition.y = topBoundary;
	if (newPosition.y > bottomBoundary - player.getSize().y) newPosition.y = bottomBoundary - player.getSize().y;

	player.setPosition(newPosition);
	playerSprite.setPosition(newPosition);
}

void Player::applyGravity() {
	if (!isOnGround) {
		velocity += GRAVITY;  // Apply gravity to the velocity
	}
}

void Player::draw() {
	window.draw(player);
	window.draw(playerSprite);
}

void Player::handlePlayerState() {
	if (player.getPosition().y == 1000.f - player.getSize().y) {
		isOnGround = true;
		velocity.y = 0.f;
	}
	else {
		isOnGround = false;
	}

}