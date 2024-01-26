#include "LeaderBoardMenu.h"

LeaderBoardMenu::LeaderBoardMenu(sf::RenderWindow& window, sf::Font font, GameStateManager* GSM) : Menu(window, font), GSM(GSM), leaderboardData("HighScores.txt") {
}

void LeaderBoardMenu::setUpMenu() {
    // Set the title of the menu
	Menu::setTitleString("Leader Board");

	// Lambda wrapper functions for menu interactions
	returnToTitleFunc = [this](float value) { this->returnToTitleScreen(); };

	// Since the box must contain a name and a score, it must be larger than the default button size
	Menu::setButtonSize(sf::Vector2f(500.f, 60.f));
	Menu::setTextSize(30.f);

	// Add the interactables to the menu
    setUpLeaderBoard();
	Menu::addInteractable("Return to title screen", Interactable::Type::Button, returnToTitleFunc);

	// Make the return to title screen button smaller
	Menu::getInteractableI(10)->setInteractableSize(sf::Vector2f(300.f, 50.f));
	Menu::getInteractableI(10)->setInteractableTextSize(20);
	Menu::getInteractableI(10)->setButtonColor(sf::Color::Magenta);
}

// To draw the menu

void LeaderBoardMenu::drawMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
	// Make sure the menu is drawn once
    if (Menu::needsRedraw()) {
		Menu::drawMenu(currentEvent);
		Menu::resetRedrawFlag();
	}

	// Set individual colors
	Menu::setBackgroundColor(backgroundColor);
	Menu::setTextColor(textColor);
	Menu::setButtonColor(buttonColor);
	Menu::setSliderColor(sliderTrackColor);
	Menu::setSliderComplementColor(sliderThumbColor);
	Menu::setHoverColor(interactableHoverColor);

	// Toggle the menu if the escape key is pressed
	Menu::toggleMenu(mainClock, crosshair);
	Menu::openMenu(currentEvent, crosshair, mainClock);
}

// Functions for the menu interactions

void LeaderBoardMenu::returnToTitleScreen() {
	GSM->openStartMenu();
}

// Set up the leader board

void LeaderBoardMenu::setUpLeaderBoard() {
    // However, the menu still needs interactables to be added to it in the form of text boxes
    std::vector<std::pair<std::string, int>> scores = leaderboardData.file_read();

    // Add the leaderboard menu text boxes (only the top 10 scores)
    for (size_t i = 0; i < scores.size(); i++) {
        std::string text = scores[i].first + ": " + std::to_string(scores[i].second);
        Menu::addInteractable(text, Interactable::Type::InfoBox, std::nullopt);
    }
}