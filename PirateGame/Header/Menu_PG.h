#pragma once

/// The base class for all of the game menus.
/// It is intended to be inherited from and extended to create new menus.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>

#include "GlobalContext_PG.h"
#include "Font_PG.h"
#include "Button_PG.h"
#include "Slider_PG.h"
#include "TextDisplayBox_PG.h"
#include "TextInpuBox_PG.h"
#include "DropDown_PG.h"
#include "Globals_PG.h"

namespace PirateGame {
	class Menu {
	public:
		Menu(GlobalContext& context) : context(context), font(Font::globalFont), window(Globals::window) {}

		~Menu() {}

		virtual void setUpMenu() {}
		virtual void draw();
		virtual void update() {}
		virtual void addInteractablesToMenu() {}
		virtual void setInteractablePositions() {}
		virtual void interactWithMenuItems() {}

	private:
		// Menu variables
		bool isMenuOpen = false;
		bool redraw = false;
		bool positionUpdated = false;
		sf::Time totalPauseDuration;

	protected:
		// Context
		GlobalContext& context;

		// SFML objects
		sf::Font& font;
		sf::RenderWindow* window = nullptr;

		// Menu items
		sf::Sprite background;
		sf::Sprite menu;

		// Menu Text
		std::string title = "Title not set";
		sf::Text titleText;

		// Menu properties
		sf::Vector2f backgroundScale = { 1, 1 };
		sf::Vector2f menuScale = { 1, 1 };

		sf::Color textColor = sf::Color(0, 0, 0, 200); // Black

		unsigned textSize = 20u;
		unsigned interactableTextSize = 10u;
		float padding = 10.0f;


		// Helper function to convert a float to a string with a set number of decimal places
		std::string floatToString(float number, int decimalPlaces = 2) {
			std::stringstream stream;
			stream << std::fixed << std::setprecision(decimalPlaces) << number;
			return stream.str();
		}

		// General function to add a button to a vector of buttons
		virtual void addButton(sf::Text text, sf::Texture& texture, std::vector<Button>& destination, std::function<void()> function, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			Button button(function);
			button.createInteractable(texture, text, scale);
			destination.push_back(button);
		}

		// General function to add a text display box to a vector of text display boxes
		virtual void addTextDisplayBox(sf::Text text, sf::Texture& texture, std::vector<TextDisplayBox>& destination, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			TextDisplayBox textDisplayBox;
			textDisplayBox.createInteractable(texture, text, scale);
			destination.push_back(textDisplayBox);
		}

		// General function to add a slider to a vector of sliders
		virtual void addSlider(sf::Text text, sf::Texture& texture, std::vector<Slider>& destination, std::function<void(float value)> function, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			Slider slider(Textures::interactableTextures.getScrollBarTrack(), Textures::interactableTextures.getScrollBarThumb(), function);
			slider.createInteractable(texture, text, scale);
			destination.push_back(slider);
		}

		// General function to add a drop down to a vector of drop downs
		virtual void addDropDown(sf::Text text, sf::Texture& texture, std::vector<DropDown>& destination, std::vector<std::pair<std::function<void()>, std::string>> options, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			DropDown dropDown(options);
			dropDown.createInteractable(texture, text, scale);
			destination.push_back(dropDown);
		}

		/// Some menus might require storage of interactables in the same vector, necessitating the use of unique pointers

		// General function to add a button to a vector of interactables
		virtual void addButton(sf::Text text, sf::Texture& texture, std::vector<std::unique_ptr<Interactable>>& destination, std::function<void()> function, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			std::unique_ptr<Button> button = std::make_unique<Button>(function);
			button->createInteractable(texture, text, scale);
			destination.push_back(std::move(button));
		}

		// General function to add a text display box to a vector of interactables
		virtual void addTextDisplayBox(sf::Text text, sf::Texture& texture, std::vector<std::unique_ptr<Interactable>>& destination, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			std::unique_ptr<TextDisplayBox> textDisplayBox = std::make_unique<TextDisplayBox>();
			textDisplayBox->createInteractable(texture, text, scale);
			destination.push_back(std::move(textDisplayBox));
		}

		// General function to add a slider to a vector of interactables
		virtual void addSlider(sf::Text text, sf::Texture& texture, std::vector<std::unique_ptr<Interactable>>& destination, std::function<void(float value)> function, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			std::unique_ptr<Slider> slider = std::make_unique<Slider>(Textures::interactableTextures.getScrollBarTrack(), Textures::interactableTextures.getScrollBarThumb(), function);
			slider->createInteractable(texture, text, scale);
			destination.push_back(std::move(slider));
		}

		// General function to add a drop down to a vector of interactables
		virtual void addDropDown(sf::Text text, sf::Texture& texture, std::vector<std::unique_ptr<Interactable>>& destination, std::vector<std::pair<std::function<void()>, std::string>> options, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			std::unique_ptr<DropDown> dropDown = std::make_unique<DropDown>(options);
			dropDown->createInteractable(texture, text, scale);
			destination.push_back(std::move(dropDown));
		}
	};
}
