#pragma once

/// HUD textures for the game.

#include "TextureHandler_PG.h"

namespace PirateGame {
	class HUDTextureHandler : public TextureHandler {
	public:
		HUDTextureHandler() {
			// Load health bar textures
			HealthBarGreen.loadFromFile("PirateGameSprites/health_bar_green.png");
			HealthBarRed.loadFromFile("PirateGameSprites/health_bar_red.png");
			miniMap.loadFromFile("PirateGameSprites/minimap_sprite.png");
			miniMapShipIcon.loadFromFile("PirateGameSprites/minimap_ship_icon.png");
		}
		~HUDTextureHandler() {};

		// Getters
		sf::Texture& getHealthBarGreen() { return HealthBarGreen; }
		sf::Texture& getHealthBarRed() { return HealthBarRed; }
		sf::Texture& getMiniMap() { return miniMap; }
		sf::Texture& getMiniMapShipIcon() { return miniMapShipIcon; }
	private:
		sf::Texture HealthBarGreen;
		sf::Texture HealthBarRed;
		sf::Texture miniMap;
		sf::Texture miniMapShipIcon;
	};
}