#include "Island_PG.h"

using namespace PirateGame;

void Island::createLandMass() {
	LandMassTypeGenerator<IslandType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(context.GTH->getLandMassTextures().getIslandTextures().getTexture(type));
	sprite.setScale(scaling, scaling);

	market = createLootPool(marketSizeLimit, marketPriceLimit, marketItemLimit);
	islandMenu = std::make_unique<IslandMenu>(context, market);
	islandMenu->setUpMenu();
}