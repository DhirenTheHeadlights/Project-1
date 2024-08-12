#include "ShipInputHandler_PG.h"

using namespace PirateGame;

void ShipInputHandler::update(const sf::Texture& cannonballTexture, GlobalIDManager* GIDM) {
	handleCannonFire(cannonballTexture, GIDM);
	handleCannonAim();
	handleAnchorDrop();
	handleSailChange();
}