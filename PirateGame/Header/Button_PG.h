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
		Button(ButtonFunction func) : func(func) {};
		~Button() {};

		void interact() override; // Overridden function to handle the interaction of the button
		void interactOnce();
		void interactHold();

		void setCooldownTime(sf::Time time) { cooldownTime = time; }
		void setHoldDown(bool hold) { holdDown = hold; }
	private:
		ButtonFunction func;
		sf::Clock cooldown;
		sf::Time cooldownTime = sf::milliseconds(100);

		bool holdDown = false;
	};
}
