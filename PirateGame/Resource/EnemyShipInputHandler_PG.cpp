#include "EnemyShipInputHandler_PG.h"

using namespace PirateGame;

void EnemyShipInputHandler::handleCannonFire() {
	// Only fire if the player is within firingRange
	float distance = sqrt(pow(playerPos.x - getMovementHandler()->getSprite().getPosition().x, 2) + pow(playerPos.y - getMovementHandler()->getSprite().getPosition().y, 2));
	if (distance > firingDistance) {
		return;
	}
	SCH->shootCannonballs(numCannons, getMovementHandler()->getSprite().getPosition() - playerPos);
}

void EnemyShipInputHandler::handleCannonAim() {
	// Set the firing side of the ship based on the player's position
	if (playerPos.x < getMovementHandler()->getSprite().getPosition().x) {
		SCH->setFiringSide(FiringSide::Port);
	}
	else {
		SCH->setFiringSide(FiringSide::Starboard);
	}
}

void EnemyShipInputHandler::handleAnchorDrop() {
	//...
}