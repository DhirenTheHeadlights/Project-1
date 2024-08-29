#include "Island_PG.h"

using namespace PirateGame;

void Island::createLandMass() {
	LandMassTypeGenerator<IslandType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(context.GTH->getLandMassTextures().getIslandTextures().getTexture(type));
	sprite.setScale(context.JSL->getGameData().gameConfig.landmassData.islandScaling, context.JSL->getGameData().gameConfig.landmassData.islandScaling);

	market = createLootPool(context.JSL->getGameData().gameConfig.landmassData.islandMarketSizeLimit, context.JSL->getGameData().gameConfig.landmassData.islandMarketPriceLimit, context.JSL->getGameData().gameConfig.landmassData.islandMarketSizeLimit);
	islandMenu = std::make_unique<IslandMenu>(context, market);
	islandMenu->setUpMenu();
}