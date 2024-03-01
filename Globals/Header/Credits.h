#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

// Fun little class to credit my hard working slaves

class Credits {
public:
	Credits(sf::Font& font) {
		credits.setFont(font);
		credits.setCharacterSize(20);
		credits.setFillColor(sf::Color::White);
		credits.setString(readFromCreditsFile());
		credits.setPosition(100, 100);

		std::cout << credits.getString().toAnsiString() << std::endl;
	}

	~Credits() {}

	void run(sf::RenderWindow& window) {
		if (clock.getElapsedTime().asSeconds() > 0.1) {
			credits.move(0, -0.5);
			clock.restart();
		}
		window.draw(credits);
	}
private:
	sf::Text credits;
	sf::Clock clock;

	std::string readFromCreditsFile() {
		std::string credits;
		std::ifstream file("Globals/Misc/Credits.txt");
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				credits += line + "\n";
			}
			file.close();
		}
		return credits;
	}
};
