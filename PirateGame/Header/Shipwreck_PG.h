#pragma once

/// Shipwreck class

#include <SFML/Graphics.hpp>

#include "LandMass_PG.h"

namespace PirateGame {
	class Shipwreck : public LandMass {
	public:
		Shipwreck(GlobalContext& context) : LandMass(context) {};

		void createLandMass() override;

		std::vector<ShopItem>& getLoot() { return loot; }
		const sf::Image& getImage(GlobalContext& context) override { return Textures::landmassTextures.getShipwreckTextures().getImage(type); }
		LandMassType getType() override { return LandMassType::Shipwreck; }
	private:
		ShipwreckType type = ShipwreckType::Shipwreck1;

		std::vector<ShopItem> loot;
	};
}