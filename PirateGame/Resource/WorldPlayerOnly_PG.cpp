#include "WorldPlayerOnly_PG.h"

using namespace PirateGame;

void PlayerOnlyWorld::setUpEnemyShips() {
	ESH.setLandmasses(LMH.getLandMasses());
	ESH.setPlayerShip(playerShip.get());
} // No enemy ships in this world

void PlayerOnlyWorld::updateGameLoop(sf::Event event) {
	updateCoreElements();

	LMH.interactWithLandmasses();

	ESH.update();

	CM.handleCollisions();

	playerShip->update();

	view.updateDebugView(event);

	QuadtreeHandler::enemyShipQuadtree->draw(context.GV.get());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		playerShip->getMovementHandler()->setSpeed(playerShip->getMovementHandler()->getSpeed() + 10.f);
	}
}