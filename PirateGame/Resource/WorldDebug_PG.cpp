#include "WorldDebug_PG.h"

using namespace PirateGame;

void DefaultDebugWorld::createWorld(sf::Event event) {
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

void DefaultDebugWorld::updateGameLoop(sf::Event event) {
	updateCoreElements();

	LMH.interactWithLandmasses();

	ESH.update();

	CM.handleCollisions(GQH.get());

	playerShip->update();

	view.updateDebugView(event);

	GQH->getEnemyShipQuadtree()->draw(context.GV.get());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		playerShip->getMovementHandler()->setSpeed(playerShip->getMovementHandler()->getSpeed() + 10.f);
		std::cout << "Speed: " << vm::magnitude(playerShip->getMovementHandler()->getVelocity()) << std::endl;
	}
}