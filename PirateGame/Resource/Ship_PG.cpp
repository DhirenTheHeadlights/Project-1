#include "Ship_PG.h"

using namespace PirateGame;

std::unordered_map<ShipClass, ShipProperties> Ship::shipConfig = {
{ ShipClass::Sloop, {100.f, 100.f, 1, "PirateGameSprites/pg_ship_sloop.png", .1f, .1f} },
{ ShipClass::Brigantine, {95.f, 133.f, 1.48f, "PirateGameSprites/pg_ship_brigantine.png", .12f, .12f} },
{ ShipClass::Frigate, {82.f, 192.f, 2.15f, "PirateGameSprites/pg_ship_frigate.png", .15f, .15f} },
{ ShipClass::ManOWar, {77.f, 250.f, 3.f, "PirateGameSprites/pg_ship_manowar.png", .18f, .18f} },
{ ShipClass::Galleon, {63.f, 380.f, 4.6f, "PirateGameSprites/pg_ship_galleon.png", .23f, .23f} }
};

// Create the ship
void Ship::createShip(ShipType type, ShipClass level) {
	// Access ship properties from the configuration map using the provided ship class
	const ShipProperties& props = shipConfig[level];

	// Assign values from ShipProperties to the Ship's member variables
	baseSpeed = props.baseSpeed * 2.f;
	health = props.health;
	maxHealth = props.health;
	regenRate = props.regenRate;
	sf::Vector2f scaling(props.scaleX * scalingFactor, props.scaleY * scalingFactor);
	sf::Vector2f spriteOrigin = props.origin;
	constSpriteBounds = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

	// Load the texture
	if (!texture.loadFromFile(props.texturePath)) {
		std::cout << "Failed to load texture: " << props.texturePath << std::endl;
	}
	else {
		sprite.setTexture(texture);
		sprite.setOrigin(spriteOrigin); // Make sure spriteOrigin is properly initialized
		sprite.setScale(scaling);
	}

	switch (type) {
	case ShipType::Player:
		// Load the texture and values
		break;
	case ShipType::Enemy:
		// Load the texture and values
		break;
	}

	// Initalize the position of the ship to be random
	SMH.setPosition(sf::Vector2f(static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000)));

	// Set type and class
	shipType = type;
	shipClass = level;
}

// Draw the ship
void Ship::updateAndDraw() {

	window = GlobalValues::getInstance().getWindow();

	regenerateHealth();

	// Draw the health bars only if it is an enemy
	if (shipType == ShipType::Enemy) {
		window->draw(healthBarRed);
		window->draw(healthBarGreen);
	}

	// Draw the velocity vector
	window->draw(createVector(sprite.getPosition(), SMH.getVelocity(), sf::Color::Blue));

	// Move and draw the ship
	sprite.setPosition(SMH.move(baseSpeed));
	window->draw(sprite);
}

// Regen Health
void Ship::regenerateHealth() {
	// Regen health every 250 milliseconds
	if (!(health < maxHealth && health >= 0)) return;
	if (healthRegenClock.getElapsedTime().asMilliseconds() > 250) {
		health += regenRate;
		healthRegenClock.restart();
	}
}

// Draw the health bars
void Ship::setHealthBarPosition() {
	// Determine the size of the health bar green based on health
	healthBarGreen.setSize(sf::Vector2f(100 * health / maxHealth, 10));
	healthBarGreen.setFillColor(sf::Color::Green);

	// Determine the size of the health bar red based on health
	healthBarRed.setSize(sf::Vector2f(100, 10));
	healthBarRed.setFillColor(sf::Color::Red);

	// Define the offset from the center of the ship to where the health bar should be
	sf::Vector2f healthBarOffset(-1 * constSpriteBounds.x / 2, constSpriteBounds.y / 2);

	// Calculate the health bar's position based on the ship's rotation
	float angleRad = rotation * 3.1415926f / 180.0f;
	sf::Transform rotationTransform;
	rotationTransform.rotate(rotation, sprite.getPosition());

	sf::Vector2f rotationPoint(sprite.getPosition().x + healthBarOffset.x, sprite.getPosition().y + healthBarOffset.y);
	sf::Vector2f healthBarPosition = rotationTransform.transformPoint(rotationPoint);

	// Set the position and rotation of the health bars
	healthBarGreen.setPosition(healthBarPosition);
	healthBarRed.setPosition(healthBarPosition);
	healthBarGreen.setRotation(rotation);
	healthBarRed.setRotation(rotation);
}

sf::VertexArray Ship::createVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color) {
	// Create a VertexArray with two points
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = start;
	line[1].position = start + vector;
	line[0].color = color;
	line[1].color = color;
	return line;
}