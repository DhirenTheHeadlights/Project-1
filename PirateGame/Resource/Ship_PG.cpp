#include "Ship_PG.h"

using namespace PirateGame;

// Create the ship
void Ship::setUpShip(ShipClass level) {
	// Set the unique ID of the ship
	ID = context.GIDM->getUniqueID();

	// If the level is random, generate a random number between 1 and 5
	if (level == ShipClass::Random) {
		// Generate a random number between 1 and 5
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(1, 5);
		level = static_cast<ShipClass>(dis(gen));

		// Access ship properties from the configuration map using the generated random number
		shipProperties = getShipProperties(level);
	}
	else {
		// Access ship properties from the configuration map using the provided ship class
		shipProperties = getShipProperties(level);
	}

	// Set class
	shipClass = level;

	health = shipProperties.maxHealth;

	// Load the texture
	sf::Vector2f scaling(shipProperties.scaleX * scalingFactor, shipProperties.scaleY * scalingFactor);

	sprite.setTexture(context.GTH->getShipTextures().getShipTextureManager().getTexture(level));
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	sprite.setScale(scaling);

	// Set the constant sprite bounds
	constSpriteBounds = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

	// Load the cannon handler
	SCH = std::make_unique<ShipCannonHandler>(sprite);
	SCH->initializeCannons(context.GTH->getShipTextures().getCannonTextureManager().getTexture(shipClass), 
						context.GTH->getShipTextures().getShipTextureManager().getImage(shipClass), 
						shipProperties.numCannons, ID, scaling);

	// Load the sail handler
	SSH = std::make_unique<ShipSailHandler>();
	SSH->loadSailPositions(context.GTH->getShipTextures().getSailTextureManager().getTextureGroup(shipClass), context.GTH->getShipTextures().getShipTextureManager().getImage(shipClass), scaling);

	// Execute custom ship setup
	customShipSetUp();
}

// Draw and update the ship
void Ship::update() {
	setHealthBarPosition();
	regenerateHealth();

	// Update handlers
	SCH->updateCannons(context.GV->getWindow(), context.GV->getGlobalClock().getElapsedTime().asSeconds());
	sf::Vector2f shipDirection = SMH->getVelocity();
	SSH->update(sprite, shipDirection);

	// Execute custom ship update
	customShipUpdate();
}

void Ship::draw() {
	sf::RenderWindow* window = context.GV->getWindow();

	window->draw(sprite);

	// Draw sails
	SSH->draw(window);

	// Custom ship draw
	customShipDraw();
}

// Regen Health
void Ship::regenerateHealth() {
	// Regen health
	if (healthRegenClock.getElapsedTime().asMilliseconds() > regenTime.asMilliseconds()) {
		health += shipProperties.regenRate;
		healthRegenClock.restart();
	}
	// Restart the damage clock too
	if (deltaTime.getElapsedTime().asSeconds() > 1) {
		deltaTime.restart();
	}
	if (health > shipProperties.maxHealth) health = shipProperties.maxHealth;
}

// Draw the health bars
void Ship::setHealthBarPosition() {
	// Determine the size of the health bar green based on health
	healthBarGreen.setSize(sf::Vector2f(-healthBarSize.x * health / shipProperties.maxHealth, healthBarSize.y));
	healthBarGreen.setFillColor(sf::Color::Green);

	// Determine the size of the health bar red based on health
	healthBarRed.setSize(sf::Vector2f( - healthBarSize.x, healthBarSize.y));
	healthBarRed.setFillColor(sf::Color::Red);

	// Define the offset from the center of the ship to where the health bar should be
	sf::Vector2f healthBarOffset(-1 * constSpriteBounds.x / 2, constSpriteBounds.y / 2);

	// Calculate the health bar's position based on the ship's rotation
	float rotation = sprite.getRotation();
	float angleRad = rotation * 3.1415926f / 180.0f;
	sf::Transform rotationTransform;
	rotationTransform.rotate(rotation, sprite.getPosition());

	sf::Vector2f rotationPoint(sprite.getPosition().x  + healthBarSize.x / 2, sprite.getPosition().y + healthBarOffset.y  + healthBarSize.y);
	sf::Vector2f healthBarPosition = rotationTransform.transformPoint(rotationPoint);

	// Set the position and rotation of the health bars
	healthBarGreen.setPosition(healthBarPosition);
	healthBarRed.setPosition(healthBarPosition );
	healthBarGreen.setRotation(rotation);
	healthBarRed.setRotation(rotation);
}
