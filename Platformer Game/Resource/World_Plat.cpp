#include "World_Plat.h"

using namespace PlatformerGame;

World::World(sf::RenderWindow& window) : window(window), player(worldMap, window) {
	map.initialize(len, wid, 10);
}

// Game loop
void World::gameLoop(sf::Event event) {
	player.handlePlayerState();
	player.move();
	player.applyGravity();
	player.draw();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}

	map.drawGrid(window);
	view.setCenter(player.getPosition());
	view.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
	window.setView(view);
}

