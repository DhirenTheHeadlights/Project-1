#include "ShipInputHandler_PG.h"

using namespace PirateGame;

void ShipInputHandler::update() {
	handleCannonFire();
	handleCannonAim();
	handleAnchorDrop();

	// Update the handlers
	SMH->move(baseSpeed);
	SCH->updateCannons();
}

void ShipInputHandler::draw() {
	SCH->drawCannons();
}