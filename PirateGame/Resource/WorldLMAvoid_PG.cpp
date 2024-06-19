#include "WorldLMAvoid_PG.h"

using namespace PirateGame;

void LMAvoidWorld::setUpLandMasses() {
	LMH.setPlayerShip(playerShip.get());

	// Starting position of the landmasses
	float startX = 1000.f;
	float startY = 1000.f;

	// Increase distance between landmasses
	float incrementX = 1500.f; // Spread out more horizontally
	float incrementY = 1500.f; // Spread out more vertically

	// Create a zigzag pattern with islands and rocks
	for (int i = 0; i < 9; ++i) { // Adjust the number of iterations based on how many obstacles you want
		float x = startX + i * incrementX;
		float y = startY + i * incrementY;

		// Alternate adding to the x and y position to create a zigzag
		if (i % 2 == 0) {
			x += 750.f; // Adjust offset to change the amplitude of the zigzag
		}
		else {
			y += 750.f;
		}

		// Create islands at main points
		LMH.createLandmass(LandMassType::Island, sf::Vector2f(x, y));

		// Create rocks in between islands
		if (i > 0) { // Ensure there is a previous point to place a rock between
			float prevX = startX + (i - 1) * incrementX;
			float prevY = startY + (i - 1) * incrementY;

			if ((i - 1) % 2 == 0) {
				prevX += 750.f;
			}
			else {
				prevY += 750.f;
			}

			// Calculate midpoint for the rock
			float rockX = (x + prevX) / 2;
			float rockY = (y + prevY) / 2;

			LMH.createLandmass(LandMassType::Rock, sf::Vector2f(rockX, rockY));
		}
	}
}


void LMAvoidWorld::setUpEnemyShips() {
	ESH.setLandmasses(LMH.getLandMasses());
	ESH.setPlayerShip(playerShip.get());
	ESH.addEnemyShip(sf::Vector2f(500.f, 50.f));
	ESH.getShipGroups().at(0)->setDestination(sf::Vector2f(10000.f, 10000.f));
}

void LMAvoidWorld::createWorld(sf::Event event) {
	window->clear();

	GlobalInputHandler::getInstance().update();

	// Handle the different game states
	switch (GlobalGameStateManager::getInstance().getCurrentGameState()) {
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

void LMAvoidWorld::updateGameLoop(sf::Event event) {
	GlobalMap::getInstance().updateChunks(playerShip->getSprite().getPosition());

	GlobalWindController::getInstance().update();
	GlobalQuadtreeHandler::getInstance().updateQuadtrees();

	background.setPosition(view.getView().getCenter().x - window->getView().getSize().x / 2.f, view.getView().getCenter().y - window->getView().getSize().y / 2.f);
	background.setScale(window->getView().getSize().x / background.getSize().x, window->getView().getSize().y / background.getSize().y);

	//playerShip->update();

	ESH.update();

	CM.handleCollisions();

	view.updateDebugView(event);

	GlobalQuadtreeHandler::getInstance().getLandMassQuadtree()->draw(window);


	// Click to change ship destination
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && GlobalValues::getInstance().getWindow()->hasFocus()) {
		sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		ESH.getShipGroups().at(0)->setDestination(mousePos);
	}
}