#pragma once
#include "SFML/Graphics.hpp"
#include "Textures_Plat.h"
#include <iostream>

namespace PlatformerGame {
	class Platforms_Plat {
	public:
		Platforms_Plat();

		//Generate Platformer
		void generatePlatforms(int numPlatforms);
		void draw(sf::RenderWindow& window);
		const std::vector<sf::RectangleShape>& getPlatforms() const {
			return platforms;
		}

	private:
		std::vector<sf::RectangleShape> platforms;
		sf::Texture platformTexture;

	};

}
