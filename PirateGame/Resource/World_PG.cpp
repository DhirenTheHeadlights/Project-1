#include "World_PG.h"

World_PG::World_PG(sf::RenderWindow& window) : window(window), view(window), ship(window), LMHandler(map) {
	map.grid(height, width, cellSize);
	ship.createShip(ShipType::Player, ShipClass::Galleon);
	LMHandler.addLandMasses(100, 500.f);
}

World_PG::~World_PG() {
}

void World_PG::createWorld() {
	window.clear();

	// Handle the different game states
	switch (GSM.getCurrentGameState()) {
		case GameState_PG::Start:
			// Draw the main menu
			break;
		case GameState_PG::OptionsMenu:
			// Draw the options menu
			break;
		case GameState_PG::LandMassInteraction:
			// Interact with the land masses
			break;
		case GameState_PG::End:
			// End the game
			break;
		case GameState_PG::GameLoop:
			// Run the game loop
			gameLoop();
			break;
	}

	gameLoop();

	window.display();
}

void World_PG::gameLoop() {
	// Temporary code to draw a grid
	map.drawGrid(window);

	// Draw the land masses
	LMHandler.drawLandMasses(window, ship);

	// Draw the ship
	ship.draw(worldMap);
	view.setCenter(ship.getPosition());



	// Temporary code to close the window
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.close();
	}
}