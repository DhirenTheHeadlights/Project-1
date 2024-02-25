#include "Cannonball_PG.h"

using namespace PirateGame;

Cannonball::Cannonball() {
	texture = textures.grabCannonballTexture();
	sprite.setTexture(texture);
}

