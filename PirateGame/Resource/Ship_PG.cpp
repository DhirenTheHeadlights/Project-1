#include "Ship_PG.h"

using namespace PirateGame;

std::unordered_map<ShipClass, ShipProperties> Ship::ShipConfig = {
{ ShipClass::Sloop,		 {100.f, 100.f, 1,	   GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getTexture(ShipClass::Sloop),   .1f,  .1f,  2}},
{ ShipClass::Brigantine, {95.f,  133.f, 1.48f, GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getTexture(ShipClass::Brigantine),    .12f, .12f, 4}},
{ ShipClass::Frigate,	 {82.f,  192.f, 2.15f, GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getTexture(ShipClass::Frigate), .15f, .15f, 6}},
{ ShipClass::ManOWar,	 {77.f,  250.f, 3.f,   GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getTexture(ShipClass::ManOWar), .18f, .18f, 8}},
{ ShipClass::Galleon,	 {63.f,  380.f, 4.6f,  GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getTexture(ShipClass::Galleon), .23f, .23f, 10}}
};

// Create the ship
void Ship::setUpShip(ShipClass level) {
	// Set the unique ID of the ship
	ID = GlobalIDManager::getInstance().getUniqueID();

	// If the level is random, generate a random number between 1 and 5
	if (level == ShipClass::Random) {
		// Generate a random number between 1 and 5
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(1, 5);
		level = static_cast<ShipClass>(dis(gen));

		// Access ship properties from the configuration map using the generated random number
		shipProperties = ShipConfig[level];
	}
	else {
		// Access ship properties from the configuration map using the provided ship class
		shipProperties = ShipConfig[level];
	}

	health = shipProperties.maxHealth;

	// Load the texture
	sf::Vector2f scaling(shipProperties.scaleX * scalingFactor, shipProperties.scaleY * scalingFactor);

	sprite.setTexture(shipProperties.texture);
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	sprite.setScale(scaling);

	// Set the constant sprite bounds
	constSpriteBounds = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

	// Load the cannon handler
	SCH = std::make_unique<ShipCannonHandler>(sprite);
	SCH->initializeCannons(level, shipProperties.numCannons, ID, scaling);

	// Load the sail handler
	SSH = std::make_unique<ShipSailHandler>(sprite);
	SSH->loadSailPositions(level, scaling);

	// Set type and class
	shipClass = level;

	// Execute custom ship setup
	customShipSetUp();
}

// Draw and update the ship
void Ship::update() {
	setHealthBarPosition();
	regenerateHealth();

	// Update handlers
	SCH->updateCannons();
	sf::Vector2f shipDirection = SMH->getVelocity();
	SSH->update(sprite, shipDirection);

	// Execute custom ship update
	customShipUpdate();
}

void Ship::draw() {
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

	window->draw(sprite);

	// Draw sails
	SSH->draw();

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
