#include "Player_Plat.h"

using namespace PlatformerGame;


const sf::Vector2f Player::GRAVITY = sf::Vector2f(0, 0.2f);  // Gravity value

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
		velocity.x = -10.f;
		playerSprite.setScale(-spriteScale.x, spriteScale.y);
		
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocity.x = 10.f;
		playerSprite.setScale(spriteScale);

	}
	else {
		velocity.x = 0;
	}
	if (isOnGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		velocity.y = -7.f;
		isOnGround = false;
	}

	// Bounds check
	position = playerSprite.getPosition() + velocity;
	if (position.x < leftBoundary) position.x = leftBoundary;
	if (position.x > rightBoundary - playerSprite.getGlobalBounds().width) position.x = rightBoundary - playerSprite.getGlobalBounds().width;
	if (position.y < topBoundary) position.y = topBoundary;
	if (position.y > bottomBoundary - playerSprite.getGlobalBounds().height) position.y = bottomBoundary - playerSprite.getGlobalBounds().height;


	//std::cout << "Player position: " << playerSprite.getPosition().x << ", " << playerSprite.getPosition().y << std::endl;
	prevPosition = playerSprite.getPosition();
	playerSprite.setPosition(position);


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
	if (playerSprite.getPosition().y == 1000.f - playerSprite.getGlobalBounds().height || isCollided == true) {
		isOnGround = true;
		velocity.y = 0.f;
	}
	else {
		isOnGround = false;
	}
	isCollided = false;

}

void Player::checkPlatformCollision(const std::vector<sf::RectangleShape>& platforms) {
	for (const auto& platform : platforms) {
		if (playerSprite.getGlobalBounds().intersects(platform.getGlobalBounds())) {
			//std::cout << "Collision detected" << std::endl;//
			isCollided = true;
			//handle collision logic using previous position and let player stand jump from platforms
			if (prevPosition.y + playerSprite.getGlobalBounds().height <= platform.getPosition().y) {
				playerSprite.setPosition(playerSprite.getPosition().x, platform.getPosition().y - playerSprite.getGlobalBounds().height);
				if (velocity.y > 0) {
					velocity.y = 0.f;
					isOnGround = true;
				}
			}
			else if (prevPosition.y > platform.getPosition().y + platform.getGlobalBounds().height) {
				//playerSprite.setPosition(playerSprite.getPosition().x, platform.getPosition().y + platform.getGlobalBounds().height);
				velocity.y = 0.f;
			}
			else if (prevPosition.x + playerSprite.getGlobalBounds().width <= platform.getPosition().x) {
				//playerSprite.setPosition(platform.getPosition().x - playerSprite.getGlobalBounds().width, playerSprite.getPosition().y);
				//velocity.x = 0.f;
			}
			else if (prevPosition.x >= platform.getPosition().x + platform.getGlobalBounds().width) {
				//playerSprite.setPosition(platform.getPosition().x + platform.getGlobalBounds().width, playerSprite.getPosition().y);
				//velocity.x = 0.f;
			}
			
		}
	}
}
