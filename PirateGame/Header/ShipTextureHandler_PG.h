#pragma once

/// This class will handle the textures for the ships in the game.

#include "TextureHandler_PG.h"
#include "ShipClass_PG.h"

namespace PirateGame {
	class ShipTextureHandler : public TextureHandler {
	public:
		ShipTextureHandler() {
			loadTextures();
		};
		~ShipTextureHandler() {};

		TextureResourceManager<ShipClass>& getTextureManager() { return textureManager; }
	private:
		// Helper functions
		void loadTextures() {
			textureManager.loadTexture(ShipClass::Sloop, "PirateGameSprites/pg_ship_sloop.png");
			textureManager.loadTexture(ShipClass::Brigantine, "PirateGameSprites/pg_ship_brigantine.png");
			textureManager.loadTexture(ShipClass::Frigate, "PirateGameSprites/pg_ship_frigate.png");
			textureManager.loadTexture(ShipClass::Galleon, "PirateGameSprites/pg_ship_galleon.png");
			textureManager.loadTexture(ShipClass::ManOWar, "PirateGameSprites/pg_ship_manowar.png");
		}

		// Texture manager
		TextureResourceManager<ShipClass> textureManager;
	};
}