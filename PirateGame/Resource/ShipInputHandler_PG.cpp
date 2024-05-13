#include "ShipInputHandler_PG.h"

using namespace PirateGame;

void ShipInputHandler::update() {
	handleCannonFire();
	handleCannonAim();
	handleAnchorDrop();
	handleSailChange();
}