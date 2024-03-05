#pragma once

/// <summary>
/// This class is used to represent the in-game HUD.
/// </summary>

#include "Menu_PG.h"
#include "Ship_PG.h"
#include "Minimap_PG.h"

namespace PirateGame {
	class InGameHUD : public Menu {
	public:
		InGameHUD() {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void updateShipPropertiesString();
		void draw() override;

		// Setters
		void setShip(Ship& ship) { this->ship = &ship; minimap.setShip(&ship); };

		// Getters
		Minimap& getMinimap() { return minimap; }
	private:
		// Menu items
		float healthFraction = 100;
		float padding = 10.f;

		Ship* ship = nullptr;

		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;

		std::vector<std::unique_ptr<TextDisplayBox>> shipProperties;

		std::unique_ptr<TextDisplayBox> goldDisplay;
		std::unique_ptr<TextDisplayBox> shipCoords;
		std::unique_ptr<TextDisplayBox> shipVeclocity;

		Minimap minimap;

		sf::Vector2f healthBarSize;
		float minimapSize = 100.f;
		sf::Text healthText;
	};
}