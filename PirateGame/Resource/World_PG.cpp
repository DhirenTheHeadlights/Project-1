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
	// Set up the window and map
	context.GV->setWindow(window);
	context.GCH->initializeMap();
	view = View(window);

	// Set up the background
	waterTiler.initialize();

	// Set up qth
	GQH->setUpQuadtrees(context.GCH.get());
}

void World::setUpPlayerShip() {
	playerShip = std::make_unique<PlayerShip>(context);
	playerShip->setUpShip(ShipClass::ManOWar);
	playerShip->getCannonHandler()->setCannonballQuadtree(GQH->getCannonballQuadtree());
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
	frameRateText.setFont(*context.GFH->getGlobalFont());
	frameRateText.setCharacterSize(24u);
	frameRateText.setFillColor(sf::Color::White);

	// Set up the experience text
	experience.setFont(*context.GFH->getGlobalFont());
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
	context.GTQP->drawTextQueue(context.GV->getWindow());
}

void World::updateCoreElements() {
	context.GC->update();
	context.GCH->updateChunks(context.GV->getWindow(), playerShip->getSprite().getPosition());
	context.GWC->update();
	GQH->updateQuadtrees();
	context.GTQP->updateTextQueue(window);
	view.showCoordsOnCursor(*context.GFH->getGlobalFont());
	waterTiler.update();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		playerShip->getMovementHandler()->setSpeed(playerShip->getMovementHandler()->getSpeed() * 2.f);
	}
}

