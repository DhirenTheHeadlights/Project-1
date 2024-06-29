#pragma once

/// This is a child menu class for the inventory menu.

#include "Menu_PG.h"
#include "PlayerShip_PG.h"
#include "ShopItem_PG.h"

namespace PirateGame {
	class InventoryMenu : public Menu {
	public:
		InventoryMenu() {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void update() override;
		void draw() override;
		
		// Setters
		void setPlayerShip(PlayerShip* ship) { this->ship = ship; };

		// Getters
		PlayerShip* getPlayerShip() { return this->ship; }
	private:
		// Menu items

		PlayerShip* ship = nullptr;
		sf::Sprite shipDisplaySprite;
		std::vector<ShopItem> inventory;
		std::vector<TextDisplayBox> inventoryBoxes;
		std::vector<TextDisplayBox> shipDisplayInfo;


		// Menu Size Constants
		float xProportion = 0.2f;
		float yProportion = 1.f;
	};
}