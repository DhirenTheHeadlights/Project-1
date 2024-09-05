#include "WorldDebug_PG.h"

using namespace PirateGame;

void DefaultDebugWorld::updateGameLoop(sf::Event event) {
	updateCoreElements();

	LMH.interactWithLandmasses();

	ESH.update();

	CM.handleCollisions(GQH.get());

	playerShip->update();

	view.updateDebugView(event);

	GQH->getEnemyShipQuadtree()->draw(context.GV.get());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		playerShip->getMovementHandler()->setSpeed(playerShip->getMovementHandler()->getSpeed() + 10.f);
	}
}