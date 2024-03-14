#include "EnemyShipInputHandler_PG.h"

using namespace PirateGame;

void EnemyShipInputHandler::handleCannonFire() {
	// Fire the cannons
	
	SCH->shootCannonballs(numCannons, getMovementHandler()->getSprite().getPosition() - playerPos);
}

void EnemyShipInputHandler::handleCannonAim() {
	//...
}

void EnemyShipInputHandler::handleAnchorDrop() {
	//...
}