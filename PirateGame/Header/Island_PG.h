#pragma once

/// Island class

#include <SFML/Graphics.hpp>

#include "LandMass_PG.h"

namespace PirateGame {
	class Island : public LandMass {
	public:
		Island(GlobalContext& context) : LandMass(context) {};

		void createLandMass() override;

		IslandMenu* getIslandMenu() { return islandMenu.get(); }
		std::vector<ShopItem>& getMarket() { return market; }

		const sf::Image& getImage(GlobalContext& context) override { return context.GTH->getLandMassTextures().getIslandTextures().getImage(type); }
		LandMassType getType() { return LandMassType::Island; }
	private:
		std::unique_ptr<IslandMenu> islandMenu;

		int marketSizeLimit = 7;
		int marketItemLimit = 100;
		int marketPriceLimit = 100;

		float scaling = 1.0f;

		IslandType type = IslandType::Island1;

		std::vector<ShopItem> market;
	};
}