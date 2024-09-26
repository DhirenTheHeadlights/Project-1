#pragma once

/// This class is for the interactable type of button. It is used to create the
/// button and handle its interaction.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <optional>

#include "Interactable_PG.h"

namespace PirateGame {
	// Function for the button, does not need to return anything
	using ButtonFunction = std::function<void()>;

	class Button : public Interactable {
	public:
		Button(ButtonFunction func = []() {}) : func(func) {};
		~Button() {};

		void interact(sf::RenderWindow* window, GlobalSoundManager* GSM) override;
		void customInteractableSetUp() override {}; // Need it but does nothing for now
		void interactOnce(sf::RenderWindow* window, GlobalSoundManager* GSM) const;
		void interactHold(sf::RenderWindow* window, GlobalSoundManager* GSM);

		// This flag allows for multiple button presses if the button is held down
		void setHoldDown(bool hold) { holdDown = hold; }
	private:
		ButtonFunction func;

		bool holdDown = false;

		sf::Clock cooldown;
		sf::Time cooldownTime = sf::seconds(0.1f);
	};
}
