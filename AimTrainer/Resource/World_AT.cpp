#include "World_AT.h"

using namespace AimTrainer;

World::World(sf::RenderWindow& window) : window(window), hashmap(), crosshair(window), leaderboardData("HighScores.txt") {

    // initialize the font
    if (!font.loadFromFile("Fonts/times_new_roman.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }

    // Create the menus
    else {
        // Set up the menu handler
        menuHandler = std::make_unique<MenuHandler>(window, font, this);

        // Set up the menus
        menuHandler->setUpMenus();
    }

    // Set up the background rectangle
    backgroundRect.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    backgroundRect.setFillColor(sf::Color(0, 191, 255));
    
    // Set up the arena box
    arenaBox.setSize(sf::Vector2f(window.getSize().x - 200.f, window.getSize().y - 200.f));
    arenaBox.setFillColor(sf::Color::White);
    arenaBox.setPosition(100.f, 100.f);
}

///////////////////////////////////////////////////////////////////
// This function is called from main.cpp //////////////////////////
///////////////////////////////////////////////////////////////////

void World::createWorld(sf::Event eventFromMain) {
    // Clear the window and assign eventFromMain to currentEvent
    window.clear();

    // Handle different game states
    switch (currentGameState) {

    case GameState::TitleScreen:
        handleTitleScreen(eventFromMain);
        break;

    case GameState::GameLoop:

        // The game loop
        window.draw(backgroundRect);
        window.draw(arenaBox);
        updateTargets();
        handleTargetCollision(eventFromMain);
        showTimer();
        crosshair.draw(true);

        handleOptionsMenu(eventFromMain); // Check and handle options menu

        break;

    case GameState::EndScreen:
        handleEndScreen(eventFromMain);
        break;

    case GameState::LeaderBoardScreen:
        handleLeaderBoardScreen(eventFromMain);
        break;

    case GameState::OptionsMenu:
        handleOptionsMenu(eventFromMain);

        if (!menuHandler->getOptionsMenu()->getIsMenuOpen()) {
            currentGameState = GameState::GameLoop;
        }

        break;

    case GameState::InsertNameMenu:
        handleInsertNameMenu(eventFromMain);

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        endGame();
		window.close();
	}

    //std::cout << name << std::endl;

    window.display();
}

void World::gameLoop(sf::Event event) {

	// The game loop
	window.draw(arenaBox);
	updateTargets();
	handleTargetCollision(event);
	showTimer();
	crosshair.draw(true);

	handleOptionsMenu(event); // Check and handle options menu
}

// This function updates the targets on the screen

void World::updateTargets() {
    // Remove all deactivated targets from the activeTargets vector
    activeTargets.erase(
        std::remove_if(
            activeTargets.begin(), activeTargets.end(),
            [](Target* target) { return !target->isActive(); }
        ),
        activeTargets.end()
    );
    // Draw new targets if there are less than the maxTargets
    if (activeTargets.size() < maxTargets) {
		createTarget();
	}
	// Draw all active targets
    for (Target* target : activeTargets) {
		window.draw(target->getShape());
        hashmap.addTarget(target);
	}
}

// This function creates a new target and adds it to the activeTargets vector

Target* World::createTarget() {
    // Get the position and size of the arenaBox
    sf::Vector2f arenaPos = arenaBox.getPosition();
    sf::Vector2f arenaSize = arenaBox.getSize();

    // Adjust the target position to be within the bounds of arenaBox
    sf::Vector2f pos = sf::Vector2f(
        std::rand() % (static_cast<unsigned int>(arenaSize.x) - static_cast<unsigned int>(size)) + arenaPos.x + size / 2,
        std::rand() % (static_cast<unsigned int>(arenaSize.y) - static_cast<unsigned int>(size)) + arenaPos.y + size / 2
    );

    // Create a new target and add it to the activeTargets vector
    Target* target = new Target(pos, size);
    activeTargets.push_back(target);
    return target;
}

// This function handles mouse events

void World::handleTargetCollision(sf::Event event) {
    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    sf::Vector2f windowSize = sf::Vector2f(window.getSize());

    if (mousePos.x < 0 || mousePos.x > windowSize.x || mousePos.y < 0 || mousePos.y > windowSize.y) {
        return; // Exit if the mouse is outside the window
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (!mousePressed && currentGameState == GameState::GameLoop) {
            Target* target = hashmap.getTargetAtCollision(mousePos, size);
            if (target != nullptr) {
                target->deActivate();
                target->~Target();
                score += 100; // Increase score for hitting a target
            }
            else {
                score -= 50; // Decrease score for a missed click
            }
        }
        mousePressed = true;
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        mousePressed = false;
    }
}

// This function displays the timer and score

void World::showTimer() {

    if (currentGameState == GameState::GameLoop) {

        // Calculate the time left in the game
        int timeElapsed = static_cast<int>(mainClock.getElapsedTime().asSeconds() - menuHandler->getOptionsMenu()->getTotalTimeInMenu().asSeconds());
        int timeLeft = timerTime - timeElapsed;
        // Check if the game is running and display the score
        if (timeLeft > 0) {
            std::string scoreAndTime = "Time Left: " + std::to_string(timeLeft) + "  Score: " + std::to_string(score);
            sf::Text timerText;
            timerText.setFont(font);
            timerText.setString(scoreAndTime);
            timerText.Bold;
            timerText.setFillColor(sf::Color::White);
            timerText.setPosition(window.getSize().x / 2 - timerText.getGlobalBounds().width / 2, 0);
            timerText.setCharacterSize(40);
            window.draw(timerText);
        }

        // if the game is over, display the score and reset the game
        else {
            currentGameState = GameState::EndScreen;
        }
    }
}

// This function ends the game

void World::endGame() {
    if (score != 0) {
		scores.push_back(std::make_pair(name, score));
        leaderboardData.addData(scores);
	}
	window.close();
}

///////////////////////////////////////////////////////////////////
// Functions for handling different game Menus ////////////////////
///////////////////////////////////////////////////////////////////

void World::handleTitleScreen(sf::Event event) {
    menuHandler->drawStartMenu(event, mainClock, crosshair);
}

void World::handleEndScreen(sf::Event event) {
    menuHandler->drawEndMenu(event, mainClock, crosshair);
}

void World::handleLeaderBoardScreen(sf::Event event) {
    menuHandler->drawLeaderBoardMenu(event, mainClock, crosshair);
}

void World::handleOptionsMenu(sf::Event event) {
    menuHandler->drawOptionsMenu(event, mainClock, crosshair);
}

void World::handleInsertNameMenu(sf::Event event) {
	menuHandler->drawInsertNameMenu(event, mainClock, crosshair);
}

///////////////////////////////////////////////////////////////////
// Overridden Functions for the game //////////////////////////////
///////////////////////////////////////////////////////////////////

void World::changeGameState(GameState newState) {
	currentGameState = newState;
}

GameState World::getCurrentGameState() const {
	return currentGameState;
}

void World::startGame() {
	currentGameState = GameState::GameLoop;
	mainClock.restart();
    scores.push_back(std::make_pair(name, score));
    score = 0;
}

void World::setName(std::string name_in) {
    if (name_in != "") {
        name = name_in;
    }
}

void World::openLeaderBoardMenu() {
	currentGameState = GameState::LeaderBoardScreen;
}

void World::openOptionsMenu() {
	currentGameState = GameState::OptionsMenu;
}

void World::openStartMenu() {
	currentGameState = GameState::TitleScreen;
}

void World::openEndScreen() {
	currentGameState = GameState::EndScreen;
}

void World::openInsertNameMenu() {
	currentGameState = GameState::InsertNameMenu;
}

void World::exit() {
	endGame();
}
