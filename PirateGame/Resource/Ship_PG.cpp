#include "Ship_PG.h"
using namespace PirateGame;

// Create the ship
void Ship::setUpShip(ShipClass level, const Region region) {
	// If the level is random, generate a random number between 1 and 5
	if (level == ShipClass::Random) {
		shipClass = getRandomShipClass();
	}

	// Set class
	shipProperties = getShipProperties(shipClass);
	health = shipProperties.maxHealth;
	birthRegion = region;

	// Load the texture
	sf::Vector2f scaling(shipProperties.scaleX * scalingFactor, shipProperties.scaleY * scalingFactor);

	sprite.setTexture(context.GTH->getShipTextures().getShipTextureManagerByRegion(birthRegion).getTexture(shipClass));
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	sprite.setScale(scaling);

	// Set the constant sprite bounds
	constSpriteBounds = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

	// Load the cannon handler
	SCH = std::make_unique<ShipCannonHandler>(context.JSL, sprite);
	SCH->initializeCannons(context.GTH->getShipTextures().getCannonTextureManagerByRegion(birthRegion).getTexture(shipClass), 
						context.GTH->getShipTextures().getShipTextureManagerByRegion(region).getImage(shipClass), 
						shipProperties.numCannons, id.get(), scaling);

	// Load the sail handler
	SSH = std::make_unique<ShipSailHandler>();
	SSH->loadSailPositions(context.GTH->getShipTextures().getSailTextureManagerByRegion(birthRegion).getTextureGroup(shipClass), context.GTH->getShipTextures().getShipTextureManagerByRegion(birthRegion).getImage(shipClass), scaling);

	// Execute custom ship setup
	customShipSetUp();
}

// Draw and update the ship
void Ship::update() {
	setHealthBarPosition();
	regenerateHealth();

	// Update handlers
	SCH->updateCannons(context.GV->getWindow(), context.GC->getDeltaTime());
	SSH->update(sprite, SMH->getVelocity(), context.JSL->getGameData().gameConfig.shipData.maxSailRotationOffset);
	SIH->update(context.GTH->getLandMassTextures().getMiscTextures().getTexture(MiscType::Cannonball), context.GIDM.get());
	SMH->update(SSH->getAverageSailDirection(), context.GC->getDeltaTime(), context.GWC->getWindDirection(), context.GWC->getWindSpeed());

	// Execute custom ship update
	customShipUpdate();
}

void Ship::draw() {
	context.GV->getWindow()->draw(sprite);

	// Draw sails
	SSH->draw(context.GV->getWindow());

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
	healthBarGreen.setSize(sf::Vector2f(-context.JSL->getGameData().gameConfig.shipData.shipHealthBarSize.x * health / shipProperties.maxHealth, context.JSL->getGameData().gameConfig.shipData.shipHealthBarSize.y));
	healthBarGreen.setFillColor(sf::Color::Green);

	// Determine the size of the health bar red based on health
	healthBarRed.setSize(healthBarGreen.getSize());
	healthBarRed.setFillColor(sf::Color::Red);

	// Define the offset from the center of the ship to where the health bar should be
	sf::Vector2f healthBarOffset(-1 * constSpriteBounds.x / 2, constSpriteBounds.y / 2);

	// Calculate the health bar's position based on the ship's rotation
	float rotation = sprite.getRotation();
	sf::Transform rotationTransform;
	rotationTransform.rotate(rotation, sprite.getPosition());

	sf::Vector2f rotationPoint(sprite.getPosition().x  + healthBarRed.getSize().x / 2, sprite.getPosition().y + healthBarOffset.y + healthBarRed.getSize().y);
	sf::Vector2f healthBarPosition = rotationTransform.transformPoint(rotationPoint);

	// Set the position and rotation of the health bars
	healthBarGreen.setPosition(healthBarPosition);
	healthBarRed.setPosition(healthBarPosition );
	healthBarGreen.setRotation(rotation);
	healthBarRed.setRotation(rotation);
}
