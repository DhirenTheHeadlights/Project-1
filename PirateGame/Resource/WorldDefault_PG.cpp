#include "WorldDefault_PG.h"

using namespace PirateGame;

void DefaultWorld::createWorld(sf::Event event) {
	window->clear();

	Input::update();

	// Handle the different game states
	switch (GameState::currGameState) {
	case GameState::State::Start:
		// Draw the main menu
		MH.openMenu(MenuType::StartMenu);
		break;
	case GameState::State::OptionsMenu:
		// Draw the options menu
		MH.openMenu(MenuType::OptionsMenu);
		break;
	case GameState::State::End:
		// End the game and close the window
		window->close();
		break;
	case GameState::State::GameLoop:
		// Run the game loop
		drawGameLoop();
		updateGameLoop(event);
		break;
	default: ;
	}

	// Close if the escape key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	// Frame rate calculation
	const sf::Time deltaTime = frameRateClock.restart();
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
	if (Globals::showHUD) MH.openMenu(MenuType::HUD);
	if (Globals::showInventory) MH.openMenu(MenuType::InventoryMenu);

	updateCoreElements();

	view.updateView(playerShip->getSprite().getPosition(), playerShip->getMovementHandler()->getVelocity());

	LMH.interactWithLandmasses();

	if (gameLoopClock.getElapsedTime() > gameLoopWaitTime) {
		ESH.update(WH.getWindDirection(), WH.getWindSpeed());
		gameLoopClock.restart();
	}

	CM.handleCollisions();

	playerShip->update(WH.getWindDirection(), WH.getWindSpeed());

	Globals::showInventory = Input::isKeyToggled(sf::Keyboard::I);
}