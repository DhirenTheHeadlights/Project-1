#include "Globals.h"

sf::VideoMode desktop; // Globals for Window
sf::RenderWindow window;

void initializeGlobals() {
    desktop = sf::VideoMode::getDesktopMode();  // Initialize the global desktop variable
    window.create(desktop, "RANGER WINDOW", sf::Style::Default);  // Initialize the global window variable
    window.setVerticalSyncEnabled(true);
    std::srand(std::time(0));// Initialize the random number generator
}