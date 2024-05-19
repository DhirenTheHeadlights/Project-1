#include "PlayerShipInputHandler_PG.h"

using namespace PirateGame;

void PlayerShipInputHandler::handleCannonFire() {
	// Grab Globals
	GlobalInputHandler& inputHandler = GlobalInputHandler::getInstance();
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();

	// Fire the cannons
	if (inputHandler.isKeyPressedOnce(fireKey)) {
		SCH->shootCannonballs();
	}
}

void PlayerShipInputHandler::handleCannonAim() {
	// Get the input handler
	GlobalInputHandler& inputHandler = GlobalInputHandler::getInstance();

	// Set the firing side of the ship
	if (inputHandler.isMouseButtonPressedOnce(portMouseButton)) {
		SCH->setFiringSide(FiringSide::Port);
	}
	if (inputHandler.isMouseButtonPressedOnce(starboardMouseButton)) {
		SCH->setFiringSide(FiringSide::Starboard);
	}

	// Rotate the cannons based on the mouse position if cannon mode is set to manual
	if (inputHandler.isKeyToggled(manualAimKey)) {
		SCH->setFiringState(FiringState::TowardsMouse);
		SMH->setStopShipRotationFlag(true);
	}
	else {
		SCH->setFiringState(FiringState::Untargeted);
		SMH->setStopShipRotationFlag(false);
	}
}

void PlayerShipInputHandler::handleAnchorDrop() {
	if (GlobalInputHandler::getInstance().isKeyPressedOnce(anchorDropKey)) {
		SMH->setAnchorDrop(!SMH->getDroppedAnchor());
	}
}

void PlayerShipInputHandler::handleSailChange() {
	SSH->moveSailLeftRightManually(sailLeftKey, sailRightKey);
	SSH->moveSailsUpAndDown(sailUpKey, sailDownKey);

	SSH->moveSailLeftRightAutomatically(GlobalWindController::getInstance().getWindDirection(), SMH->getVelocity());
}