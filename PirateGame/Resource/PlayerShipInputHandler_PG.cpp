#include "PlayerShipInputHandler_PG.h"

using namespace PirateGame;

void PlayerShipInputHandler::handleCannonFire() {
	// Get the input handler
	InputHandler& inputHandler = GlobalValues::getInstance().getInputHandler();

	// Fire the cannons
	if (inputHandler.isKeyPressedOnce(fireKey)) {
		SCH->shootCannonballs(numCannons);
	}
}

void PlayerShipInputHandler::handleCannonAim() {
	// Get the input handler
	InputHandler& inputHandler = GlobalValues::getInstance().getInputHandler();

	// Set the firing side of the ship
	if (inputHandler.isMouseButtonPressedOnce(portMouseButton)) {
		SCH->setFiringSide(FiringSide::Port);
	}
	if (inputHandler.isMouseButtonPressedOnce(starboardMouseButton)) {
		SCH->setFiringSide(FiringSide::Starboard);
	}

	// Rotate the cannons based on the mouse position if cannon mode is set to manual
	SCH->setAimTowardsMouse(inputHandler.isKeyToggled(manualAimKey));
	SMH->setStopShipRotationFlag(inputHandler.isKeyToggled(manualAimKey));
}

void PlayerShipInputHandler::handleAnchorDrop() {
	if (GlobalValues::getInstance().getInputHandler().isKeyPressedOnce(anchorDropKey)) {
		SMH->setAnchorDrop(!SMH->getDroppedAnchor());
	}
}