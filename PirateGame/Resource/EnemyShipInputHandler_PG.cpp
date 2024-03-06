#include "EnemyShipInputHandler_PG.h"

using namespace PirateGame;

void EnemyShipInputHandler::handleCannonFire() {
	// Fire the cannons
	SCH->shootCannonballs(numCannons);
}

void EnemyShipInputHandler::handleCannonAim() {
	//...
}

void EnemyShipInputHandler::handleAnchorDrop() {
	//...
}