#pragma once

/// Shipwreck class

#include <SFML/Graphics.hpp>

#include "LandMass_PG.h"

namespace PirateGame {
	class Shipwreck : public LandMass {
	public:
		Shipwreck() {}

		void createLandMass() override;

		std::vector<ShopItem>& getLoot() { return loot; }
		sf::Image getImage() override { return GlobalTextureHandler::getInstance().getLandMassTextures().getShipwreckTextures().getImage(type); }
		LandMassType getType() { return LandMassType::Shipwreck; }
	private:
		int shipwreckLootPoolSize = 3;
		int shipwreckItemLimit = 5;

		float scaling = 2.f;

		ShipwreckType type = ShipwreckType::Shipwreck1;

		std::vector<ShopItem> loot;
	};
}