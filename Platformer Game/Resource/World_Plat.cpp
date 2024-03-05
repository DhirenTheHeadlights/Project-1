#include "World_Plat.h"
#include "HUD_Plat.h"

using namespace PlatformerGame;

//World::World(sf::RenderWindow& window) : window(window), player(worldMap, window) {
	//map.grid(len, wid, 10.f);
//}

World::World(sf::RenderWindow& window) : window(window), player(worldMap, window), healthBar(100, sf::Vector2f(50, 50), sf::Vector2f(200, 20)) {
	map.grid(len, wid, 10.f);
}

//Create the world
//void World::createWorld(sf::Event event) {
//	platforms.generatePlatforms(100);
//}

// Game loop
void World::gameLoop(sf::Event event) {

	background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	background.setFillColor(sf::Color(111, 0, 0, 0.8*255));
	background.setPosition(window.getView().getCenter().x - background.getSize().x/2, window.getView().getCenter().y - background.getSize().y/2);
	window.draw(background);


	player.handlePlayerState();
	player.move();
	player.applyGravity();
	player.draw();
	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}
	healthBar.update();
	
	map.drawGrid(window);

	healthBar.draw(window);

	view.setCenter(player.getPosition());
	view.setSize(window.getSize().x, window.getSize().y);

	window.setView(view);

}

