#include "World_PG.h"


using namespace PirateGame;

void World::setUpWorld() {
	setUpWorldElements();
	setUpPlayerShip();
	setUpLandMasses();
	setUpEnemyShips();
	setUpCollisionManager();
	setUpMenus();
	setUpUI();

	// Set up the view
	view.setUpView(playerShip->getSprite().getPosition(), sf::Vector2f(window->getSize()));
}

void World::setUpWorldElements() {
	// Set up the window and map
	context.GV->setWindow(window);
	context.GCH->initializeMap();
	view = View(window);

	// Load JSON Values
	context.JSL->loadGameConfig("PirateGame/json/gameConfig.json");
	context.JSL->loadSaveData("PirateGame/json/saveFile.json");

	// Set up the background
	waterTiler.initialize();

	// Set up qth
	GQH->setUpQuadtrees(context.GCH.get());

	// Load ship properties
	setShipConfig(context.JSL->getGameData().gameConfig);
}

void World::setUpPlayerShip() {
	playerShip = std::make_unique<PlayerShip>(context);
	playerShip->setUpShip(playerShipClass);
	playerShip->getCannonHandler()->setCannonballQuadtree(GQH->getCannonballQuadtree());
}

void World::setUpLandMasses() {
	LMH.setPlayerShip(playerShip.get());
	LMH.addLandMasses(numLandMasses, distanceBetweenLandMasses);
}

void World::setUpEnemyShips() {
	ESH.setLandmasses(LMH.getLandMasses());
	ESH.addEnemyShips(numEnemyShips);
	ESH.setPlayerShip(playerShip.get());
}

void World::setUpCollisionManager() {
	CM.setPlayerShip(playerShip.get());
	CM.setEnemyShips(ESH.getEnemyShips());
	CM.setLandMasses(LMH.getLandMasses());
}

void World::setUpMenus() {
	MH.createMenus();
	
	// Set up the hud
	MH.getHUD()->getMinimap().setLandmasses(LMH.getLandMasses());
	MH.getHUD()->getMinimap().setEnemyShips(ESH.getEnemyShips());
	MH.getHUD()->setPlayerShip(*playerShip.get());

	// Set up the inventory
	MH.getInventoryMenu()->setPlayerShip(playerShip.get());
	MH.setUpMenus();
}

void World::setUpUI() {
	// Set up the frame rate text
	frameRateText.setFont(*context.GFH->getGlobalFont());
	frameRateText.setCharacterSize(24u);
	frameRateText.setFillColor(sf::Color::White);

	// Set up the experience text
	experience.setFont(*context.GFH->getGlobalFont());
	experience.setCharacterSize(24u);
	experience.setFillColor(sf::Color::White);
}

void World::drawGameLoop() {
	waterTiler.draw(window);
	LMH.drawLandMasses();
	window->draw(frameRateText);
	window->draw(experience);
	playerShip->draw();
	ESH.draw();
	context.GTQP->drawTextQueue(context.GV->getWindow());
}

void World::updateCoreElements() {
	context.GC->update();
	context.GCH->updateChunks(context.GV->getWindow(), playerShip->getSprite().getPosition());
	context.GWC->update();
	GQH->updateQuadtrees(context.GCH->getMapBounds());
	context.GTQP->updateTextQueue(window);
	view.showCoordsOnCursor(*context.GFH->getGlobalFont());
	waterTiler.update();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		playerShip->getMovementHandler()->setSpeed(playerShip->getMovementHandler()->getSpeed() + 2.f);
	}

	// Autosave
	if (context.GC->getAutosaveTrigger()) {
		JSONSave::saveData("PirateGame/json/SaveFile.json", playerShip.get());
		std::cout << "Game autosaved!" << std::endl;
	}
	// Save game data if ` is pressed
	if (context.GIH->isKeyPressedOnce(sf::Keyboard::Tilde)) {
		JSONSave::saveData("PirateGame/json/SaveFile.json", playerShip.get());
		std::cout << "Game manual saved!" << std::endl;
	}
}

void World::createWorld(sf::Event event) {
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

