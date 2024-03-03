#pragma once

/// <summary>
/// This class is used to represent the in-game HUD.
/// </summary>

#include "Menu_PG.h"
#include "Ship_PG.h"

namespace PirateGame {
	class InGameHUD : public Menu {
	public:
		InGameHUD(GameStateManager& GSM) : GSM(GSM) {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void draw() override;

		// Setters
		void setShip(Ship& ship) { this->ship = &ship; };
	private:
		// GameStateManager
		GameStateManager& GSM;

		// Menu items
		float healthFraction = 100;
		float padding = 10.f;

		Ship* ship = nullptr;

		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;

		// Temp rectangle for minimap
		sf::RectangleShape minimap;

		sf::Vector2f healthBarSize;
		sf::Text healthText;
	};
}