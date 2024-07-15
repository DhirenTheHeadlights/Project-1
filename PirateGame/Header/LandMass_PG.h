#pragma once

/// <summary>
/// This class is used to represent the land masses in the game.
/// The types of land masses include: islands, rocks, and shipwrecks.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <variant>

#include "GlobalTextureHandler_PG.h"
#include "ShopItem_PG.h"
#include "IslandMenu_PG.h"
#include "LandMassTypeGenerator_PG.h"

namespace PirateGame {
	class LandMass {
	public:
		void createLandMass(LandMassType type);
		void setRandomRotation() { sprite.setRotation(static_cast<float>(rand() % 360)); }

		// Setters
		void setPosition(sf::Vector2f pos) { sprite.setPosition(pos); }

		// Getters
		LandMassType getType() const { return type; }
		sf::Sprite& getSprite() { return sprite; }
		std::vector<ShopItem>& getMarket() { return market; }
		std::vector<ShopItem>& getLoot() { return loot; }
		ShopItem& getMarketItem(std::string itemName);
		IslandMenu* getIslandMenu() { return islandMenu.get(); }
		std::variant<IslandType, RockType, ShipwreckType> getSpecificType() const { return specificType; }

		// Draw the land mass
		void draw(sf::RenderWindow& window) const { 
			window.draw(sprite); 
		}

	private:
		// Sprite to represent the land mass
		sf::Sprite sprite;
		float scaling = 1;

		// Types
		LandMassType type = LandMassType::Island;
		std::variant<IslandType, RockType, ShipwreckType> specificType;

		// Market
		std::vector<ShopItem> market;
		std::vector<ShopItem> loot;
		std::unique_ptr<IslandMenu> islandMenu;
		int marketSizeLimit = 7;
		int marketItemLimit = 100;
		int marketPriceLimit = 100;
		int shipwreckLootPoolSize = 3;
		int shipwreckItemLimit = 5;

		// Type generator
		LandMassTypeGenerator<LandMassType> typeGenerator;

		std::vector<ShopItem> createLootPool(int marketSize, int marketPrice, int marketItems);

		float gold = 0;
	};
}