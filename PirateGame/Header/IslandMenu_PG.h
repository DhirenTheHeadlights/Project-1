#pragma once

/// This is a child menu class for the island menu.

#include "Menu_PG.h"
#include "PlayerShip_PG.h"
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
		void setShip(PlayerShip& ship) { this->ship = &ship; };
		void setMarket(std::vector<ShopItem> market) { this->market = market; }
		void setEnteredIsland(bool enteredIsland) { this->enteredIsland = enteredIsland; }
		void setHasPlayerSaidNo(bool playerPromptedOnce) { this->hasPlayerSaidNo = playerPromptedOnce; }

		// Getters
		bool getEnteredIsland() { return enteredIsland; }
		bool getPlayerPromptedOnce() { return hasPlayerSaidNo; }
	private:
		// Menu items
		float padding = 10.f;
		float gold = 1000.f;

		bool enteredIsland = false;
		bool addedShipInventory = false;
		bool hasPlayerSaidNo = false;

		sf::Vector2f initalMenuSize = sf::Vector2f(600.0f, 110.0f);
		sf::Vector2f expandedMenuSize = sf::Vector2f(900.0f, 500.0f);

		sf::Vector2f merchandiseSize = sf::Vector2f(200.0f, 50.0f);
		sf::Vector2f buyButtonSize = sf::Vector2f(100.0f, 50.0f);
		sf::Vector2f sellButtonSize = sf::Vector2f(100.0f, 50.0f);
		sf::Vector2f uiButtonSize = sf::Vector2f(100.0f, 30.0f);
		sf::Vector2f goldDisplaySize = sf::Vector2f(200.f, 50.f);
		sf::Vector2f islandNameDisplaySize = sf::Vector2f(420.f, 50.f);

		PlayerShip* ship = nullptr;

		std::vector<std::unique_ptr<Button>> buyButtons;
		std::vector<std::unique_ptr<Button>> sellButtons;
		std::vector<std::unique_ptr<TextDisplayBox>> merchandise;
		std::vector<std::unique_ptr<TextDisplayBox>> shipInventoryInteractable;
		std::vector<std::unique_ptr<TextDisplayBox>> marketInventory;
		std::vector<std::unique_ptr<Button>> uiButtons;

		// Gold text display box
		std::unique_ptr<TextDisplayBox> shipGoldDisplay;
		std::unique_ptr<TextDisplayBox> islandGoldDisplay;

		// Island name display
		std::unique_ptr<TextDisplayBox> islandNameDisplay;

		std::vector<ShopItem>& market;
		std::vector<ShopItem> shipInventory;

		std::string islandName = "Island Name";
		sf::Text titleText;
		sf::Text islandDescription;

		sf::Clock buyClock;
	};
}