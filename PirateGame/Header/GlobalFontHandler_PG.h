#pragma once

/// Global font handler for pirate game

#include <SFML/Graphics.hpp>
#include <iostream>

namespace PirateGame {
	namespace Font {
		// Fonts
		inline sf::Font timesNewRoman;
		inline sf::Font pixelifySansRegular;
		inline sf::Font pixelifySansMedium;
		inline sf::Font pixelifySansSemiBold;
		inline sf::Font pixelifySansBold;

		inline void setUpFonts() {
			// Initialize the fonts
			if (!timesNewRoman.loadFromFile("Fonts/times_new_roman.ttf")) {
				std::cout << "Error loading Times New Roman font" << std::endl;
			}
			if (!pixelifySansRegular.loadFromFile("Fonts/PixelifySans-Regular.ttf")) {
				std::cout << "Error loading Pixelify Sans Regular font" << std::endl;
			}
			if (!pixelifySansMedium.loadFromFile("Fonts/PixelifySans-Medium.ttf")) {
				std::cout << "Error loading Pixelify Sans Medium font" << std::endl;
			}
			if (!pixelifySansSemiBold.loadFromFile("Fonts/PixelifySans-SemiBold.ttf")) {
				std::cout << "Error loading Pixelify Sans SemiBold font" << std::endl;
			}
			if (!pixelifySansBold.loadFromFile("Fonts/PixelifySans-Bold.ttf")) {
				std::cout << "Error loading Pixelify Sans Bold font" << std::endl;
			}
		};
	};
}