#pragma once

/// <summary>
/// This class is used to represent the land masses in the game.
/// The types of land masses include: islands, rocks, and shipwrecks.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Textures_PG.h"
#include "ShopItem_PG.h"
#include "IslandMenu_PG.h"

namespace PirateGame {
	// This enum will be used to determine the land mass type
	enum class LandMassType { Island, Rock, Shipwreck };

	class LandMass {
	public:
		void createLandMass(LandMassType type, Textures& texture);
		void setRandomRotation() { sprite.setRotation(static_cast<float>(rand() % 360)); }

		// Setters
		void setPosition(sf::Vector2f pos) { sprite.setPosition(pos); }

		// Getters
		LandMassType getType() const { return type; }
		sf::Sprite& getSprite() { return sprite; }
		std::vector<ShopItem>& getMarket() { return market; }
		ShopItem& getMarketItem(std::string itemName);
		IslandMenu* getIslandMenu() { return islandMenu.get(); }

		// Draw the land mass
		void draw(sf::RenderWindow& window) { window.draw(sprite); }

	private:
		// Sprite to represent the land mass
		sf::Sprite sprite;
		sf::Texture texture;
		float scaling = 1;
		LandMassType type = LandMassType::Island;

		// Market
		std::vector<ShopItem> market;
		std::unique_ptr<IslandMenu> islandMenu;

		void createMarket();

		float gold = 0;
	};
}