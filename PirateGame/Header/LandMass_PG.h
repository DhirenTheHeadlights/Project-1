#pragma once

/// <summary>
/// This class is used to represent the land masses in the game.
/// The types of land masses include: islands, rocks, and shipwrecks.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Textures_PG.h"

namespace PirateGame {
	// This enum will be used to determine the land mass type
	enum class LandMassType { Island, Rock, Shipwreck };

	class LandMass {
	public:
		void createLandMass(LandMassType type, Textures& texture);

		// Add a new item to the market
		void addItem(std::string name, int price);

		// Setters
		void setPosition(sf::Vector2f pos);

		// Getters
		std::vector<std::pair<std::string, int>> getMarket() { return market; }
		sf::RectangleShape getInteractablePoint() { return interactablePoint; }
		LandMassType getType() const { return type; }
		sf::Sprite& getSprite() { return sprite; }
		sf::Vector2f getSpritePos() { return sprite.getPosition(); }
		sf::Vector2f getSpriteSize() { return sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height); }

		// Draw the land mass
		void draw(sf::RenderWindow& window);

		//Select and assign sprites
		void selectIslandSprite(Textures& texture);
		void selectRockSprite(Textures& texture);
	private:
		// Sprite to represent the land mass
		sf::Sprite sprite;
		sf::Texture texture;
		float scaling = 1;

		// Variable to store the land mass's type
		LandMassType type = LandMassType::Island;

		// Temporary rectangle to represent interactable point
		sf::RectangleShape interactablePoint;

		// Variables to store the land mass's values
		std::vector<std::pair<std::string, int>> market;
	};
}