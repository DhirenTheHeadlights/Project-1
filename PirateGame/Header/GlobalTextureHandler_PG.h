#pragma once

/// This class is a singleton class that contains all of the texture managers for the game.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "InteractableTextures_PG.h"
#include "LandMassTextures_PG.h"
#include "HUDTextures_PG.h"
#include "OptionsMenuTextureHandler_PG.h"
#include "MarketTextureHandler_PG.h"
#include "ShipTextureHandler_PG.h"
#include "InventoryTextures_PG.h"

namespace PirateGame {
	class GlobalTextureHandler {
	public:
		static GlobalTextureHandler& getInstance() {
			static GlobalTextureHandler instance;
			return instance;
		}

		// Getters for texture managers
		InteractableTextureHandler& getInteractableTextures() { return interactableTextures; }
		LandmassTextureHandler& getLandMassTextures() { return landMassTextures; }
		HUDTextureHandler& getHUDTextures() { return HUDTextures; }
		OptionsMenuTextureHandler& getOptionsMenuTextures() { return optionsMenuTextures; }
		MarketTextureHandler& getMarketTextures() { return marketTextures; }
		ShipTextureHandler& getShipTextures() { return shipTextures; }
		InventoryTextureHandler& getInventoryTextures() { return inventoryTextures; }

	private:
		GlobalTextureHandler() {}

		// Delete copy constructor and assignment operator
		GlobalTextureHandler(GlobalTextureHandler const&) = delete;
		GlobalTextureHandler& operator=(GlobalTextureHandler const&) = delete;

		// Managers
		InteractableTextureHandler interactableTextures;
		LandmassTextureHandler landMassTextures;
		HUDTextureHandler HUDTextures;
		OptionsMenuTextureHandler optionsMenuTextures;
		MarketTextureHandler marketTextures;
		ShipTextureHandler shipTextures;
		InventoryTextureHandler inventoryTextures;
	};

}