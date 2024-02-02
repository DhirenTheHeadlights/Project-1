#include "World_PG.h"

using namespace PirateGame;

World::World(sf::RenderWindow& window) : window(window) {
	map.grid(height, width, cellSize);
	ship.createShip(ShipType::Player, ShipClass::Galleon);
	LMHandler.addLandMasses(100, 500.f);
}

World::~World() {
}

void World::createWorld(sf::Event event) {
	window.clear();

	// Handle the different game states
	switch (GSM.getCurrentGameState()) {
		case GameState::Start:
			// Draw the main menu
			MH.openMenu(MenuType::);
			break;
		case GameState::OptionsMenu:
			// Draw the options menu
			break;
		case GameState::LandMassInteraction:
			// Interact with the land masses
			break;
		case GameState::End:
			// End the game
			break;
		case GameState::GameLoop:
			// Run the game loop
			gameLoop();
			break;
	}

	gameLoop();

	window.display();
}

void World::gameLoop() {
	// Temp background rect for water
	sf::RectangleShape background(sf::Vector2f(10000, 10000));
	background.setFillColor(sf::Color(0, 158, 163));
	window.draw(background);


	// Temporary code to draw a grid
	//map.drawGrid(window);

	// Draw the land masses
	LMHandler.drawLandMasses(window, ship);

	// Draw the ship
	ship.draw(worldMap);
	view.setCenter(ship.getSpritePosition());



	// Temporary code to close the window
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}
}