#include "WorldESAvoid_PG.h"

using namespace PirateGame;

void ESAvoidWorld::setUpLandMasses() {
	LMH.setPlayerShip(playerShip.get());

	// Add 2 land masses, one behind each ship
	LMH.createIsland(sf::Vector2f(-1000.f, 50.f));
	LMH.createIsland(sf::Vector2f(4000.f, 50.f));
}

void ESAvoidWorld::setUpEnemyShips() {
	ESH.setLandmasses(LMH.getLandMasses());
	ESH.setPlayerShip(playerShip.get());
	ESH.setInteractionChance(0);

	// 2 enemy ships
	ESH.addEnemyShip(sf::Vector2f(1000.f, 50.f));
	ESH.addEnemyShip(sf::Vector2f(2500.f, 50.f));

	// Both ships should be headed at each other
	ESH.getShipGroups().at(0)->setDestination(sf::Vector2f(2500.f, 50.f));
	ESH.getShipGroups().at(1)->setDestination(sf::Vector2f(1000.f, 50.f));
}

void ESAvoidWorld::updateGameLoop(sf::Event event) {
	updateCoreElements();

	ESH.update(WH.getWindDirection(), WH.getWindSpeed());

	CM.handleCollisions();

	view.updateDebugView(event);

	QuadtreeHandler::enemyShipQuadtree->draw();
}