#include "Slider_PG.h"

using namespace PirateGame;

// Set up the slider
void Slider::customInteractableSetUp() {
    this->name = text.getString();
	text.setString(name + ":  " + std::to_string(static_cast<int>(value * 100)));
    text.setOrigin(0.f, 0.f);
}

// Set the position of the slider
void Slider::setPosition(sf::Vector2f pos) {
	// Set the positon of all of the sprites
	sprite.setPosition(pos);

	sf::Vector2f spritePosCentered = sf::Vector2f(pos.x + sprite.getGlobalBounds().getSize().x / 2, pos.y + sprite.getGlobalBounds().getSize().y / 2);
	sf::Vector2f sliderTrackSizeDiv2 = sf::Vector2f(sliderTrack.getGlobalBounds().getSize().x / 2, sliderTrack.getGlobalBounds().getSize().y / 2);
	sliderTrack.setPosition(spritePosCentered - sliderTrackSizeDiv2);

	sf::Vector2f sliderPos = sliderTrack.getPosition();
	sliderThumb.setPosition(sliderPos.x + (sliderTrack.getGlobalBounds().width - sliderThumb.getGlobalBounds().width) * value, 
							sliderPos.y - (sliderThumb.getGlobalBounds().height - sliderTrack.getGlobalBounds().height) / 2);
}

// Interact with the slider
void Slider::interact(sf::RenderWindow* window, GlobalSoundManager* GSM) {
    sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));

    // Start grabbing if the mouse is over the slider thumb and the left mouse button is pressed
    if (sliderThumb.getGlobalBounds().contains(mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isGrabbing = true;
    }

    // If the mouse button is not pressed, set isGrabbing to false
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isGrabbing = false;
    }

    if (isGrabbing) {
        // Calculate the new x position of the slider thumb, keeping it within the bounds of the slider track
        float newX = mousePosition.x - sliderThumb.getGlobalBounds().width / 2;
        newX = std::max(newX, sliderTrack.getPosition().x);
        newX = std::min(newX, sliderTrack.getPosition().x + sliderTrack.getGlobalBounds().width - sliderThumb.getGlobalBounds().width);

        // Set the new position of the slider thumb
        sliderThumb.setPosition(newX, sliderThumb.getPosition().y);

        // Update the slider value and text display
        float value = (newX - sliderTrack.getPosition().x) / (sliderTrack.getGlobalBounds().width - sliderThumb.getGlobalBounds().width);
        text.setString(name + ":  " + std::to_string(static_cast<int>(value * 100)));
        func(value); // Call the function with the new value of the slider
    }
}


// Draw the slider
void Slider::draw(sf::RenderWindow* window) {
	window->draw(sprite);
	window->draw(sliderTrack);
	window->draw(sliderThumb);
	window->draw(text);
}