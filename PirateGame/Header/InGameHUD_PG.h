#pragma once

/// This class is used to represent the in-game HUD.

#include "Menu_PG.h"
#include "PlayerShip_PG.h"
#include "Minimap_PG.h"

namespace PirateGame {
	class InGameHUD : public Menu {
	public:
		InGameHUD(GlobalContext& context, WindHandler& WH) : Menu(context), minimap(Textures::HUDTextures.getMiniMap(), Textures::HUDTextures.getMiniMapShipIcon()), WH(WH) {
			healthBarGreenSprite.setTexture(Textures::HUDTextures.getHealthBarGreen());
			healthBarGreenSprite.setScale(healthBarScale);

			healthBarRedSprite.setTexture(Textures::HUDTextures.getHealthBarRed());
			healthBarRedSprite.setScale(healthBarScale);
		};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void updateShipPropertiesStrings();
		void draw() override;
		void update() override;
		void interactWithMenuItems() override;

		// Setters
		void setPlayerShip(PlayerShip& ship) { this->ship = &ship; minimap.setShip(&ship); };

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
		sf::Vector2f infoBoxScale = { 0.5f, 0.5f };

		WindHandler& WH;
		sf::VertexArray windVector;
		sf::CircleShape windCircle;
		sf::Text windText;

		std::vector<TextDisplayBox> shipPropertiesLeftSide;
		std::vector<TextDisplayBox> shipPropertiesRightSide;

		Button settingsButton;

		Minimap minimap;

		float minimapSize = 100.f;
		sf::Text healthText;
	};
}