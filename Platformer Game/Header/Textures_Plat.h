#pragma once

/// <summary>

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdlib.h>

namespace PlatformerGame {
	class Textures {
	public:
		Textures();
		~Textures();

		sf::Texture& grabPlayerTexture(int index);


	private:
		std::vector<sf::Texture> playerTextures{};
		std::vector<sf::Texture> enemyTextures;
		std::vector<sf::Texture> platformTextures;
		std::vector<sf::Texture> backgroundTextures;
		std::vector<sf::Texture> bulletTextures;
		std::vector<sf::Texture> powerupTextures;
		std::vector<sf::Texture> explosionTextures;
		std::vector<sf::Texture> particleTextures;
		std::vector<sf::Texture> healthTextures;
		std::vector<sf::Texture> coinTextures;
		std::vector<sf::Texture> doorTextures;
		std::vector<sf::Texture> keyTextures;
		std::vector<sf::Texture> spikeTextures;
		std::vector<sf::Texture> trapTextures;
		std::vector<sf::Texture> movingPlatformTextures;
		std::vector<sf::Texture> movingPlatformTextures2;
		std::vector<sf::Texture> movingPlatformTextures3;
		std::vector<sf::Texture> movingPlatformTextures4;
		std::vector<sf::Texture> movingPlatformTextures5;
		std::vector<sf::Texture> movingPlatformTextures6;
		std::vector<sf::Texture> movingPlatformTextures7;
		std::vector<sf::Texture> movingPlatformTextures8;
		std::vector<sf::Texture> movingPlatformTextures9;
		std::vector<sf::Texture> movingPlatformTextures10;
		std::vector<sf::Texture> movingPlatformTextures11;
		std::vector<sf::Texture> movingPlatformTextures12;
		std::vector<sf::Texture> movingPlatformTextures13;
		std::vector<sf::Texture> movingPlatformTextures14;
		std::vector<sf::Texture> movingPlatformTextures15;
		std::vector<sf::Texture> movingPlatformTextures16;
		std::vector<sf::Texture> movingPlatformTextures17;
		std::vector<sf::Texture> movingPlatformTextures18;
		std::vector<sf::Texture> movingPlatformTextures19;
		std::vector<sf::Texture> movingPlatformTextures20;
		std::vector<sf::Texture> movingPlatformTextures21;
		std::vector<sf::Texture> movingPlatformTextures22;
		std::vector<sf::Texture> movingPlatformTextures23;
		std::vector<sf::Texture> movingPlatformTextures24;


	};
}