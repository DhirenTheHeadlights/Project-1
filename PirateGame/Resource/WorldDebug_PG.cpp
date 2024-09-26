#include "WorldDebug_PG.h"

using namespace PirateGame;

void DefaultDebugWorld::updateGameLoop(sf::Event event) {
	updateCoreElements();

	LMH.interactWithLandmasses();

	ESH.update(WH.getWindDirection(), WH.getWindSpeed());

	CM.handleCollisions();

	playerShip->update(WH.getWindDirection(), WH.getWindSpeed());

	view.updateDebugView(event);

	QuadtreeHandler::enemyShipQuadtree->draw();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		playerShip->getMovementHandler()->setSpeed(playerShip->getMovementHandler()->getSpeed() + 10.f);
	}
}