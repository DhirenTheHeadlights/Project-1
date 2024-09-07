#pragma once

/// Global font handler for pirate game

#include <SFML/Graphics.hpp>
#include <iostream>

namespace PirateGame {
	class GlobalFontHandler {
	public:
		GlobalFontHandler() {
			// Initialize the pointers
			timesNewRoman = std::make_unique<sf::Font>();
			pixelifySansRegular = std::make_unique<sf::Font>();
			pixelifySansMedium = std::make_unique<sf::Font>();
			pixelifySansSemiBold = std::make_unique<sf::Font>();
			pixelifySansBold = std::make_unique<sf::Font>();

			// Initialize the fonts
			if (!timesNewRoman->loadFromFile("Fonts/times_new_roman.ttf")) {
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

		// Getters
		sf::Font* getGlobalFont() const { return pixelifySansRegular.get(); }
		sf::Font* getTimesNewRoman() const { return timesNewRoman.get(); }
		sf::Font* getPixelifySansRegular() const { return pixelifySansRegular.get(); }
		sf::Font* getPixelifySansMedium() const { return pixelifySansMedium.get(); }
		sf::Font* getPixelifySansSemiBold() const { return pixelifySansSemiBold.get(); }
		sf::Font* getPixelifySansBold() const { return pixelifySansBold.get(); }

	private:
		// Fonts
		std::unique_ptr<sf::Font> timesNewRoman;
		std::unique_ptr<sf::Font> pixelifySansRegular;
		std::unique_ptr<sf::Font> pixelifySansMedium;
		std::unique_ptr<sf::Font> pixelifySansSemiBold;
		std::unique_ptr<sf::Font> pixelifySansBold;
	};
}