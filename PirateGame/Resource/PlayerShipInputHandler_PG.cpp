#include "PlayerShipInputHandler_PG.h"

using namespace PirateGame;

void PlayerShipInputHandler::handleCannonFire(const sf::Texture& cannonballTexture, GlobalIDManager* GIDM) {
	// Fire the cannons
	if (Input::isKeyPressedOnce(fireKey) && cannonCooldownClock.getElapsedTime().asSeconds() > cooldown.asSeconds()) {
		SCH->shootCannonballs(cannonballTexture, GIDM);
		cannonCooldownClock.restart();
		if (inAudioRange) GSM->playSound(SoundId::CannonShot);
	}
}

void PlayerShipInputHandler::handleCannonAim() {
	// Set the firing side of the ship
	if (Input::isMouseButtonPressedOnce(portMouseButton)) {
		SCH->setFiringSide(FiringSide::Port);
	}
	if (Input::isMouseButtonPressedOnce(starboardMouseButton)) {
		SCH->setFiringSide(FiringSide::Starboard);
	}

	// Rotate the cannons based on the mouse position if cannon mode is set to manual
	if (Input::isKeyToggled(manualAimKey)) {
		SCH->setFiringState(FiringState::TowardsMouse);
		SMH->setStopShipRotationFlag(true);
	}
	else {
		SCH->setFiringState(FiringState::Untargeted);
		SMH->setStopShipRotationFlag(false);
	}
}

void PlayerShipInputHandler::handleAnchorDrop() {
	if (Input::isKeyPressedOnce(anchorDropKey)) {
		SMH->setAnchorDrop(!SMH->getDroppedAnchor());
	}
}

void PlayerShipInputHandler::handleSailChange(const sf::Vector2f& windDirection) {
	if (Input::isKeyHeld(sailUpKey)) {
		SSH->moveSailsUp(sailUpKey);
	}
	if (Input::isKeyHeld(sailDownKey)) {
		SSH->moveSailsDown(sailDownKey);
	}
	if (Input::isKeyHeld(sailLeftKey)) {
		SSH->moveSailsLeft(sailLeftKey, json->getGameData().gameConfig.shipData.sailRotationSpeed);
	}
	if (Input::isKeyHeld(sailRightKey)) {
		SSH->moveSailsRight(sailRightKey, json->getGameData().gameConfig.shipData.sailRotationSpeed);
	}

	SSH->moveSailLeftRightAutomatically(windDirection, sprite.getRotation(), json->getGameData().gameConfig.shipData.sailRotationSpeed);
}