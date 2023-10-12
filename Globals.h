#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include <string>

extern sf::VideoMode desktop;
extern sf::RenderWindow window;
extern std::vector<sf::Color> colors;

void initializeGlobals();
