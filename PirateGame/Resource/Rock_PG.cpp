#include "Rock_PG.h"

using namespace PirateGame;

void Rock::createLandMass() {
	LandMassTypeGenerator<RockType> typeGen;
	type = typeGen.getType();
	sprite.setTexture(context.GTH->getLandMassTextures().getRockTextures().getTexture(type));
	sprite.setScale(scaling, scaling);
}