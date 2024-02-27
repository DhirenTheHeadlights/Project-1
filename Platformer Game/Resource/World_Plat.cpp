#include "World_Plat.h"

world_plat::world_plat(sf::RenderWindow& window) : window(window), player(worldMap, window) {
	map.grid(len, wid, 10);
}

void createGame(sf::Event event) {
	
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

	map.drawGrid(window);
	view.setCenter(player.getPosition());
	view.setSize(window.getSize().x, window.getSize().y);
	window.setView(view);	
}

