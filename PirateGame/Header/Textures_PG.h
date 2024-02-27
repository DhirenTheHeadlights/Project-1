#pragma once

/// <summary>
/// This class is going to handle all textures for the game.
/// It will load and create vectors of textures for all sprites in game.
/// </summary>

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdlib.h>

namespace PirateGame {
	class Textures {
	public:
		Textures();
		~Textures();

		// Takes the ship type as a string, returns the sf::Texture for that ship.
		sf::Texture& grabShipTexture(int index);
		// These two functions return a random island or rock texture unless a specific one is specified
		sf::Texture& grabIslandTexture(int index = -1);
		sf::Texture& grabRockTexture(int index = -1);
		sf::Texture& grabCannonballTexture() { return cannonball; }
	private:
		std::vector<sf::Texture> shipTextures{};
		std::vector<sf::Texture> islandTextures{};
		std::vector<sf::Texture> rockTextures{};
		sf::Texture cannonball;
	};
}