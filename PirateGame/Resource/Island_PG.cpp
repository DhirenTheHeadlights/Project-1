#include "Island_PG.h"

using namespace PirateGame;

void Island::createLandMass() {
	LandMassTypeGenerator<IslandType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getIslandTextures().getTexture(type));
	sprite.setScale(scaling, scaling);

	market = createLootPool(marketSizeLimit, marketPriceLimit, marketItemLimit);
	islandMenu = std::make_unique<IslandMenu>(market);
	islandMenu->setUpMenu();
}