#pragma once

/// <summary>
/// This class is the for the slider interactable object. It will have
/// a background, a foreground, and a frame. It will also have a slider
/// track and a slider thumb. There is a text box to display the value
/// of the slider for precision.
/// </summary>

#include "Interactable_PG.h"

// Function for the slider, returns nothing but takes in a float
using SliderFunction = std::function<void(float value)>;

namespace PirateGame {
	class Slider : public Interactable {
	public:
		Slider(SliderFunction func, sf::Font& font) : func(func), Interactable(font) {};
		~Slider() {};

		void setUpInteractable(sf::Vector2f size) override;
		void interact(sf::RenderWindow& window) override;
		void draw(sf::RenderWindow& window) override;
		void setPosition(sf::Vector2f pos) override;	

		// Setters for properties
		void setSliderThumbSize(sf::Vector2f size) { sliderThumb.setSize(size); }
		void setSliderThumbColor(sf::Color color) { sliderThumb.setFillColor(color); }
		void setSliderThumbPosition(sf::Vector2f pos) { sliderThumb.setPosition(pos); }

	private:
		SliderFunction func;

		sf::RectangleShape sliderThumb;
	};
}

