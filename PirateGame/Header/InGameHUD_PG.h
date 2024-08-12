#pragma once

/// This class is used to represent the in-game HUD.

#include "Menu_PG.h"
#include "PlayerShip_PG.h"
#include "Minimap_PG.h"

namespace PirateGame {
	class InGameHUD : public Menu {
	public:
		InGameHUD(GlobalContext& context) : Menu(context), minimap(context.GTH->getHUDTextures().getMiniMap(), context.GTH->getHUDTextures().getMiniMapShipIcon()) {
			healthBarGreenSprite.setTexture(context.GTH->getHUDTextures().getHealthBarGreen());
			healthBarGreenSprite.setScale(healthBarScale);

			healthBarRedSprite.setTexture(context.GTH->getHUDTextures().getHealthBarRed());
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