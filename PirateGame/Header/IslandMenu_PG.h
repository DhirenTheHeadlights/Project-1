#pragma once

/// This is a child menu class for the island menu.

#include "Menu_PG.h"
#include "Ship_PG.h"
#include "ShopItem_PG.h"

namespace PirateGame {
	class IslandMenu : public Menu {
	public:
		IslandMenu() {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void addMarketInteractables();
		void draw() override;

		// Setters
		void setShip(Ship& ship) { this->ship = &ship; };
		void setMarket(std::vector<ShopItem> market) { this->market = market; }
	private:
		// Menu items
		float padding = 10.f;
		bool enteredIsland = false;

		sf::Vector2f initalMenuSize = sf::Vector2f(600.0f, 150.0f);

		Ship* ship = nullptr;

		std::vector<std::unique_ptr<Button>> buyButtons;
		std::vector<std::unique_ptr<Button>> sellButtons;
		std::vector<std::unique_ptr<TextDisplayBox>> merchandise;

		std::vector<ShopItem> market;

		sf::Text islandName;
	};
}