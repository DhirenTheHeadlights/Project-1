#include "PlayerShipInputHandler_PG.h"

using namespace PirateGame;

void PlayerShipInputHandler::handleCannonFire(const sf::Texture& cannonballTexture, GlobalIDManager* GIDM) {
	// Fire the cannons
	if (GIH->isKeyPressedOnce(fireKey) && cannonCooldownClock.getElapsedTime().asSeconds() > cooldown.asSeconds()) {
		SCH->shootCannonballs(cannonballTexture, GIDM);
		cannonCooldownClock.restart();
		if (inAudioRange) GSM->playSound(SoundId::CannonShot);
	}
}

void PlayerShipInputHandler::handleCannonAim() {
	// Set the firing side of the ship
	if (GIH->isMouseButtonPressedOnce(portMouseButton)) {
		SCH->setFiringSide(FiringSide::Port);
	}
	if (GIH->isMouseButtonPressedOnce(starboardMouseButton)) {
		SCH->setFiringSide(FiringSide::Starboard);
	}

	// Rotate the cannons based on the mouse position if cannon mode is set to manual
	if (GIH->isKeyToggled(manualAimKey)) {
		SCH->setFiringState(FiringState::TowardsMouse);
		SMH->setStopShipRotationFlag(true);
	}
	else {
		SCH->setFiringState(FiringState::Untargeted);
		SMH->setStopShipRotationFlag(false);
	}
}

void PlayerShipInputHandler::handleAnchorDrop() {
	if (GIH->isKeyPressedOnce(anchorDropKey)) {
		SMH->setAnchorDrop(!SMH->getDroppedAnchor());
	}
}

void PlayerShipInputHandler::handleSailChange() {
	if (GIH->isKeyHeld(sailUpKey)) {
		SSH->moveSailsUp(sailUpKey);
	}
	if (GIH->isKeyHeld(sailDownKey)) {
		SSH->moveSailsDown(sailDownKey);
	}
	if (GIH->isKeyHeld(sailLeftKey)) {
		SSH->moveSailsLeft(sailLeftKey, json->getGameData().gameConfig.shipData.sailRotationSpeed);
	}
	if (GIH->isKeyHeld(sailRightKey)) {
		SSH->moveSailsRight(sailRightKey, json->getGameData().gameConfig.shipData.sailRotationSpeed);
	}

	SSH->moveSailLeftRightAutomatically(GWC->getWindDirection(), sprite.getRotation(), json->getGameData().gameConfig.shipData.sailRotationSpeed);
}