#pragma once

/// This class is the for the slider interactable object. It will have
/// a background, a foreground, and a frame. It will also have a slider
/// track and a slider thumb. There is a text box to display the value
/// of the slider for precision.

#include "Interactable_PG.h"

// Function for the slider, returns nothing but takes in a float
using SliderFunction = std::function<void(float value)>;

namespace PirateGame {
	class Slider : public Interactable {
	public:
		Slider(sf::Texture track, sf::Texture thumb, SliderFunction func) : func(func) {
			// For now, we are going to load default thumb and track textures
			// We will need to change this to be more flexible
			sliderTrack.setTexture(track);
			sliderThumb.setTexture(thumb);
			sliderThumb.setScale(4.f, 4.f);
		};
		~Slider() {};

		void customInteractableSetUp() override;
		void interact(sf::RenderWindow* window, GlobalInputHandler* GIH, GlobalSoundManager* GSM) override;
		void draw(sf::RenderWindow* window) override;
		void setPosition(sf::Vector2f pos) override;

	private:
		SliderFunction func;

		float value = 0.5f; // Default value
		bool isGrabbing = false;

		std::string name;

		sf::Sprite sliderTrack;
		sf::Sprite sliderThumb;
	};
}

