#include "Slider_PG.h"

using namespace PirateGame;

// Set up the slider
void Slider::customInteractableSetUp() {

	
	text.setString(name + ":  " + std::to_string(static_cast<int>(value * 100)));
}

// Set the position of the slider
void Slider::setPosition(sf::Vector2f pos) {
	// Set the positon of all of the sprites
	sprite.setPosition(pos);
	sliderTrack.setPosition(pos);
	sliderThumb.setPosition(pos.x + (sliderTrack.getGlobalBounds().width - sliderThumb.getGlobalBounds().width) * value, pos.y);
}

// Interact with the slider
void Slider::interact() {
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));
	if (sliderThumb.getGlobalBounds().contains(mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// Set the position of the slider thumb to the mouse position
		sliderThumb.setPosition(static_cast<float>(sf::Mouse::getPosition(*window).x), sliderThumb.getPosition().y);

		// If the slider thumb is outside the bounds of the slider track, set it to the bounds
		if (sliderThumb.getPosition().x < sliderTrack.getPosition().x) {
			sliderThumb.setPosition(sliderTrack.getPosition().x, sliderThumb.getPosition().y);
		}

		// Set the position of the slider thumb to the bounds of the slider track
		if (sliderThumb.getPosition().x > sliderTrack.getPosition().x + sliderTrack.getGlobalBounds().width - sliderThumb.getGlobalBounds().width) {
			sliderThumb.setPosition(sliderTrack.getPosition().x + sliderTrack.getGlobalBounds().width - sliderThumb.getGlobalBounds().width, sliderThumb.getPosition().y);
		}
		// Set the text to display the value of the slider
		text.setString(name + ":  " + std::to_string(static_cast<int>((sliderThumb.getPosition().x - sliderTrack.getPosition().x) / (sliderTrack.getGlobalBounds().width - sliderThumb.getGlobalBounds().width) * 100)));

		// Call the function with the value of the slider
		func((sliderThumb.getPosition().x - sliderTrack.getPosition().x) / (sliderTrack.getGlobalBounds().width - sliderThumb.getGlobalBounds().width));
	}
	
}

// Draw the slider
void Slider::draw() {
	window->draw(sprite);
	window->draw(sliderTrack);
	window->draw(sliderThumb);
	window->draw(text);
}