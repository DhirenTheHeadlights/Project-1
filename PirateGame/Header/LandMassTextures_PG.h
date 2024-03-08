#pragma once

/// Inherited class for land mass textures.

#include "TextureHandler_PG.h"

namespace PirateGame {
	class LandMassTextureHandler : public TextureHandler {
	public:
		LandMassTextureHandler() {
			// Load all island textures
			sf::Texture island1, island2;
			island1.loadFromFile("PirateGameSprites/pg_island_1.png");
			island2.loadFromFile("PirateGameSprites/pg_island_2.png");
			for (auto& i : { island1, island2 }) {
				islandTextures.push_back(i);
			}

			// Load all rock textures
			sf::Texture rock1, rock2, rock3, rock4, rock5, rock6, rock7, rock8, rock9;
			rock1.loadFromFile("PirateGameSprites/pg_rock_1.png");
			rock2.loadFromFile("PirateGameSprites/pg_rock_2.png");
			rock3.loadFromFile("PirateGameSprites/pg_rock_3.png");
			rock4.loadFromFile("PirateGameSprites/pg_rock_4.png");
			rock5.loadFromFile("PirateGameSprites/pg_rock_5.png");
			rock6.loadFromFile("PirateGameSprites/pg_rock_6.png");
			rock7.loadFromFile("PirateGameSprites/pg_rock_7.png");
			rock8.loadFromFile("PirateGameSprites/pg_rock_8.png");
			rock9.loadFromFile("PirateGameSprites/pg_rock_9.png");
			for (auto& i : { rock1, rock2, rock3, rock4, rock5, rock6, rock7, rock8, rock9 }) {
				rockTextures.push_back(i);
			}

			// Load cannonball texture
			cannonballTexture.loadFromFile("PirateGameSprites/pg_misc_cannonball.png");
		}
		~LandMassTextureHandler() {};

		// These two functions return a random island or rock texture unless a specific one is specified
		sf::Texture& grabIslandTexture(int index = -1) {
			if (index == -1) {
				return islandTextures[rand() % islandTextures.size()];
			}
			else {
				return islandTextures[index];
			}
		}
		sf::Texture& grabRockTexture(int index = -1) {
			if (index == -1) {
				return rockTextures[rand() % rockTextures.size()];
			}
			else {
				return rockTextures[index];
			}
		}

		// Getters
		sf::Texture& getCannonballTexture() { return cannonballTexture; }

	private:
		std::vector<sf::Texture> islandTextures{};
		std::vector<sf::Texture> rockTextures{};

		sf::Texture cannonballTexture;
	};
}