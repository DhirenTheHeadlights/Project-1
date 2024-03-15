#pragma once

/// This is a child menu class for the island menu.

#include "Menu_PG.h"
#include "PlayerShip_PG.h"
#include "ShopItem_PG.h"

namespace PirateGame {
	enum class page {
		marketPage1,
		marketPage2,
		marketPage3,
		shipUpgrades,
	};

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
		bool getEnteredIsland() const { return enteredIsland; }
		bool getPlayerPromptedOnce() const { return hasPlayerSaidNo; }
	private:
		// Menu items
		float padding = 10.f;
		float gold = 1000.f;
		int textSize = 40;
		int interactableTextSizeSmall = 30;
		int interactableTextSizeBig = 60;

		bool enteredIsland = false;
		bool addedInteractables = false;
		bool hasPlayerSaidNo = false;

		PlayerShip* ship = nullptr;

		sf::Sprite banner;
		sf::Sprite initialMenu;

		std::vector<std::unique_ptr<Button>> buyButtons;
		std::vector<std::unique_ptr<Button>> sellButtons;
		std::vector<std::unique_ptr<TextDisplayBox>> merchandise;
		std::vector<std::unique_ptr<TextDisplayBox>> shipInventoryInteractable;
		std::vector<std::unique_ptr<TextDisplayBox>> marketInventory;
		std::vector<std::unique_ptr<Button>> uiButtons;

		sf::Vector2f bannerScale = sf::Vector2f(1.5f, 1.5f);

		// Gold text display box
		std::unique_ptr<TextDisplayBox> shipGoldDisplay;
		std::unique_ptr<TextDisplayBox> islandGoldDisplay;

		// Names
		std::unique_ptr<TextDisplayBox> islandNameDisplay;
		std::unique_ptr<TextDisplayBox> shipNameDisplay;
		std::unique_ptr<TextDisplayBox> marketNameDisplay;

		std::vector<ShopItem>& market;
		std::vector<ShopItem> shipInventory;

		std::string islandName = "Island Name";

		sf::Clock buyClock;

		// Specific values for menu item placement

		const sf::Vector2f initialButtonPositionLeft = sf::Vector2f(12.f, 12.f);
		const sf::Vector2f initialButtonPositionRight = sf::Vector2f(251.f, 12.f);

		const sf::Vector2f marketTopMiddle = sf::Vector2f(400.f, 22.f);
		const sf::Vector2f marketTopLeft = sf::Vector2f(20.f, 22.f);
		const sf::Vector2f marketTopRight = sf::Vector2f(1204.f, 22.f);
		const sf::Vector2f marketMiddle = sf::Vector2f(400.f, 162.f);
		const sf::Vector2f marketLeftMiddle = sf::Vector2f(20.f, 162.f);
		const sf::Vector2f marketSell = sf::Vector2f(210.f, 162.f);
		const sf::Vector2f marketBuy = sf::Vector2f(1204.f, 162.f);
		const sf::Vector2f marketRightMiddle = sf::Vector2f(1394.f, 162.f);
		const sf::Vector2f marketBottomLeft = sf::Vector2f(20.f, 883.f);
		const sf::Vector2f marketNavigationLeft = sf::Vector2f(304.f, 883.f);
		const sf::Vector2f marketNavigationRight = sf::Vector2f(1204.f, 883.f);
		const sf::Vector2f marketBottomRight = sf::Vector2f(1300.f, 883.f);
		const sf::Vector2f marketBottomMiddle = sf::Vector2f(400.f, 883.f);
	};
}