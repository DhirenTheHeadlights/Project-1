#pragma once

/// Rock class

#include <SFML/Graphics.hpp>

#include "LandMass_PG.h"

namespace PirateGame {
	class Rock : public LandMass {
	public:
		Rock() {} 

		void createLandMass() override;

		sf::Image getImage() override { return GlobalTextureHandler::getInstance().getLandMassTextures().getRockTextures().getImage(type); }
		LandMassType getType() { return LandMassType::Rock; }
	private:
		float scaling = 0.125f;

		RockType type = RockType::Rock1;
	};
}