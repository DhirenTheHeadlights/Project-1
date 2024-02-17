#include "Ship_PG.h"

using namespace PirateGame;

// Create the ship
void Ship::createShip(ShipType type, ShipClass level) {
	// Set the window
	window = GlobalValues::getInstance().getWindow();

	// This switch case will determine the ship type
	// and assign the appropriate texture/sprite

	switch (level) {
	case ShipClass::Sloop:
		// Load the values
		baseSpeed = 100.f;
		health = 100;
		regenRate = 1;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_sloop.png")) std::cout << "Failed to load Sloop texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(spriteOrigin);
		sprite.setScale(.1f * scaling, .1f * scaling);
		break;
	case ShipClass::Brigantine:
		// Load the values
		baseSpeed = 95.f;
		health = 133;
		regenRate = 1.48f;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_brigantine.png")) std::cout << "Failed to load Brigantine texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(spriteOrigin);
		sprite.setScale(.12f * scaling, .12f * scaling);
		break;
	case ShipClass::Frigate:
		// Load the values
		baseSpeed = 82.f;
		health = 192.f;
		regenRate = 2.15f;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_frigate.png")) std::cout << "Failed to load Frigate texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(spriteOrigin);
		sprite.setScale(.15f * scaling, .15f * scaling);
		break;
	case ShipClass::ManOWar:
		// Load the values
		baseSpeed = 77.f;
		health = 250.f;
		regenRate = 3.f;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_manowar.png")) std::cout << "Failed to load Man O'War texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(spriteOrigin);
		sprite.setScale(.18f * scaling, .18f * scaling);
		break;
	case ShipClass::Galleon:
		// Load the values
		baseSpeed = 63.f;
		health = 380.f;
		regenRate = 4.6f;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_galleon.png")) std::cout << "Failed to load Galleon texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(spriteOrigin);
		sprite.setScale(.23f * scaling, .23f * scaling);
		break;
	default:
		// Handle other cases
		break;
	}

	// This switch case will determine the ship class
	// and assign the appropriate texture/sprite
	switch (type) {
	case ShipType::Player:
		// Load the texture and values
		break;
	case ShipType::Enemy:
		// Load the texture and values
		break;
	}

	// Set the base speed
	baseSpeed *= 5;

	// Set the maximum health
	maxHealth = health;

	// Set constant values for the sprite len and height
	float spriteLen = sprite.getGlobalBounds().width;
	float spriteHeight = sprite.getGlobalBounds().height;
	constSpriteBounds = sf::Vector2f(spriteLen, spriteHeight);

	// Initalize the position of the ship to be random
	SMH.setPosition(sf::Vector2f(static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000)));

	// Set type and class
	shipType = type;
	shipClass = level;
}

// Draw the ship
void Ship::draw(sf::Vector2f map) {
	// Set the health to 0 if it is negative
	if (health < 0) health = 0;

	// Regen health every 250 milliseconds
	if (health < maxHealth && health >= 0) {
		if (healthRegenClock.getElapsedTime().asMilliseconds() > 250) {
			health += regenRate;
			healthRegenClock.restart();
		}
	}

	// Determine the size of the health bar green based on health
	healthBarGreen.setSize(sf::Vector2f(100 * health / maxHealth, 10));
	healthBarGreen.setFillColor(sf::Color::Green);

	// Determine the size of the health bar red based on health
	healthBarRed.setSize(sf::Vector2f(100, 10));
	healthBarRed.setFillColor(sf::Color::Red);

	// Define the offset from the center of the ship to where the health bar should be
	float healthBarOffsetX = -constSpriteBounds.x / 2; // Offset to the left of the sprite
	float healthBarOffsetY = constSpriteBounds.y / 2; // Offset above the sprite

	// Calculate the health bar's position based on the ship's rotation
	float angleRad = rotation * 3.1415926f / 180.0f;
	sf::Transform rotationTransform;
	rotationTransform.rotate(rotation, sprite.getPosition());

	float x = sprite.getPosition().x + healthBarOffsetX;
	float y = sprite.getPosition().y + healthBarOffsetY;
	sf::Vector2f healthBarPosition = rotationTransform.transformPoint(x, y);

	// Set the position and rotation of the health bars
	healthBarGreen.setPosition(healthBarPosition);
	healthBarRed.setPosition(healthBarPosition);
	healthBarGreen.setRotation(rotation);
	healthBarRed.setRotation(rotation);

	// Draw the health bars only if it is an enemy
	if (shipType == ShipType::Enemy) {
		window->draw(healthBarRed);
		window->draw(healthBarGreen);
	}
	// Move and draw the ship
	sprite.setPosition(SMH.move(baseSpeed));
	window->draw(sprite);
}