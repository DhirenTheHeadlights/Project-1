#include "Slider_PG.h"

using namespace PirateGame;

// Set up the slider
void Slider::setUpInteractable(sf::Vector2f size) {
	// Set the size of the interactable objects
	background.setSize(size);
	foreground.setSize(sf::Vector2f(size.x - padding * 2, size.y / 2 - padding * 2));
	frame.setSize(size);
	sliderThumb.setSize(sf::Vector2f(5.f, 20.f));

	// Set the color of the slider thumb
	sliderThumb.setFillColor(foregroundColor);

	// Set the text to be under the slider thumb
	text.setCharacterSize(size.y / 2);
	text.setFont(font);
	text.setFillColor(textColor);

	// Set the color of the interactable objects
	updateColors();
}

// Set the position of the slider
void Slider::setPosition(sf::Vector2f pos) {
	// Set the position of the interactable objects
	background.setPosition(pos);
	frame.setPosition(pos);
	// Set the position of the slider track to be in the center of the background
	foreground.setPosition(sf::Vector2f(pos.x + padding, pos.y + 2 * padding));
	// Set the text to be in the center of the foreground
	float x = foreground.getPosition().x + foreground.getSize().x / 2 - text.getGlobalBounds().width / 2;
	float y = foreground.getPosition().y + foreground.getSize().y / 2 - text.getGlobalBounds().height / 2;
	text.setPosition(sf::Vector2f(x, y));
	// Set the position of the slider thumb to be in the center of the slider track
	sliderThumb.setPosition(foreground.getPosition().x, foreground.getPosition().y + foreground.getSize().y / 2 - sliderThumb.getSize().y / 2);
}

// Interact with the slider
void Slider::interact(sf::RenderWindow& window) {
	// If the slider is being clicked
	if (foreground.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// Set the position of the slider thumb to the mouse position
		sliderThumb.setPosition(sf::Mouse::getPosition(window).x, sliderThumb.getPosition().y);
		// If the slider thumb is outside the bounds of the slider track, set it to the bounds
		if (sliderThumb.getPosition().x < foreground.getPosition().x) sliderThumb.setPosition(foreground.getPosition().x, sliderThumb.getPosition().y);
		if (sliderThumb.getPosition().x > foreground.getPosition().x + foreground.getSize().x - sliderThumb.getSize().x) sliderThumb.setPosition(foreground.getPosition().x + foreground.getSize().x - sliderThumb.getSize().x, sliderThumb.getPosition().y);
	}
	// Set the text to display the value of the slider
	text.setString(name + ":  " + std::to_string(static_cast<int>((sliderThumb.getPosition().x - foreground.getPosition().x) / (foreground.getSize().x - sliderThumb.getSize().x) * 100)));
}

// Draw the slider
void Slider::draw(sf::RenderWindow& window) {
	window.draw(background);
	window.draw(foreground);
	window.draw(frame);
	window.draw(sliderThumb);
	window.draw(text);
}