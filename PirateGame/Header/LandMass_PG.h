#pragma once
#pragma once

/// This class is used to represent the land masses in the game.
/// The types of land masses include: islands, rocks, and shipwrecks.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <variant>

#include "ShopItem_PG.h"
#include "IslandMenu_PG.h"
#include "LandMassTypeGenerator_PG.h"

namespace PirateGame {
	class LandMass {
	public:
		LandMass(GlobalContext& context) : context(context) {}

		virtual void createLandMass() = 0;
		void setRandomRotation() { sprite.setRotation(static_cast<float>(rand() % 360)); }

		// Getters
		sf::Sprite& getSprite() { return sprite; }
		virtual const sf::Image& getImage(GlobalContext& context) = 0;
		virtual LandMassType getType() = 0;
		bool isActive() const { return active; }

		// Setters
		void deactivate() { active = false; }

		// Draw the land mass
		void draw(sf::RenderWindow& window) const {
			window.draw(sprite); 
		}

	protected:
		// Global context
		GlobalContext& context;

		// Sprite to represent the land mass
		sf::Sprite sprite;

		// Active flag
		bool active = true;

		// Type generator
		LandMassTypeGenerator<LandMassType> typeGenerator;

		std::vector<ShopItem> createLootPool(int marketSize, int marketPrice, int marketItems);
	};
}