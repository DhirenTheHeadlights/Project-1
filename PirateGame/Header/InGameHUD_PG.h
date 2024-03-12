#pragma once

/// <summary>
/// This class is used to represent the in-game HUD.
/// </summary>

#include "Menu_PG.h"
#include "PlayerShip_PG.h"
#include "Minimap_PG.h"
#include "GlobalTextureHandler_PG.h"

namespace PirateGame {
	class InGameHUD : public Menu {
	public:
		InGameHUD() {
			healthBarGreenSprite.setTexture(GlobalTextureHandler::getInstance().getHUDTextures().getHealthBarGreen());
			healthBarGreenSprite.setScale(healthBarScale);

			healthBarRedSprite.setTexture(GlobalTextureHandler::getInstance().getHUDTextures().getHealthBarRed());
			healthBarRedSprite.setScale(healthBarScale);
		};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void addInfoBox(std::string text, std::vector<std::unique_ptr<TextDisplayBox>>& destination);
		void updateShipPropertiesString();
		void draw() override;

		// Setters
		void setShip(PlayerShip& ship) { this->ship = &ship; minimap.setShip(&ship); };

		// Getters
		Minimap& getMinimap() { return minimap; }
	private:
		// Menu items
		float healthFraction = 100;
		float padding = 10.f;

		PlayerShip* ship = nullptr;

		sf::Sprite healthBarGreenSprite;
		sf::Sprite healthBarRedSprite;

		sf::Vector2f healthBarScale = { 11, 4 };
		sf::Vector2f settingsScale = { 0.8f, 0.8f };

		sf::VertexArray windVector;
		sf::CircleShape windCircle;
		sf::Text windText;

		std::vector<std::unique_ptr<TextDisplayBox>> shipPropertiesLeftSide;
		std::vector<std::unique_ptr<TextDisplayBox>> shipPropertiesRightSide;

		Minimap minimap;

		float minimapSize = 100.f;
		sf::Text healthText;
	};
}