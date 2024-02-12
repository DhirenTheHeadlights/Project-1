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
		void addInteractables() override;
		void draw() override;

		// Setters
		void setShip(Ship& ship) { this->health = ship.getHealth(); }
	private:
		// GameStateManager
		GameStateManager& GSM;

		// Menu variables
		sf::View HUDView;

		// Menu items
		float health = 100;
		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;
		sf::Text healthText;
	};
}