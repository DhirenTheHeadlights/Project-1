#include "WorldESAvoid_PG.h"

using namespace PirateGame;

void ESAvoidWorld::setUpLandMasses() {
	LMH.setPlayerShip(playerShip.get());

	// Add 2 land masses, one behind each ship
	LMH.createIsland(sf::Vector2f(-1000.f, 50.f));
	LMH.createIsland(sf::Vector2f(4000.f, 50.f));
}

void ESAvoidWorld::setUpEnemyShips() {
	ESH.setLandmasses(LMH.getLandMasses());
	ESH.setPlayerShip(playerShip.get());
	ESH.setInteractionChance(0);

	// 2 enemy ships
	ESH.addEnemyShip(sf::Vector2f(1000.f, 50.f));
	ESH.addEnemyShip(sf::Vector2f(2500.f, 50.f));

	// Both ships should be headed at each other
	ESH.getShipGroups().at(0)->setDestination(sf::Vector2f(2500.f, 50.f));
	ESH.getShipGroups().at(1)->setDestination(sf::Vector2f(1000.f, 50.f));
}

void ESAvoidWorld::createWorld(sf::Event event) {
	window->clear();

	context.GIH->update();

	// Handle the different game states
	switch (context.GGSM->getCurrentGameState()) {
	case GameState::Start:
		// Draw the main menu
		MH.openMenu(MenuType::StartMenu);
		break;
	case GameState::OptionsMenu:
		// Draw the options menu
		MH.openMenu(MenuType::OptionsMenu);
		break;
	case GameState::End:
		// End the game and close the window
		window->close();
		break;
	case GameState::GameLoop:
		// Run the game loop
		drawGameLoop();
		updateGameLoop(event);
		break;
	}

	// Close if the escape key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	window->display();
}

void ESAvoidWorld::updateGameLoop(sf::Event event) {
	updateCoreElements();

	ESH.update();

	CM.handleCollisions(GQH.get());

	view.updateDebugView(event);

	GQH->getEnemyShipQuadtree()->draw(context.GV.get());
}