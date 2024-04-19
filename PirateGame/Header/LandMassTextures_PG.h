#pragma once

#include "TextureHandler_PG.h"
#include <vector>
#include <unordered_map>

namespace PirateGame {
	class LandmassTextureHandler : public TextureHandler {
	public:
		LandmassTextureHandler() {
			// Load island textures
			loadTexture(islandTextures, "PirateGameSprites/pg_island_1.png");
			loadTexture(islandTextures, "PirateGameSprites/pg_island_2.png");

			// Load rock textures
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_1.png");
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_2.png");
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_3.png");
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_4.png");
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_5.png");
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_6.png");
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_7.png");
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_8.png");
			loadTexture(rockTextures, "PirateGameSprites/pg_rock_9.png");

			// Load cannonball texture
			cannonballTexture.loadFromFile("PirateGameSprites/pg_misc_cannonball.png");

			// Prepare image maps from textures
			prepareImageMaps();
		}
		~LandmassTextureHandler() {}

		sf::Texture& grabIslandTexture(int index = -1) {
			return genericGrabTexture(islandTextures, index);
		}

		sf::Texture& grabRockTexture(int index = -1) {
			return genericGrabTexture(rockTextures, index);
		}

		sf::Image getLandMassImage(const sf::Texture* texture) {
			return landmassImages[texture];
		}

		sf::Texture& getCannonballTexture() {
			return cannonballTexture;
		}

	private:
		std::vector<sf::Texture> islandTextures;
		std::vector<sf::Texture> rockTextures;
		sf::Texture cannonballTexture;

		std::unordered_map<const sf::Texture*, sf::Image> landmassImages;

	    void loadTexture(std::vector<sf::Texture>& textures, const std::string& filename) {
			textures.emplace_back();
			textures.back().loadFromFile(filename);
		}

		void prepareImageMaps() {
			for (auto& texture : islandTextures) {
				landmassImages[&texture] = texture.copyToImage();
			}
			for (auto& texture : rockTextures) {
				landmassImages[&texture] = texture.copyToImage();
			}
		}

		sf::Texture& genericGrabTexture(std::vector<sf::Texture>& textures, int index) {
			if (index == -1) {
				return textures[rand() % textures.size()];
			}
			else {
				return textures[index];
			}
		}
	};
}
