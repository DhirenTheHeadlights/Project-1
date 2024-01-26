#pragma once
/// <summary>
/// This is the abstract class for the game template.
/// </summary>

#include <SFML/Graphics.hpp>

class GameTemplate {
public:
    virtual void createWorld(sf::Event& event) = 0;
    virtual ~GameTemplate() = default;
};

