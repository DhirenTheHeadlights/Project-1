#pragma once
#pragma once

/// This class is used to represent the land masses in the game.
/// The types of land masses include: islands, rocks, and shipwrecks.

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
		virtual void createLandMass() = 0;
		void setRandomRotation() { sprite.setRotation(static_cast<float>(rand() % 360)); }

		// Getters
		sf::Sprite& getSprite() { return sprite; }
		virtual sf::Image getImage() = 0;
		virtual LandMassType getType() = 0;

		// Draw the land mass
		void draw(sf::RenderWindow& window) const {
			window.draw(sprite); 
		}

	protected:
		// Sprite to represent the land mass
		sf::Sprite sprite;

		// Type generator
		LandMassTypeGenerator<LandMassType> typeGenerator;

		std::vector<ShopItem> createLootPool(int marketSize, int marketPrice, int marketItems);
	};
}