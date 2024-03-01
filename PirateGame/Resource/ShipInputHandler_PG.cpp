#include "ShipInputHandler_PG.h"

using namespace PirateGame;

void ShipInputHandler::update() {
	handleCannonFire();
	handleCannonAim();
	handleAnchorDrop();

	// Update the ship cannon handler
	SCH.updateCannonballs();

	// Update the ship movement handler
	SMH.move(baseSpeed);
}

void ShipInputHandler::handleCannonFire() {
	// Get the input handler
	InputHandler& inputHandler = GlobalValues::getInstance().getInputHandler();

	// Fire the cannons
	if (inputHandler.isKeyPressedOnce(sf::Keyboard::Space)) {
		SCH.shootCannonballs(numCannons);
	}
}

void ShipInputHandler::handleCannonAim() {
	// Get the input handler
	InputHandler& inputHandler = GlobalValues::getInstance().getInputHandler();

	// Set the firing side of the ship
	if (inputHandler.isMouseButtonPressedOnce(portMouseButton)) {
		SCH.setFiringSide(FiringSide::Port);
	}
	else if (inputHandler.isMouseButtonPressedOnce(starboardMouseButton)) {
		SCH.setFiringSide(FiringSide::Starboard);
	}

	// Rotate the cannons based on the mouse position if cannon mode is set to manual
	SCH.setAimTowardsMouse(inputHandler.isKeyToggled(manualAimKey));
	SMH.setStopShipRotationFlag(inputHandler.isKeyToggled(manualAimKey));
}

void ShipInputHandler::handleAnchorDrop() {
	//...
}