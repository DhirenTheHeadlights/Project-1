#include "Globals.h"

Map map; // Global Map
sf::VideoMode desktop; // Globals for Window
sf::RenderWindow window;

std::vector<sf::Color> colors = {
sf::Color::Red,
sf::Color::Green,
sf::Color::Blue,
sf::Color::Yellow,
sf::Color::Cyan,
sf::Color::Magenta,
sf::Color::White };

void initializeGlobals() {
    desktop = sf::VideoMode::getDesktopMode();  // Initialize the global desktop variable
    window.create(desktop, "RANGER WINDOW", sf::Style::Fullscreen);  // Initialize the global window variable
}