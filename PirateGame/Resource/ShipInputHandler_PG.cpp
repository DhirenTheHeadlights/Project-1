#include "ShipInputHandler_PG.h"

using namespace PirateGame;

void ShipInputHandler::update(const sf::Texture& cannonballTexture, GlobalIDManager* GIDM, const sf::Vector2f windDirection, const float windSpeed) {
	handleCannonFire(cannonballTexture, GIDM);
	handleCannonAim();
	handleAnchorDrop();
	handleSailChange(windDirection);
}