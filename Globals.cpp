#include "Globals.h"

Map map;
sf::VideoMode desktop;
sf::RenderWindow window;

void initializeGlobals() {
    map.grid(10000, 10000, 10);  // Initialize the map
    desktop = sf::VideoMode::getDesktopMode();  // Initialize the global desktop variable
    window.create(desktop, "RANGER WINDOW", sf::Style::Fullscreen);  // Initialize the global window variable
}