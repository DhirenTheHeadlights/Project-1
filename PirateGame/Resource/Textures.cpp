#include "Textures.h"

// Constructor

Textures::Textures() {
	// Load all ship textures
	sf::Texture sloop, brigantine, frigate, manowar, galleon;
	sloop.loadFromFile("PirateGameSprites/pg_ship_sloop.png");
	brigantine.loadFromFile("PirateGameSprites/pg_ship_brigantine.png");
	frigate.loadFromFile("PirateGameSprites/pg_ship_frigate.png");
	manowar.loadFromFile("PirateGameSprites/pg_ship_manowar.png");
	galleon.loadFromFile("PirateGameSprites/pg_ship_manowar.png");
	for (auto& i : { sloop, brigantine, frigate, manowar, galleon }) {
		shipTextures.push_back(i);
	}

	// Load all island textures
	sf::Texture island1, island2;
	island1.loadFromFile("PirateGameSprites/pg_island_1.png");
	island2.loadFromFile("PirateGameSprites/pg_island_2.png");
	for (auto& i : { island1, island2 }) {
		islandTextures.push_back(i);
	}

	// Load all rock textures
	sf::Texture rock1, rock2, rock3, rock4, rock5, rock6, rock7, rock8, rock9;
	rock1.loadFromFile("PirateGameSprites/pg_rock_1.png");
	rock2.loadFromFile("PirateGameSprites/pg_rock_2.png");
	rock3.loadFromFile("PirateGameSprites/pg_rock_3.png");
	rock4.loadFromFile("PirateGameSprites/pg_rock_4.png");
	rock5.loadFromFile("PirateGameSprites/pg_rock_5.png");
	rock6.loadFromFile("PirateGameSprites/pg_rock_6.png");
	rock7.loadFromFile("PirateGameSprites/pg_rock_7.png");
	rock8.loadFromFile("PirateGameSprites/pg_rock_8.png");
	rock9.loadFromFile("PirateGameSprites/pg_rock_9.png");
	for (auto& i : { rock1, rock2, rock3, rock4, rock5, rock6, rock7, rock8, rock9 }) {
		rockTextures.push_back(i);
	}

	// Load cannonball texture
	cannonball.loadFromFile("PirateGameSprites/pg_misc_cannonball.png");
}

// Deconstructor
Textures::~Textures() {};

// Returns ship texture
sf::Texture& Textures::grabShipTexture(int index) {
	return shipTextures[index];
}

// Returns island texture
sf::Texture& Textures::grabIslandTexture(int index) {
	if (index != -1) return islandTextures[index];
	else {
		return islandTextures[std::rand() % (islandTextures.size() - 1)];
	}
}

// Returns rock texture
sf::Texture& Textures::grabRockTexture(int index) {
	if (index != -1) return rockTextures[index];
	else {
		return rockTextures[std::rand() % (islandTextures.size() - 1)];
	}
}