#include "PlayerShipInputHandler_PG.h"

using namespace PirateGame;

void ShipInputHandler::update() {
	handleCannonFire();
	handleCannonAim();
	handleAnchorDrop();

	// Update the handlers
	SMH->move(baseSpeed);
	SCH->updateCannonballs();
}

void ShipInputHandler::draw() {
	SCH->drawCannonballs();
}