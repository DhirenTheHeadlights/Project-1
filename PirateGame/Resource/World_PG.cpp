#include "World_PG.h"

using namespace PirateGame;

World::World(sf::RenderWindow* window_in, bool debug) {
	// Set the window
	GlobalValues::getInstance().setWindow(window_in);
	window = GlobalValues::getInstance().getWindow();
	view.setUpView();

	// Set up the map
	GlobalMap::getInstance().initializeMap();
	
	// Set up the pointers
	playerShip = std::make_unique<PlayerShip>();
	LMHandler = std::make_unique<LandMassHandler>();
	MH = std::make_unique<MenuHandler>();
	CM = std::make_unique<CollisionManager>();
	ESH = std::make_unique<EnemyShipHandler>();

	// Set up the values
	if (debug) {
		numLandMasses = numLandMassesDebug;
		distanceBetweenLandMasses = distanceBetweenLandMassesDebug;
		numEnemyShips = numEnemyShipsDebug;
		distanceBetweenEnemyShips = distanceBetweenEnemyShipsDebug;
	}
	else {
	}

	// Set up the world
	setUpWorld();
}

void World::setUpWorld() {
	// Set up the player ship
	playerShip->setUpShip(ShipClass::Frigate);
	playerShip->getCannonHandler().setCannonballHashmap(GlobalHashmapHandler::getInstance().getCannonballHashmap());

	// Set up the land masses
	LMHandler->addLandMasses(numLandMasses, distanceBetweenLandMasses);

	// Set up the enemy ships
	ESH->setLandmasses(LMHandler->getLandMasses());
	ESH->addEnemyShips(numEnemyShips);
	ESH->setPlayerShip(playerShip.get());

	// Set up the collision manager
	CM->setPlayerShips(playerShip.get());
	CM->setEnemyShips(ESH->getEnemyShips());
	CM->setLandMasses(LMHandler->getLandMasses());

	// Set the game state to start
	GlobalGameStateManager::getInstance().changeGameState(GameState::Start);

	// Set up the menus
	MH->createMenus();
	MH->setUpMenus();

	// Set up the view
	view.setUpView();

	// Set up the hud minimap
	MH->getHUD()->getMinimap().setLandmasses(LMHandler->getLandMasses());
	MH->getHUD()->getMinimap().setEnemyShips(ESH->getEnemyShips());
	MH->getHUD()->setPlayerShip(*playerShip.get());

	// Set up the frame rate text
	frameRateText.setFont(*GlobalFontHandler::getInstance().getGlobalFont());
	frameRateText.setCharacterSize(24);
	frameRateText.setFillColor(sf::Color::White);

	// Set up the background
	background.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	background.setFillColor(backgroundColor);
}

void World::createWorld(sf::Event event) {
	window->clear();

	GlobalInputHandler::getInstance().update();

	GlobalGameStateManager* GSM = &GlobalGameStateManager::getInstance();

	// Handle the different game states
	switch (GSM->getCurrentGameState()) {
	case GameState::Start:
		// Draw the main menu
		MH->openMenu(MenuType::StartMenu);
		break;
	case GameState::OptionsMenu:
		// Draw the options menu
		MH->openMenu(MenuType::OptionsMenu);
		break;
	case GameState::LandMassInteraction:
		// Interact with the land masses
		break;
	case GameState::End:
		// End the game and close the window
		window->close();
		break;
	case GameState::GameLoop:
		// Run the game loop
		drawGameLoop();
		gameLoop();
		if (GlobalValues::getInstance().getShowHUD()) MH->openMenu(MenuType::HUD);
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

	// Temporary code to close the window
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	window->display();
}

void World::gameLoop() {
	// Update the map
	GlobalMap::getInstance().updateChunks(playerShip->getSprite().getPosition());
	GlobalMap::getInstance().getMapAtCurrentChunk(playerShip->getSprite().getPosition())->drawGrid(*window);

	GlobalWindController::getInstance().update();

	background.setPosition(view.getView().getCenter().x - window->getSize().x / 2.f, view.getView().getCenter().y - window->getSize().y / 2.f);

	LMHandler->interactWithLandmasses(playerShip.get());

	ESH->update();

	CM->handleCollisions();

	playerShip->update();

	view.setCenter(playerShip->getSprite().getPosition());

	// For debugging purposes, this line sets the view to an enemy ship
	//view.setCenter(ESH->getEnemyShips()[0]->getSprite().getPosition());
}

void World::drawGameLoop() {
	window->draw(background);
	LMHandler->drawLandMasses();
	window->draw(frameRateText);
	playerShip->draw();
	ESH->draw();
}

