#include "World_PG.h"

using namespace PirateGame;

void World::setUpWorld() {
	setUpWorldElements();
	setUpPlayerShip();
	setUpLandMasses();
	setUpEnemyShips();
	setUpCollisionManager();
	setUpMenus();
	setUpUI();
}

void World::setUpWorldElements() {
	// Set up the window and map/view
	GlobalValues::getInstance().setWindow(window);
	GlobalChunkHandler::getInstance().initializeMap();
	view.setUpView();

	// Set up the background
	waterTiler.initialize();
}

void World::setUpPlayerShip() {
	playerShip = std::make_unique<PlayerShip>();
	playerShip->setUpShip(ShipClass::ManOWar);
	playerShip->getCannonHandler()->setCannonballHashmap(GlobalQuadtreeHandler::getInstance().getCannonballQuadtree());
}

void World::setUpLandMasses() {
	LMH.setPlayerShip(playerShip.get());
	LMH.addLandMasses(numLandMasses, distanceBetweenLandMasses);
}

void World::setUpEnemyShips() {
	ESH.setLandmasses(LMH.getLandMasses());
	ESH.addEnemyShips(numEnemyShips);
	ESH.setPlayerShip(playerShip.get());
}

void World::setUpCollisionManager() {
	CM.setPlayerShip(playerShip.get());
	CM.setEnemyShips(ESH.getEnemyShips());
	CM.setLandMasses(LMH.getLandMasses());
}

void World::setUpMenus() {
	MH.createMenus();
	
	// Set up the hud
	MH.getHUD()->getMinimap().setLandmasses(LMH.getLandMasses());
	MH.getHUD()->getMinimap().setEnemyShips(ESH.getEnemyShips());
	MH.getHUD()->setPlayerShip(*playerShip.get());

	// Set up the inventory
	MH.getInventoryMenu()->setPlayerShip(playerShip.get());
	MH.setUpMenus();
}

void World::setUpUI() {
	// Set up the frame rate text
	frameRateText.setFont(*GlobalFontHandler::getInstance().getGlobalFont());
	frameRateText.setCharacterSize(24u);
	frameRateText.setFillColor(sf::Color::White);

	// Set up the experience text
	experience.setFont(*GlobalFontHandler::getInstance().getGlobalFont());
	experience.setCharacterSize(24u);
	experience.setFillColor(sf::Color::White);
}

void World::drawGameLoop() {
	waterTiler.draw(window);
	LMH.drawLandMasses();
	window->draw(frameRateText);
	window->draw(experience);
	playerShip->draw();
	ESH.draw();
}

void World::updateCoreElements() {
	GlobalValues::getInstance().getGlobalClock().restart();
	GlobalChunkHandler::getInstance().updateChunks(playerShip->getSprite().getPosition());
	GlobalWindController::getInstance().update();
	GlobalQuadtreeHandler::getInstance().updateQuadtrees();
	view.showCoordsOnCursor();
	waterTiler.update();
}

