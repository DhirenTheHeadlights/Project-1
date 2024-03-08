#pragma once

/// Interactable textures for the game.

#include "TextureHandler_PG.h"

namespace PirateGame {
	class InteractableTextureHandler : public TextureHandler {
	public:
		InteractableTextureHandler() {
			// Load all interactable textures
			sf::Texture healthBarGreen, healthBarRed;
			healthBarGreen.loadFromFile("PirateGameSprites/health_bar_green.png");
			healthBarRed.loadFromFile("PirateGameSprites/health_bar_red.png");
			for (auto& i : { healthBarGreen, healthBarRed }) {
				interactableTextures.push_back(i);
			}
		}
		~InteractableTextureHandler() {};

		// These two functions return a specific interactable texture
		sf::Texture& grabHealthBarGreenTexture() { return interactableTextures[0]; }
		sf::Texture& grabHealthBarRedTexture() { return interactableTextures[1]; }

	private:
		std::vector<sf::Texture> interactableTextures{};
	};
}