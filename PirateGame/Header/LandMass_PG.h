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
		ShopItem& getMarketItem(std::string itemName);
		IslandMenu* getIslandMenu() { return islandMenu.get(); }
		std::variant<IslandType, RockType> getSpecificType() const { return specificType; }

		// Draw the land mass
		void draw(sf::RenderWindow& window) const { 
			window.draw(sprite); 

			// Debug draw the bounding box
			sf::FloatRect bounds = sprite.getGlobalBounds();
			sf::RectangleShape rect(sf::Vector2f(bounds.width, bounds.height));
			rect.setPosition(bounds.left, bounds.top);
			rect.setFillColor(sf::Color::Transparent);
			rect.setOutlineColor(sf::Color::Red);
			rect.setOutlineThickness(1);
			window.draw(rect);
		}

	private:
		// Sprite to represent the land mass
		sf::Sprite sprite;
		float scaling = 1;

		// Types
		LandMassType type = LandMassType::Island;
		std::variant<IslandType, RockType> specificType;

		// Market
		std::vector<ShopItem> market;
		std::unique_ptr<IslandMenu> islandMenu;

		// Type generator
		LandMassTypeGenerator<LandMassType> typeGenerator;

		void createMarket();

		float gold = 0;
	};
}