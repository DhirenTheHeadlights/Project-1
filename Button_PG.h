#pragma once

/// <summary>
/// This class is for the interactable type of button. It is used to create the
/// button and handle its interaction.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <optional>

#include "Interactable_PG.h"

// Function for the button, does not need to return anything
using InteractableFunction = std::function<void()>;

class Button : public Interactable {
public:
	Button(InteractableFunction& func, sf::Font& font) : func(func), Interactable(font) {};
	~Button() {};

	void interact(sf::RenderWindow& window) override;

private:
	InteractableFunction& func;
	sf::Clock cooldown;
};

