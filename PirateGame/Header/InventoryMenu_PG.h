#pragma once

/// This is a child menu class for the inventory menu.

#include "Menu_PG.h"
#include "PlayerShip_PG.h"
#include "ShopItem_PG.h"
#include "ScrollBar_PG.h"

namespace PirateGame {
	class InventoryMenu : public Menu {
	public:
		InventoryMenu(GlobalContext& context) : Menu(context) {};

		void setUpMenu() override;
		void setInteractablePositions() override;
		void addInteractablesToMenu() override;
		void interactWithMenuItems() override;
		void update() override;
		void draw() override;
		
		// Setters
		void setPlayerShip(PlayerShip* ship) { this->ship = ship; };

		// Getters
		PlayerShip* getPlayerShip() const { return this->ship; }
	private:
		// Menu items
		PlayerShip* ship = nullptr;

		sf::Sprite shipDisplaySprite;
		sf::Sprite shipDisplayBackground;
		sf::Sprite inventoryDisplayBackground;

		std::vector<ShopItem> inventory;
		std::vector<std::shared_ptr<Interactable>> inventoryBoxes;
		std::vector<TextDisplayBox> shipDisplayInfo;

		ScrollBar scrollBar = ScrollBar(context);

		// Menu Size Constants
		float xProportion = 0.2f;
		float yProportion = 1.f;
		sf::Vector2f scale = sf::Vector2f(1.f, 1.f);

		// Constant positions
		sf::Vector2f shipDisplayPosition = sf::Vector2f(34.f, 35.f);
		sf::Vector2f shipInfoPosition = sf::Vector2f(34.f, 322.f);
		sf::Vector2f inventoryPosition = sf::Vector2f(34.f, 514.f);
		sf::Vector2f scrollBarPosition = sf::Vector2f(277.f, 514.f);
	};
}