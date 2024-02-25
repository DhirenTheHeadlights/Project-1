#include "Ship_PG.h"

using namespace PirateGame;

std::unordered_map<ShipClass, ShipProperties> Ship::shipConfig = {
{ ShipClass::Sloop, {100.f, 100.f, 1, "PirateGameSprites/pg_ship_sloop.png", .1f, .1f, 1} },
{ ShipClass::Brigantine, {95.f, 133.f, 1.48f, "PirateGameSprites/pg_ship_brigantine.png", .12f, .12f, 2} },
{ ShipClass::Frigate, {82.f, 192.f, 2.15f, "PirateGameSprites/pg_ship_frigate.png", .15f, .15f, 3} },
{ ShipClass::ManOWar, {77.f, 250.f, 3.f, "PirateGameSprites/pg_ship_manowar.png", .18f, .18f, 4} },
{ ShipClass::Galleon, {63.f, 380.f, 4.6f, "PirateGameSprites/pg_ship_galleon.png", .23f, .23f, 5} }
};

// Create the ship
void Ship::createShip(ShipType type, ShipClass level) {
	// Access ship properties from the configuration map using the provided ship class
	shipProperties = shipConfig[level];

	// Load the texture
	if (!texture.loadFromFile(shipProperties.texturePath)) {
		std::cout << "Failed to load texture: " << shipProperties.texturePath << std::endl;
	}
	else {
		sf::Vector2f scaling(shipProperties.scaleX * scalingFactor, shipProperties.scaleY * scalingFactor);
		constSpriteBounds = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

		sprite.setTexture(texture);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
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

// Draw and update the ship
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
	SMH.move(shipProperties.baseSpeed);
	window->draw(sprite);

	// Set the firing side of the ship
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) SCH.setFiringSide(FiringSide::Left);
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) SCH.setFiringSide(FiringSide::Right);

	// Fire the cannons
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
	//if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		SCH.shootCannonballs(shipProperties.numCannons);
	}

	SCH.updateCannonballs();
}

// Regen Health
void Ship::regenerateHealth() {
	// Regen health every 250 milliseconds
	if (!(health < shipProperties.maxHealth && health >= 0.f)) return;
	if (healthRegenClock.getElapsedTime().asMilliseconds() > 250) {
		health += shipProperties.regenRate;
		healthRegenClock.restart();
	}
}

// Draw the health bars
void Ship::setHealthBarPosition() {
	// Determine the size of the health bar green based on health
	healthBarGreen.setSize(sf::Vector2f(100.f * health / shipProperties.maxHealth, 10));
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