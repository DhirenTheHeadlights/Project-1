#pragma once

/// This class is a singleton class that contains all of the texture managers for the game.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "InteractableTextures_PG.h"
#include "LandMassTextures_PG.h"
#include "HUDTextures_PG.h"
#include "OptionsMenuTextureHandler_PG.h"
#include "MarketTextureHandler_PG.h"

namespace PirateGame {
	class GlobalTextureHandler {
	public:
		static GlobalTextureHandler& getInstance() {
			static GlobalTextureHandler instance;
			return instance;
		}

		// Getters for texture managers
		InteractableTextureHandler& getInteractableTextures() { return *interactableTextures; }
		LandMassTextureHandler& getLandMassTextures() { return *landMassTextures; }
		HUDTextureHandler& getHUDTextures() { return *HUDTextures; }
		OptionsMenuTextureHandler& getOptionsMenuTextures() { return *optionsMenuTextures; }
		MarketTextureHandler& getMarketTextures() { return *marketTextures; }

	private:
		GlobalTextureHandler() {
			// Nothing to do here
		}

		// Delete copy constructor and assignment operator
		GlobalTextureHandler(GlobalTextureHandler const&) = delete;
		GlobalTextureHandler& operator=(GlobalTextureHandler const&) = delete;

		// Managers
		std::unique_ptr<InteractableTextureHandler> interactableTextures = std::make_unique<InteractableTextureHandler>();
		std::unique_ptr<LandMassTextureHandler> landMassTextures = std::make_unique<LandMassTextureHandler>();
		std::unique_ptr<HUDTextureHandler> HUDTextures = std::make_unique<HUDTextureHandler>();
		std::unique_ptr<OptionsMenuTextureHandler> optionsMenuTextures = std::make_unique<OptionsMenuTextureHandler>();
		std::unique_ptr<MarketTextureHandler> marketTextures = std::make_unique<MarketTextureHandler>();
	};
}