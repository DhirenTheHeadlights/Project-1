#pragma once

/// This class will handle the textures for the ships in the game.

#include "TextureHandler_PG.h"

namespace PirateGame {
	class ShipTextureHandler : public TextureHandler {
	public:
		ShipTextureHandler() {
			// Load the textures
			sloop.loadFromFile("PirateGameSprites/pg_ship_sloop.png");
			brig.loadFromFile("PirateGameSprites/pg_ship_brigantine.png");
			frigate.loadFromFile("PirateGameSprites/pg_ship_frigate.png");
			galleon.loadFromFile("PirateGameSprites/pg_ship_galleon.png");
			manowar.loadFromFile("PirateGameSprites/pg_ship_manowar.png");
		};
		~ShipTextureHandler() {};

		// Getters
		sf::Texture& getSloop() { return sloop; }
		sf::Texture& getBrig() { return brig; }
		sf::Texture& getFrigate() { return frigate; }
		sf::Texture& getGalleon() { return galleon; }
		sf::Texture& getManowar() { return manowar; }
		
	private:
		// Textures
		sf::Texture sloop;
		sf::Texture brig;
		sf::Texture frigate;
		sf::Texture galleon;
		sf::Texture manowar;
	};
}