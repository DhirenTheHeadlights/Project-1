#pragma once

/// This class will handle the textures for the ships in the game.

#include "TextureHandler_PG.h"
#include "ShipEnums_PG.h"

namespace PirateGame {
	class ShipTextureHandler : public TextureHandler {
	public:
		ShipTextureHandler() {
			loadTextures();
		};
		~ShipTextureHandler() {};

		TextureResourceManager<ShipClass>& getShipTextureManager() { return shipTextureManager; }
		TextureResourceManager<ShipClass>& getCannonTextureManager() { return cannonTextureManager; }
		TextureResourceManager<ShipClass>& getSailTextureManager() { return sailTextureManager; }
	private:
		// Helper functions
		void  loadTextures() {
			shipTextureManager.loadTexture(ShipClass::Sloop, "PirateGameSprites/pg_ship_sloop.png");
			shipTextureManager.loadTexture(ShipClass::Brigantine, "PirateGameSprites/pg_ship_brigantine.png");
			shipTextureManager.loadTexture(ShipClass::Frigate, "PirateGameSprites/pg_ship_frigate.png");
			shipTextureManager.loadTexture(ShipClass::Galleon, "PirateGameSprites/pg_ship_galleon.png");
			shipTextureManager.loadTexture(ShipClass::ManOWar, "PirateGameSprites/pg_ship_manowar.png");

			cannonTextureManager.loadTexture(ShipClass::Sloop, "PirateGameSprites/pg_ship_sloop_cannon.png");
			cannonTextureManager.loadTexture(ShipClass::Brigantine, "PirateGameSprites/pg_ship_brigantine_cannon.png");
			cannonTextureManager.loadTexture(ShipClass::Frigate, "PirateGameSprites/pg_ship_frigate_cannon.png");
			cannonTextureManager.loadTexture(ShipClass::Galleon, "PirateGameSprites/pg_ship_galleon_cannon.png");
			cannonTextureManager.loadTexture(ShipClass::ManOWar, "PirateGameSprites/pg_ship_manowar_cannon.png");

			//// Sloop
			//sailTextureManager.loadTexture(ShipClass::Sloop, "PirateGameSprites/pg_ship_sloop_sail.png");

			//// Brigantine
			//sailTextureManager.loadTexture(ShipClass::Brigantine, "PirateGameSprites/pg_ship_brigantine_sail_front.png");
			//sailTextureManager.loadTexture(ShipClass::Brigantine, "PirateGameSprites/pg_ship_brigantine_sail_back.png");

			// Frigate
			std::vector<std::string> frigateSailTextures = {
				"PirateGameSprites/pg_ship_frigate_sail_back.png",
				"PirateGameSprites/pg_ship_frigate_sail_middle.png",
				"PirateGameSprites/pg_ship_frigate_sail_front.png"
			};
			sailTextureManager.loadTextures(ShipClass::Frigate, frigateSailTextures);

			//// Galleon
			//sailTextureManager.loadTexture(ShipClass::Galleon, "PirateGameSprites/pg_ship_galleon_sail_front.png");
			//sailTextureManager.loadTexture(ShipClass::Galleon, "PirateGameSprites/pg_ship_galleon_sail_middle.png");
			//sailTextureManager.loadTexture(ShipClass::Galleon, "PirateGameSprites/pg_ship_galleon_sail_back.png");

			//// ManOWar
			//sailTextureManager.loadTexture(ShipClass::ManOWar, "PirateGameSprites/pg_ship_manowar_sail_front.png");
			//sailTextureManager.loadTexture(ShipClass::ManOWar, "PirateGameSprites/pg_ship_manowar_sail_front_middle.png");
			//sailTextureManager.loadTexture(ShipClass::ManOWar, "PirateGameSprites/pg_ship_manowar_sail_back_middle.png");
			//sailTextureManager.loadTexture(ShipClass::ManOWar, "PirateGameSprites/pg_ship_manowar_sail_back.png");
		}

		// Texture manager
		TextureResourceManager<ShipClass> shipTextureManager;
		TextureResourceManager<ShipClass> cannonTextureManager;
		TextureResourceManager<ShipClass> sailTextureManager;
	};
}