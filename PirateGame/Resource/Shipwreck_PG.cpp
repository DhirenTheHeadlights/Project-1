#include "Shipwreck_PG.h"

using namespace PirateGame;

void Shipwreck::createLandMass() {
	LandMassTypeGenerator<ShipwreckType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(context.GTH->getLandMassTextures().getShipwreckTextures().getTexture(type));
	sprite.setScale(context.JSL->getGameData().gameConfig.landmassData.shipwreckScaling, context.JSL->getGameData().gameConfig.landmassData.shipwreckScaling);

	loot = createLootPool(context.JSL->getGameData().gameConfig.landmassData.shipwreckLootPoolSize, 1, context.JSL->getGameData().gameConfig.landmassData.shipwreckLootPoolSize);
}