#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <functional>
#include <vector>
#include "Globals.h"
#include "World.h"
#include "Arena.h"

// Define a type alias for a function that takes no arguments and returns void
using GameFunction = std::function<void()>;

class Game { // A class to store game data
public:
    Game(const std::string& n, GameFunction func) : name(n), gameFunc(func) {} // Constructor

    std::string getName() const { return name; }
    void run() const { gameFunc(); } 

private:
    std::string name;
    GameFunction gameFunc;
};

class gameButton {
public:
    gameButton(Game& gamedata, sf::Font& font, float width = 300.f, float height = 100.f) :
        game(gamedata), button(sf::Vector2f(width, height)), gameName(gamedata.getName(), font, 24) {
        // Set the button text (game name)
        gameName.setFillColor(sf::Color::Red);
        // Set button size and color
        button.setFillColor(sf::Color::White);
        // Optional: set button text position here or let the user set it
    }

    void setPosition(float x, float y) {
        button.setPosition(x, y);
        gameName.setPosition(x + 10, y + (button.getSize().y - gameName.getCharacterSize()) / 2); // Center text vertically
    }

    void draw(sf::RenderWindow& window) { // Draw the button
        window.draw(button);
        window.draw(gameName);
    }

    bool isMouseOver(sf::RenderWindow& window) { // Check if the mouse is over the button
        auto mousePos = sf::Mouse::getPosition(window);
        auto translatedPos = window.mapPixelToCoords(mousePos);
        return button.getGlobalBounds().contains(translatedPos);
    }

    const Game& getGame() const { // Return the game data
        return game;
    }

private:
    sf::RectangleShape button;
    sf::Text gameName;
    Game game;
};

void agario() { // Agar.io
    initializeGlobals();
    World world(window);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
			}
        }
        window.clear();
        world.createWorld(window, event);
        window.display();
    }
}

void aimTrainer() { // Aim Trainer
    desktop = sf::VideoMode::getDesktopMode();  // Initialize the global desktop variable
    window.create(desktop, "RANGER WINDOW", sf::Style::Fullscreen);  // Initialize the global window variable
    window.setVerticalSyncEnabled(true);
    std::srand(std::time(0));// Initialize the random number generator

    Arena arena(window); // Create the arena

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        arena.createArena();
        window.display();
    }
}

void snake() { // Snake or some other game idk
	// TODO
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Selection Menu");
    window.setVerticalSyncEnabled(true);

    std::vector<Game> games = {
        Game("Agar.io", agario),
        Game("Aim Trainer", aimTrainer),
        Game("Snake", snake)
        // Add more games here...
    };

    std::vector<gameButton> buttons;
    sf::Font font;
    if (!font.loadFromFile("times_new_roman.ttf")) {
        std::cerr << "Could not load font" << std::endl;
        return -1;
    }

    float x = 100.f; // X position of the first button
    float y = 100.f; // Y position of the first button
    float yPadding = 150.f; // Space between buttons

    // Create buttons for each game
    for (auto& game : games) {
        gameButton button(game, font);
        button.setPosition(x, y);
        y += yPadding; // Update y position for the next button
        buttons.push_back(button);
    }

    // Game selection menu loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

        // Check if any button is clicked
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            for (auto& button : buttons) {
                if (button.isMouseOver(window)) {
                    window.close(); // Close the menu window before launching the game
                    button.getGame().run(); // Run the selected game, which will create its own window
                    window.create(sf::VideoMode(800, 600), "Game Selection Menu"); // Recreate the menu window if needed after the game exits
                }
            }
        }

        window.clear();

        // Draw buttons
        for (auto& button : buttons) {
            button.draw(window);
        }

        window.display();
    }

    return 0;
}