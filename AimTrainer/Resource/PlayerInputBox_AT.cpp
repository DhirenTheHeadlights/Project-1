#include "PlayerInputBox_AT.h"

PlayerInputBox::PlayerInputBox() {
}

// Setters for the player input box

void PlayerInputBox::setFont(sf::Font& font) {
	playerInputText.setFont(font);
}

void PlayerInputBox::setDefaultText(std::string text_in) {
	text = text_in; // This sets the default text that is displayed in the box'
    setString(text_in);
    defaultText = text_in;
}

void PlayerInputBox::setOnlyNumbers(bool onlyNumbers) {
	this->onlyNumbers = onlyNumbers;
}

void PlayerInputBox::setMaxCharacters(int maxCharacters) {
	this->maxCharacters = maxCharacters;
}

void PlayerInputBox::setString(std::string text) {
	// Make sure the text is not longer than the max characters
    if (text.length() > maxCharacters) {
		text = text.substr(0, maxCharacters);
	}
    playerInputText.setString(text);
}

// Draw the player input box

void PlayerInputBox::drawPlayerInputBox(sf::RenderWindow& window, sf::Event event) {
	createPlayerInputBox(window, event);
	window.draw(playerInputBox);
	window.draw(playerInputText);
}

// Main method for player input box creation and interaction

void PlayerInputBox::createPlayerInputBox(sf::RenderWindow& window, sf::Event event) {
    std::cout << text << std::endl;
    // Set the position of the player input text
    float x = playerInputBox.getPosition().x + playerInputBox.getSize().x / 2 - playerInputText.getGlobalBounds().width / 2;
    float y = playerInputBox.getPosition().y + playerInputBox.getSize().y / 2 - playerInputText.getGlobalBounds().getSize().y;
    playerInputText.setPosition(x, y);

    // Get mouse position relative to the window
    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

    // Handle mouse button press
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        clicked = playerInputBox.getGlobalBounds().contains(mousePos);
    }

    // if the box is empty, set the default text
    if (text == "" && resetTextBox) {
        text = defaultText;
        playerInputText.setString(text);
    }

    // Output if not clicked
    if (!clicked) {
        return;
    }

    // Clear the box of the default text
    if (text == defaultText) {
		text = "";
		playerInputText.setString(text);
	}

    // Handle backspace
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace && text.length() > 0) {
            text.erase(text.length() - 1);
            playerInputText.setString(text);
        }
        if (event.key.code == sf::Keyboard::Return) {
			clicked = false;
		}
    }

    // Handle text input for letters
    if (event.type == sf::Event::TextEntered && event.text.unicode != 8 && !onlyNumbers) {
        if (event.text.unicode < 128 && text.length() < maxCharacters) {
            text += static_cast<char>(event.text.unicode);
            playerInputText.setString(text);
        }
    }
    // Handle text input for numbers only
    else if (event.type == sf::Event::TextEntered && event.text.unicode != 8 && onlyNumbers) {
        if (event.text.unicode >= 48 && event.text.unicode <= 57 && text.length() < maxCharacters) {
			text += static_cast<char>(event.text.unicode);
			playerInputText.setString(text);
		}
	}
}


// Get the box rectangle shape

sf::RectangleShape& PlayerInputBox::getPlayerInputBox() {
	return playerInputBox;
}

// Get the box text

sf::Text& PlayerInputBox::getPlayerInputText() {
	return playerInputText;
}

// Get the clicked variable to update other classes if needed

bool PlayerInputBox::getClicked() {
	return clicked;
}

// Get the input string

std::string PlayerInputBox::getString() {
    return text;
}

// Set if the text box should be reset when clicked out of

void PlayerInputBox::setResetTextBox(bool resetTextBox) {
	this->resetTextBox = resetTextBox;
}