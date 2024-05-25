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
	GlobalMap::getInstance().initializeMap();
	view.setUpView();

	// Set up the background
	background.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	background.setFillColor(backgroundColor);
}

void World::setUpPlayerShip() {
	playerShip = std::make_unique<PlayerShip>();
	playerShip->setUpShip(ShipClass::ManOWar);
	playerShip->getCannonHandler()->setCannonballHashmap(GlobalHashmapHandler::getInstance().getCannonballHashmap());
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
	MH.setUpMenus();
	
	// Set up the hud
	MH.getHUD()->getMinimap().setLandmasses(LMH.getLandMasses());
	MH.getHUD()->getMinimap().setEnemyShips(ESH.getEnemyShips());
	MH.getHUD()->setPlayerShip(*playerShip.get());
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
	window->draw(background);
	LMH.drawLandMasses();
	window->draw(frameRateText);
	window->draw(experience);
	playerShip->draw();
	ESH.draw();
}

