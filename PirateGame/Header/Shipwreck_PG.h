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
		const sf::Image& getImage(GlobalContext& context) override { return context.GTH->getLandMassTextures().getShipwreckTextures().getImage(type); }
		LandMassType getType() { return LandMassType::Shipwreck; }
	private:
		const int shipwreckLootPoolSize = 3;
		const int shipwreckItemLimit = 5;

		const float scaling = 5.f;

		ShipwreckType type = ShipwreckType::Shipwreck1;

		std::vector<ShopItem> loot;
	};
}