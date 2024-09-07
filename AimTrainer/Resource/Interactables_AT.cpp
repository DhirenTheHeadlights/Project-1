#include "Interactables_AT.h"

Interactable::Interactable(InteractableFunction func, Type type) : interactableFunc(std::move(func)), interactableType(type) {
    // Load font and set text properties and button function
    if (font.loadFromFile("Fonts/times_new_roman.ttf")) {
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
    }
    else {
        std::cout << "Error loading font" << std::endl;
    }
}

void Interactable::createInteractable(std::string name, float len, float wid, float size) {
    // Set the length, width, and text size of the interactable
    length = len;
    width = wid;
    textSize = size;

    // background rectangle
    backgroundRect.setSize(sf::Vector2f(length, width));
    backgroundRect.setFillColor(sf::Color::Transparent);
    backgroundRect.setOutlineThickness(2.0f);
    backgroundRect.setOutlineColor(sf::Color::Black);

    // Set the size and color of the interactable if it's a button or info box
    if (interactableType == Type::Button || interactableType == Type::InfoBox) {
        rectHorizontal.setSize(sf::Vector2f(length, width));    
        rectHorizontal.setFillColor(buttonColor);
    }

    // If the interactable is a slider, create the vertical rectangle and set its color
    else if (interactableType == Type::Slider) {

        // Set the background rect of the slider to be bigger to account for the inputbox
        float inputBoxX = sliderInputBox.getPlayerInputBox().getSize().x;
        backgroundRect.setSize(sf::Vector2f(length + inputBoxX + inputBoxOffset, width / 5.0f));
        
        // horizontal rectangle for slider
        rectHorizontal.setSize(sf::Vector2f(length + inputBoxOffset, width / 5.0f));
        rectHorizontal.setFillColor(sliderColor);

        // vertical rectangle for slider
        rectVertical.setSize(sf::Vector2f(length / 10.0f, width / 5.0f));
        rectVertical.setFillColor(sliderColorComplement);

        // Input box for the slider
        sliderInputBox.setFont(font);
        sliderInputBox.getPlayerInputBox().setSize(sf::Vector2f(length / 10.0f, width / 5.0f));
        sliderInputBox.getPlayerInputBox().setFillColor(sf::Color::Transparent);
        sliderInputBox.getPlayerInputBox().setOutlineThickness(2.0f);
        sliderInputBox.getPlayerInputBox().setOutlineColor(sf::Color::Black);

        // Text for the input box
        sliderInputBox.getPlayerInputText().setFillColor(textColor);
        sliderInputBox.getPlayerInputText().setCharacterSize(static_cast<unsigned int>(textSize));
        sliderInputBox.setOnlyNumbers(true);
        sliderInputBox.setMaxCharacters(2);
        sliderInputBox.setResetTextBox(false);
    }

    // If the interactable is an input box, create the input box
    else if (interactableType == Type::InputBox) {
		rectHorizontal.setSize(sf::Vector2f(length, width));
		rectHorizontal.setFillColor(buttonColor);

        // Set the font and text properties of the input box
        inputBox.setFont(font);
        inputBox.setDefaultText(name);
        inputBox.getPlayerInputText().setFillColor(textColor);
        inputBox.getPlayerInputText().setCharacterSize(static_cast<unsigned int>(textSize));

        // Set properties of the input box
        float padding = 20.0f;
        inputBox.getPlayerInputBox().setSize(sf::Vector2f(length, width));
        inputBox.setOnlyNumbers(false);
        inputBox.setMaxCharacters(20);
        inputBox.getPlayerInputBox().setFillColor(sf::Color::Transparent);
        inputBox.getPlayerInputBox().setOutlineThickness(6.0f);
        inputBox.getPlayerInputBox().setOutlineColor(sf::Color::Black);
	}

    // If the interactable is not a player input box, set the text of the interactable
    if (interactableType != Type::InputBox) {
        // Set the text of the interactable
        text = sf::Text(name, font);
        text.setCharacterSize(static_cast<unsigned int>(textSize));
        text.setFillColor(textColor);
	}
}

void Interactable::interact(sf::RenderWindow& window, sf::Clock& cooldown) {
    // If the interactable is a button, call the button function when the button is pressed
    if (interactableType == Type::Button) {
        buttonPress(window, cooldown);
    }
    // If the interactable is a slider, move the slider when the mouse is dragged
    else if (interactableType == Type::Slider) {
        sliderDrag(window);

        // Adjust the slider length to account for the width of the slider thumb
        float adjustedSliderLength = length - rectVertical.getSize().x;

        // Calculate the percentage position of the slider
        float sliderPosition = rectVertical.getPosition().x - rectHorizontal.getPosition().x;
        float percent = (sliderPosition / adjustedSliderLength) * 100;

        // Ensure the percentage is within 0 to 100
        percent = std::max(0.0f, std::min(percent, 100.0f));

        // Convert the percentage to a string and set it to the input box
        std::string text = std::to_string(static_cast<int>(percent));
        sliderInputBox.setString(text);

        // Update the slider position if the player input box is clicked
        if (sliderInputBox.getClicked()) {
            float newOffset = std::stof(sliderInputBox.getPlayerInputText().getString().toAnsiString());
            newOffset = std::max(0.0f, std::min(newOffset, 100.0f));
            float newPosition = rectHorizontal.getPosition().x + (newOffset / 100.0f) * adjustedSliderLength;
            rectVertical.setPosition(newPosition, rectVertical.getPosition().y);
        }
    }
}

void Interactable::buttonPress(sf::RenderWindow& window, sf::Clock& cooldown) {
    if (cooldown.getElapsedTime().asSeconds() < 0.5) { // 0.5 seconds cooldown
        return; // Do not process the button press if we're in cooldown
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isMouseOverButton(window)) {
        callButtonAction();
        cooldown.restart(); // Restart the cooldown clock after a button press
    }

    else if (isMouseOverButton(window)) {
        rectHorizontal.setFillColor(hoverColor); // Change the color of the button if the mouse is over it
	}
}

bool Interactable::isMouseOverButton(sf::RenderWindow& window) const {
    // Get the current mouse position in the window
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    // Return true if mouse is within the button's rectangle
    return rectHorizontal.getGlobalBounds().contains(mousePos);
}

void Interactable::callButtonAction() const {
    // Check if there's an associated action and call the correct variant
    if (interactableFunc) {
        std::visit([](auto&& func) {
            // Identify the type of the function and call it
            using T = std::decay_t<decltype(func)>;
            if constexpr (std::is_same_v<T, std::function<void()>>) {
                func(); // Call the function without parameters
            }
            else if constexpr (std::is_same_v<T, std::function<void(float)>>) {
                func(1.0f); // Call the function with a float parameter (if needed)
            }
            }, *interactableFunc);
    }
}

void Interactable::sliderDrag(sf::RenderWindow& window) {
    // Check if the left mouse button is pressed
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        return; // Early return if mouse is not pressed
    }

    // Get the current mouse position in the window
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    // Check if the mouse is within the bounds of the horizontal rectangle of the slider
    if (!rectHorizontal.getGlobalBounds().contains(mousePos)) {
        return; // Early return if mouse is outside the slider bounds
    }

    // Update the slider's position based on mouse position
    updateSliderPositionVariable(mousePos);

    // Call the associated action function, if it exists
    callSliderAction();
}

void Interactable::updateSliderPositionVariable(const sf::Vector2f& mousePos) {
    // Calculate the new X position for the slider, constrained within the horizontal bar
    float min = std::min(mousePos.x, rectHorizontal.getPosition().x + rectHorizontal.getSize().x - rectVertical.getSize().x);
    offset = std::max(rectHorizontal.getPosition().x, min);
}

void Interactable::callSliderAction() const {
    // Calculate the offset between the horizontal and vertical rectangles
    float offset = rectVertical.getPosition().x - rectHorizontal.getPosition().x;

    // Check if there's an associated action and call the correct variant
    if (interactableFunc) {
        std::visit([offset](auto&& func) {
            // Identify the type of the function and call if it's the correct type
            using T = std::decay_t<decltype(func)>;
            if constexpr (std::is_same_v<T, std::function<void(float)>>) {
                func(offset); // If it's a function that takes a float, pass the offset
            }
            // If the action is a function without parameters, we don't call it for slider interaction
            }, *interactableFunc);
    }
}

void Interactable::setPos(sf::Vector2f pos) {
    // Set the position of the interactable
    rectHorizontal.setPosition(pos);
    backgroundRect.setPosition(pos);

    // If the interactable is a slider, set the position of the rectangles differently
    if (interactableType == Type::Slider) {
        // Set the position of the background rectangle to account for the input box and offset
        pos.x -= sliderInputBox.getPlayerInputBox().getSize().x / 2 + inputBoxOffset / 2;
        backgroundRect.setPosition(pos);
        rectHorizontal.setPosition(pos);

        // Inital position of the slider is the center of the horizontal rectangle
        float defaultPosVerticalX = pos.x + rectHorizontal.getSize().x / 2 - rectVertical.getSize().x / 2;
        float defaultPosVerticalY = pos.y + rectHorizontal.getSize().y / 2 - rectVertical.getSize().y / 2;

        // The new position is determined by the offset
        float netOffset = 0;

        // If the offset is 0, the slider is in the center of the horizontal rectangle
        if (offset == 0) {
			netOffset = 0;
		}

        // If the offset is not 0, the slider will be moved accordingly
        else {
            netOffset = offset - defaultPosVerticalX; // This offset is calculated by updateSliderPositionVariable()
        }

        // Update the default position of the vertical rectangle
        defaultPosVerticalX += netOffset;

        rectPosVertical = sf::Vector2f(defaultPosVerticalX, defaultPosVerticalY);
        rectVertical.setPosition(rectPosVertical);

        // Set the position of the input box to be to the right of the horizontal rectangle
        sliderInputBox.getPlayerInputBox().setPosition(pos.x + rectHorizontal.getSize().x + inputBoxOffset, pos.y);
    }

    else if (interactableType == Type::InputBox) {
        // Set the postition of the input box to be centered within the horizontal rect
        float x = pos.x + rectHorizontal.getSize().x / 2 - inputBox.getPlayerInputBox().getSize().x / 2;
        float y = pos.y + rectHorizontal.getSize().y / 2 - inputBox.getPlayerInputBox().getSize().y / 2;
        inputBox.getPlayerInputBox().setPosition(x, y);
    }

    // Set the position of the text
    float x = pos.x + rectHorizontal.getSize().x / 2 - text.getGlobalBounds().width / 2;
    // If the interactable is a slider, the text is offset due to the input box
    if (interactableType == Type::Slider) {
		x = pos.x + sliderInputBox.getPlayerInputBox().getSize().x + rectHorizontal.getSize().x / 2
            - text.getGlobalBounds().width / 2;
	}

    float y = 0;
    // If the interactable is a button/infobox/inputbox, set the position of the text in the center of the button
    if (interactableType == Type::Button || interactableType == Type::InfoBox || interactableType == Type::InputBox) {
        y = pos.y + (rectHorizontal.getSize().y - text.getGlobalBounds().height) / 2;
    }

    // If the interactable is a slider, set the position of the text below the slider
    else {
        y = pos.y + rectHorizontal.getSize().y;
    }
    text.setPosition(x, y);
}

void Interactable::draw(sf::RenderWindow& window, sf::Event event) {
    // Draw the interactable
    window.draw(rectHorizontal);
    window.draw(backgroundRect);
    // If the interactable is a slider, draw the vertical rectangle
    if (interactableType == Type::Slider) {
        window.draw(rectVertical);
        sliderInputBox.drawPlayerInputBox(window, event);
    }
    // If the interactable is an input box, draw the input box
    else if (interactableType == Type::InputBox) {
		inputBox.drawPlayerInputBox(window, event);
	}
    window.draw(text);
}

// Color setting functions

void Interactable::setTextColor(sf::Color color) {
    textColor = color;
    text.setFillColor(color);
}

void Interactable::setButtonColor(sf::Color color) {
    buttonColor = color;
    rectHorizontal.setFillColor(color);
}

void Interactable::setSliderColor(sf::Color color) {
    sliderColor = color;
    backgroundRect.setOutlineColor(color);
}

void Interactable::setSliderComplementColor(sf::Color color) {
    sliderColorComplement = color;
    rectVertical.setFillColor(color);
}

void Interactable::setHoverColor(sf::Color color) {
    hoverColor = color;
}

// Getters for interactable properties

sf::Vector2f Interactable::getSize() const {
    return rectHorizontal.getSize();
}

Interactable::Type Interactable::getInteractableType() const {
	return interactableType;
}

sf::Vector2f Interactable::getPos() const {
	return rectHorizontal.getPosition();
}

PlayerInputBox& Interactable::getSliderInputBox() {
	return sliderInputBox;
}

PlayerInputBox& Interactable::getInputBox() {
	return inputBox;
}

std::string Interactable::getInputString() {
    if (interactableType == Type::InputBox) {
		return inputBox.getString();
	}
    else {
        return "Not a player input box";
    }
}

// Setters for interactable properties

void Interactable::setInteractableSize(sf::Vector2f size) {
	rectHorizontal.setSize(size);
    backgroundRect.setSize(size);
}

void Interactable::setInteractableTextSize(float size) {
	text.setCharacterSize(static_cast<unsigned int>(size));
}