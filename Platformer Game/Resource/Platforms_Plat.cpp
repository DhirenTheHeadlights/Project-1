#include "Platforms_Plat.h"

using namespace PlatformerGame;

Platforms_Plat::Platforms_Plat(){

}

void Platforms_Plat::generatePlatforms(int numPlatforms) {

	int platformsGenerated = 0;

	while (platformsGenerated < numPlatforms) {
		int yRandom = std::rand() % 1000;
		int xRandom = std::rand() % 100000;

		sf::RectangleShape rectangle(sf::Vector2(120.f, 50.f));

	}

}