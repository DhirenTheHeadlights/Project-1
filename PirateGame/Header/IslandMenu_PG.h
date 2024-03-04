#pragma once

/// This is a child menu class for the island menu.

#include "Menu_PG.h"
#include "Ship_PG.h"
#include "ShopItem_PG.h"

namespace PirateGame {
	class IslandMenu : public Menu {
	public:
		IslandMenu(std::vector<ShopItem>& market) : market(market) {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void addMarketInteractables();
		void addShipInventoryInteractables();
		void updateMarket();
		void draw() override;

		// Setters
		void setShip(Ship& ship) { this->ship = &ship; };
		void setMarket(std::vector<ShopItem> market) { this->market = market; }
		void setEnteredIsland(bool enteredIsland) { this->enteredIsland = enteredIsland; }
	private:
		// Menu items
		float padding = 10.f;
		float gold = 1000.f;

		bool enteredIsland = false;
		bool addedShipInventory = false;

		sf::Vector2f initalMenuSize = sf::Vector2f(600.0f, 150.0f);
		sf::Vector2f expandedMenuSize = sf::Vector2f(900.0f, 500.0f);

		sf::Vector2f merchandiseSize = sf::Vector2f(200.0f, 50.0f);
		sf::Vector2f buyButtonSize = sf::Vector2f(100.0f, 50.0f);
		sf::Vector2f sellButtonSize = sf::Vector2f(100.0f, 50.0f);
		sf::Vector2f uiButtonSize = sf::Vector2f(100.0f, 30.0f);

		Ship* ship = nullptr;

		std::vector<std::unique_ptr<Button>> buyButtons;
		std::vector<std::unique_ptr<Button>> sellButtons;
		std::vector<std::unique_ptr<TextDisplayBox>> merchandise;
		std::vector<std::unique_ptr<TextDisplayBox>> shipInventoryInteractable;
		std::vector<std::unique_ptr<TextDisplayBox>> marketInventory;
		std::vector<std::unique_ptr<Button>> uiButtons;

		std::vector<ShopItem>& market;
		std::vector<ShopItem> shipInventory;

		sf::Text islandName;
		sf::Text islandDescription;
		sf::Text merchantGold;
		sf::Text shipGold;
	};
}