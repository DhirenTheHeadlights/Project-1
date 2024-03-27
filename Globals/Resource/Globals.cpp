#include "Globals.h"

sf::VideoMode desktop; // Globals for Window
sf::RenderWindow window;

void initializeGlobals(std::string name) {
    desktop = sf::VideoMode::getDesktopMode();           // Initialize the global desktop variable
    window.create(desktop, name, sf::Style::Default);    // Initialize the global window variable
    window.setVerticalSyncEnabled(false);
    std::srand(static_cast<unsigned int>(std::time(0))); // Initialize the random number generator
}