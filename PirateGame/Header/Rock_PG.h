#pragma once

/// Rock class

#include <SFML/Graphics.hpp>

#include "LandMass_PG.h"

namespace PirateGame {
	class Rock : public LandMass {
	public:
		Rock(GlobalContext& context) : LandMass(context) {};

		void createLandMass() override;

		const sf::Image& getImage(GlobalContext& context) override { return context.GTH->getLandMassTextures().getRockTextures().getImage(type); }
		LandMassType getType() { return LandMassType::Rock; }
	private:
		const float scaling = 0.125f;

		RockType type = RockType::Rock1;
	};
}