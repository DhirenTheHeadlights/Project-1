#include "Shipwreck_PG.h"

using namespace PirateGame;

void Shipwreck::createLandMass() {
	LandMassTypeGenerator<ShipwreckType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(context.GTH->getLandMassTextures().getShipwreckTextures().getTexture(type));
	sprite.setScale(scaling, scaling);

	loot = createLootPool(shipwreckLootPoolSize, 1, shipwreckItemLimit);
}