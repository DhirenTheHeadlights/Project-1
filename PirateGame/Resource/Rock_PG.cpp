#include "Rock_PG.h"

using namespace PirateGame;

void Rock::createLandMass() {
	LandMassTypeGenerator<RockType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(Textures::landmassTextures.getRockTextures().getTexture(type));
	sprite.setScale(context.JSL->getGameData().gameConfig.landmassData.rockScaling, context.JSL->getGameData().gameConfig.landmassData.rockScaling);
}