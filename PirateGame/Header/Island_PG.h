#pragma once

/// Island class

#include <SFML/Graphics.hpp>

#include "LandMass_PG.h"

namespace PirateGame {
	class Island : public LandMass {
	public:
		Island(GlobalContext& context) : LandMass(context) {};

		void createLandMass() override;

		IslandMenu* getIslandMenu() const { return islandMenu.get(); }
		std::vector<ShopItem>& getMarket() { return market; }

		const sf::Image& getImage(GlobalContext& context) override { return Textures::landmassTextures.getIslandTextures().getImage(type); }
		LandMassType getType() override { return LandMassType::Island; }
	private:
		std::unique_ptr<IslandMenu> islandMenu;

		IslandType type = IslandType::Island1;

		std::vector<ShopItem> market;
	};
}