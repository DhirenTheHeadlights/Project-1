#pragma once

/// This class will handle the textures for the ships in the game.

#include "TextureHandler_PG.h"

namespace PirateGame {
	class ShipTextureHandler : public TextureHandler {
	public:
		ShipTextureHandler() {
			// Load the textures
			loadTexture(shipTextures, "PirateGameSprites/pg_ship_sloop.png");
			loadTexture(shipTextures, "PirateGameSprites/pg_ship_brigantine.png");
			loadTexture(shipTextures, "PirateGameSprites/pg_ship_frigate.png");
			loadTexture(shipTextures, "PirateGameSprites/pg_ship_galleon.png");
			loadTexture(shipTextures, "PirateGameSprites/pg_ship_manowar.png");

			prepareImageMaps();
		};
		~ShipTextureHandler() {};

		// Getters
		sf::Texture& getSloop() { return shipTextures[0]; }
		sf::Texture& getBrig() { return shipTextures[1]; }
		sf::Texture& getFrigate() { return shipTextures[2]; }
		sf::Texture& getGalleon() { return shipTextures[3]; }
		sf::Texture& getManowar() { return shipTextures[4]; }
		sf::Image getShipImage(const sf::Texture* texture) {
			return shipImages[texture];
		}
		
	private:
		// Textures
		std::vector<sf::Texture> shipTextures;

		std::unordered_map<const sf::Texture*, sf::Image> shipImages;

		void loadTexture(std::vector<sf::Texture>& textures, const std::string& filename) {
			textures.emplace_back();
			textures.back().loadFromFile(filename);
		}

		void prepareImageMaps() {
			for (auto& texture : shipTextures) {
				shipImages[&texture] = texture.copyToImage();
			}
		}
	};
}