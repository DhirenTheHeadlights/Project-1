#include "Shipwreck_PG.h"

using namespace PirateGame;

void Shipwreck::createLandMass() {
	LandMassTypeGenerator<ShipwreckType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getShipwreckTextures().getTexture(type));
	sprite.setScale(scaling, scaling);

	loot = createLootPool(shipwreckLootPoolSize, 0, shipwreckItemLimit);
}