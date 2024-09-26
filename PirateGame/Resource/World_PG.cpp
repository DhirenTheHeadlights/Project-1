#include "World_PG.h"
#include "Font_PG.h"

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
	Globals::window = window;
	ChunkHandler::initializeMap();
	view = View(window);

	// Load JSON Values
	context.JSL->loadGameConfig("PirateGame/json/gameConfig.json");
	context.JSL->loadSaveData("PirateGame/json/saveFile.json");

	// Set up the background and load fonts
	waterTiler.initialize();
	Font::setUpFonts();

	// Set up qth
	QuadtreeHandler::setUpQuadtrees();

	// Set up Input Handler
	Input::initializeInput();

	// Load ship properties
	setShipConfig(context.JSL->getGameData().gameConfig);

	// Load region properties
	GameWorldData data = context.JSL->getGameData().gameConfig.gameWorldData;
	ChunkHandler::getRegionHandler().setRegionScaling({ data.sandyShoresScaling, data.murkyMarshesScaling, data.devilsDenScaling, data.shipwreckShoalsScaling, data.stormySeasScaling });
}

void World::setUpPlayerShip() {
	playerShip = std::make_unique<PlayerShip>(context);
	playerShip->setUpShip(static_cast<ShipClass>(context.JSL->getGameData().saveData.playerShipClass));
	playerShip->getSprite().setPosition(context.JSL->getGameData().saveData.playerPosition[0], context.JSL->getGameData().saveData.playerPosition[1]);
	playerShip->getCannonHandler()->setCannonballQuadtree(QuadtreeHandler::cannonballQuadtree.get());
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
	MH.createMenus(WH);
	
	// Set up the hud
	MH.getHUD()->getMinimap().setLandmasses(LMH.getLandMasses());
	MH.getHUD()->getMinimap().setEnemyShips(ESH.getEnemyShips());
	MH.getHUD()->setPlayerShip(*playerShip);

	// Set up the inventory
	MH.getInventoryMenu()->setPlayerShip(playerShip.get());
	MH.setUpMenus();
}

void World::setUpUI() {
	// Set up the frame rate text
	frameRateText.setFont(Font::globalFont);
	frameRateText.setCharacterSize(24u);
	frameRateText.setFillColor(sf::Color::White);

	// Set up the experience text
	experience.setFont(Font::globalFont);
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
	TextQueue::drawTextQueue(Globals::window);
}

void World::updateCoreElements() {
	Clock::update();
	ChunkHandler::updateChunks(Globals::window, playerShip->getSprite().getPosition());
	WH.update();
	QuadtreeHandler::updateQuadtrees(ChunkHandler::getMapBounds());
	TextQueue::updateTextQueue(window);
	view.showCoordsOnCursor(Font::globalFont);
	waterTiler.update();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		playerShip->getMovementHandler()->setSpeed(playerShip->getMovementHandler()->getSpeed() + 2.f);
	}

	// Autosave
	if (Clock::getAutosaveTrigger()) {
		JSONSave::saveData("PirateGame/json/SaveFile.json", playerShip.get());
		std::cout << "Game autosaved!" << std::endl;
	}
	// Save game data if ` is pressed
	if (Input::isKeyPressedOnce(sf::Keyboard::Tilde)) {
		JSONSave::saveData("PirateGame/json/SaveFile.json", playerShip.get());
		std::cout << "Game manual saved!" << std::endl;
	}
}

void World::createWorld(const sf::Event event) {
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

	window->display();
}

