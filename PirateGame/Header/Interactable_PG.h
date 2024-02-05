#pragma once

/// <summary>
///  This class is the base class for all interactable objects in the game.
///	 By default, it will have a background, a foreground, and a frame.
///  Therefore, it can be used to make a text display box at base.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

namespace PirateGame {
	class Interactable {
	public:
		Interactable(sf::Font& font) : font(font) {};
		~Interactable() {};

		virtual void setUpInteractable(sf::Vector2f size);
		virtual void interact(sf::RenderWindow& window) = 0;
		virtual void draw(sf::RenderWindow& window);
		void updateColors();

		// Setters for properties
		virtual void setPosition(sf::Vector2f pos);
		void setSize(sf::Vector2f size) { this->size = size; }
		void setScale(sf::Vector2f scale) { this->scale = scale; }
		void setTexture(const std::string& textureFile) { if (!texture.loadFromFile(textureFile)) std::cout << "Error Loading File"; };
		void setString(const std::string& string) { this->name = string, text.setString(string); }

		void setTextPosition(sf::Vector2f pos) { text.setPosition(pos); }

		// Setters for colors
		void setBackgroundColor(sf::Color color) { this->backgroundColor = color; }
		void setForegroundColor(sf::Color color) { this->foregroundColor = color; }
		void setFrameColor(sf::Color color) { this->frameColor = color; }
		void setTextColor(sf::Color color) { this->textColor = color; }
		void setOutlineThickness(float thickness) { this->outlineThickness = thickness; }

	protected:
		// Variables
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Vector2f scale;
		std::string name;

		float padding = 10.0f;
		float outlineThickness = 5.f;

		// Objects
		sf::Sprite sprite;
		sf::Texture texture;
		sf::RectangleShape background;
		sf::RectangleShape foreground;
		sf::RectangleShape frame;
		sf::Font& font;
		sf::Text text;

		// Default colors
		sf::Color backgroundColor = sf::Color(100, 100, 150, 200);
		sf::Color foregroundColor = sf::Color(255, 255, 255, 200);
		sf::Color frameColor = sf::Color(150, 150, 200, 200);
		sf::Color textColor = sf::Color(0, 0, 0, 200);
	};
}

