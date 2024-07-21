#include "Rock_PG.h"

using namespace PirateGame;

void Rock::createLandMass() {
	LandMassTypeGenerator<RockType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getRockTextures().getTexture(type));
	sprite.setScale(scaling, scaling);
}