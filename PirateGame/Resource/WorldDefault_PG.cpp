#include "WorldDefault_PG.h"

using namespace PirateGame;

void DefaultWorld::createWorld(sf::Event event) {
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

	// Frame rate calculation
	sf::Time deltaTime = frameRateClock.restart();
	frameRateUpdateTime += deltaTime;
	++frameCount;

	if (frameCount >= numFramesToAverage) {
		float frameRate = frameCount / frameRateUpdateTime.asSeconds();
		frameRateText.setString("FPS: " + std::to_string(static_cast<int>(frameRate)));
		frameRateUpdateTime = sf::Time::Zero;
		frameCount = 0;
	}

	// Set the position of the frame rate text to be in the bottom left corner
	frameRateText.setPosition(view.getView().getCenter().x - window->getSize().x / 2.f, view.getView().getCenter().y + window->getSize().y / 2.f - 2 * frameRateText.getGlobalBounds().height);

	// Experience text is right by it
	experience.setString("Exp: " + std::to_string(playerShip->getExp()) + "/" + std::to_string(playerShip->getExpToLevelUp()));
	experience.setPosition(frameRateText.getPosition().x + frameRateText.getLocalBounds().getSize().x + 10.f, frameRateText.getPosition().y);

	window->display();
}

void DefaultWorld::updateGameLoop(sf::Event event) {
	if (context.GV->getShowHUD()) MH.openMenu(MenuType::HUD);
	if (context.GV->getShowInventory()) MH.openMenu(MenuType::InventoryMenu);

	updateCoreElements();

	view.updateView(playerShip->getSprite().getPosition(), playerShip->getMovementHandler()->getVelocity());

	LMH.interactWithLandmasses();

	if (gameLoopClock.getElapsedTime() > gameLoopWaitTime) {
		ESH.update();
		gameLoopClock.restart();
	}

	CM.handleCollisions(GQH.get());

	playerShip->update();

	context.GV->setShowInventory(context.GIH->isKeyToggled(sf::Keyboard::I));
}