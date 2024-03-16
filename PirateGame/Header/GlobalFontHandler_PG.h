#pragma once

/// Global font handler for pirate game

#include <SFML/Graphics.hpp>
#include <iostream>

namespace PirateGame {
	class GlobalFontHandler {
	public:
		// Public method to access the instance
		static GlobalFontHandler& getInstance() {
			static GlobalFontHandler instance;
			return instance;
		}

		// Getters
		sf::Font* getGlobalFont() { return pixelifySansRegular.get(); }
		sf::Font* getTimesNewRoman() { return timesNewRoman.get(); }
		sf::Font* getPixelifySansRegular() { return pixelifySansRegular.get(); }
		sf::Font* getPixelifySansMedium() { return pixelifySansMedium.get(); }
		sf::Font* getPixelifySansSemiBold() { return pixelifySansSemiBold.get(); }
		sf::Font* getPixelifySansBold() { return pixelifySansBold.get(); }

	private:
		// Private Constructor
		GlobalFontHandler() {
			// Initalize the pointers
			timesNewRoman = std::make_unique<sf::Font>();
			pixelifySansRegular = std::make_unique<sf::Font>();
			pixelifySansMedium = std::make_unique<sf::Font>();
			pixelifySansSemiBold = std::make_unique<sf::Font>();
			pixelifySansBold = std::make_unique<sf::Font>();

			// Initalize the fonts
			if (!timesNewRoman->loadFromFile("Fonts/times-new-roman.ttf")) {
				std::cout << "Error loading Times New Roman font" << std::endl;
			}
			if (!pixelifySansRegular->loadFromFile("Fonts/PixelifySans-Regular.ttf")) {
				std::cout << "Error loading Pixelify Sans Regular font" << std::endl;
			}
			if (!pixelifySansMedium->loadFromFile("Fonts/PixelifySans-Medium.ttf")) {
				std::cout << "Error loading Pixelify Sans Medium font" << std::endl;
			}
			if (!pixelifySansSemiBold->loadFromFile("Fonts/PixelifySans-SemiBold.ttf")) {
				std::cout << "Error loading Pixelify Sans SemiBold font" << std::endl;
			}
			if (!pixelifySansBold->loadFromFile("Fonts/PixelifySans-Bold.ttf")) {
				std::cout << "Error loading Pixelify Sans Bold font" << std::endl;
			}
		};

		// Delete the copy constructor and assignment operator
		GlobalFontHandler(GlobalFontHandler const&) = delete;
		GlobalFontHandler& operator=(GlobalFontHandler const&) = delete;

		// Fonts
		std::unique_ptr<sf::Font> timesNewRoman;
		std::unique_ptr<sf::Font> pixelifySansRegular;
		std::unique_ptr<sf::Font> pixelifySansMedium;
		std::unique_ptr<sf::Font> pixelifySansSemiBold;
		std::unique_ptr<sf::Font> pixelifySansBold;
	};
}