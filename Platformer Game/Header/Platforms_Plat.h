#pragma once
#include "SFML/Graphics.hpp"
#include "Textures_Plat.h"
#include <iostream>

namespace PlatformerGame {
	class Platforms_Plat {
	public:
		Platforms_Plat(sf::RenderWindow& window);

		//Generate Platformer
		void generatePlatforms(int numPlatforms);

	private:

	};

}
