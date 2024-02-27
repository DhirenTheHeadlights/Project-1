#include "World_Plat.h"

world_plat::world_plat(sf::RenderWindow& window) : window(window), player(map, window) {
}

// Game loop
void world_plat::gameLoop(sf::Event event) {
	player.handlePlayerState();
	player.move();
	player.applyGravity();
	player.draw();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}
}

