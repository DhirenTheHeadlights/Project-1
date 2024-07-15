#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <functional>
#include <vector>


// Include all of the game headers
#include "Globals/Header/Globals.h"
#include "AgarGame/Header/World_AG.h"
#include "AimTrainer/Header/World_AT.h"
#include "LightAndShadow/Header/WorldLS.h"
#include "PirateGame/Header/WorldFactory_PG.h"
#include "Platformer Game/Header/World_Plat.h"
#include "DoodleJump/World_DJ.h"
#include "Asteroids/Header/World_AR.h"

#include "Credits.h"

// Define a type alias for a function that takes no arguments and returns void
using GameFunction = std::function<void()>;

bool exitLauncher = false; 

class Game { // A class to store game data
public:
    Game(const std::string& n, GameFunction func) : name(n), gameFunc(func) {} // Constructor

    std::string getName() const { return name; }
    void run() const {
        gameFunc();
    }  

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
    }

    void setPosition(float x, float y) {
        button.setPosition(x, y);
        gameName.setPosition(x + 10, y + (button.getSize().y - gameName.getCharacterSize()) / 2); // Center text vertically
    }

    void draw(sf::RenderWindow& window) const { // Draw the button
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
    initializeGlobals("Agar.io Window");
    AgarGame::World world(window);
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
        world.createWorld(event);
        window.display();
    }
}

void aimTrainer() { // Aim Trainer
    initializeGlobals("Aim Trainer Window");

    AimTrainer::World world(window); // Create the arena

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Create the arena; where all of the action happens
            world.createWorld(event);
        }
    }
}

void pirateGame() { // Pirate Game
    initializeGlobals("Pirate Game Window");
	
    PirateGame::WorldFactory worldFactory;
    PirateGame::World* world = nullptr;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (!worldFactory.getHasSelectedWorld()) {
            world = worldFactory.selectWorld(&window);
        }
        else {
            world->createWorld(event);
        }
    }
}

void lightAndShadow() {
    initializeGlobals("Light and Shadow Game Window");

    LightAndShadow::World world(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { // Close the window if the user clicks the X button
            	window.close();
            }
		}
		window.clear();
		world.createWorld(event);
		window.display();
    }
}

void platformer() {
    initializeGlobals("Platformer Window");

    PlatformerGame::World world(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Close the window if the user clicks the X button
                window.close();
            }
        }
        window.clear();
        world.gameLoop(event);
        window.display();
    }
}

void credits() {
    initializeGlobals("Credits Window");
    sf::Font font;

    if (!font.loadFromFile("Fonts/times_new_roman.ttf")) {
		std::cerr << "Could not load font" << std::endl;
		return;
	}

    Credits credits(font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        credits.run(window);
    }
}

void doodleJump() {
	initializeGlobals("Doodle Jump Window");

	DoodleJump::World world(window);

    while (window.isOpen()) {
		sf::Event event;
        while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
            	window.close();
            }
		}
		window.clear();
		world.run();
		window.display();
	}
}

void asteroids() {
    initializeGlobals("Platformer Window");

    Asteriods::World world(&window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Close the window if the user clicks the X button
                window.close();
            }
        }
        window.clear();
        world.run(event);
        window.display();
    }
}

void exitTheLauncher() {
    exitLauncher = true;
	window.close();
}

int main() {
    std::string name = "Game Selection Window";
    initializeGlobals(name);

    window.setVerticalSyncEnabled(true);

    std::vector<Game> games = {
        Game("Agar.io", agario),
        Game("Aim Trainer", aimTrainer),
        Game("Light and Shadows", lightAndShadow),
        Game("Pirate Game", pirateGame),
        Game("Platformer", platformer),
        Game("Doodle Jump", doodleJump),
        Game("Asteroids", asteroids),
        // Add more games here...

        Game("Credits", credits),

        Game("Exit", exitTheLauncher),
    };

    std::vector<gameButton> buttons;
    sf::Font font;
    if (!font.loadFromFile("Fonts/times_new_roman.ttf")) {
        std::cerr << "Could not load font" << std::endl;
        return -1;
    }

    float x = 100.f;                // X position of the first button
    float y = 100.f;                // Y position of the first button
    float yPadding = 150.f;         // Space between buttons
    float xPadding = 400.f;         // Space between columns
    float numButtonsInColumn = 6.f; // Number of buttons in a column

    // Create buttons for each game
    for (auto& game : games) {
        // if there are too many buttons, move to the next column
        if (y > yPadding * numButtonsInColumn) {
			y = 100.f; // Reset y position
			x += xPadding;
		}

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

        window.clear();

        // Draw buttons
        for (auto& button : buttons) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && button.isMouseOver(window)) {
                window.close();          // Close the menu window before launching the game
                button.getGame().run();  // Run the selected game, which will create its own window
                if (!exitLauncher) initializeGlobals(name); // Recreate the menu window if needed after the game exits
			}
            button.draw(window);
        }

        window.display();
    }

    return 0;
}