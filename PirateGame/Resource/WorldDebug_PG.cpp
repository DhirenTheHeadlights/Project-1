#include "WorldDebug_PG.h"

using namespace PirateGame;

void DefaultDebugWorld::createWorld(sf::Event event) {
	window->clear();

	GlobalInputHandler::getInstance().update();

	GlobalGameStateManager* GSM = &GlobalGameStateManager::getInstance();

	// Handle the different game states
	switch (GSM->getCurrentGameState()) {
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
	// Update the map
	GlobalMap::getInstance().updateChunks(playerShip->getSprite().getPosition());

	GlobalWindController::getInstance().update();

	background.setPosition(view.getView().getCenter().x - window->getView().getSize().x / 2.f, view.getView().getCenter().y - window->getView().getSize().y / 2.f);
	background.setScale(window->getView().getSize().x / background.getSize().x, window->getView().getSize().y / background.getSize().y);

	LMHandler.interactWithLandmasses(playerShip.get());

	if (gameLoopClock.getElapsedTime() > gameLoopWaitTime) {
		ESH.update();
		gameLoopClock.restart();
	}

	CM.handleCollisions();

	playerShip->update();

	view.updateDebugView(event);
}