#include "EnemyShip_PG.h"

using namespace PirateGame;

void EnemyShip::customShipSetUp() {
	// Add handlers
	SIH = std::make_unique<EnemyShipInputHandler>(getSprite(), context.GWC.get(), context.GSM.get());
	SMH = std::make_unique<EnemyShipMovementHandler>(getSprite());

	SIH->setBaseSpeed(getSpecificShipProperties().baseSpeed);
	SIH->setCannonHandler(getCannonHandler());
	SIH->setMovementHandler(SMH.get());
	SIH->setSailHandler(getSailHandler());
}

void EnemyShip::customShipUpdate() {
	SIH->update(context.GTH->getLandMassTextures().getMiscTextures().getTexture(MiscType::Cannonball), context.GIDM.get());
	SMH->update(getSpecificShipProperties().baseSpeed, SSH->getAverageSailDirection(), context.GV->getGlobalClock().getElapsedTime().asSeconds(), context.GWC->getWindDirection(), context.GWC->getWindSpeed());
}

void EnemyShip::customShipDraw() {
	// Draw the health bar
	sf::RenderWindow* window = context.GV->getWindow();
	window->draw(healthBarRed);
	window->draw(healthBarGreen);

	getCannonHandler()->drawCannons(window);
	getSailHandler()->draw(window);

	if (debug) {
		context.GV->displayText("Health: " + std::to_string(getSpecificShipProperties().maxHealth), getSprite().getPosition() + sf::Vector2f(0, 50), sf::Color::White);
	}
}

