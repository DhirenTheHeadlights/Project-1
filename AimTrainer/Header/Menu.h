#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "GameStateManager.h"
#include "Interactables.h"
#include "Crosshair.h"
#include "GameCallBacks.h"

class Menu {
public:
    // Constructor
    Menu(sf::RenderWindow& window, sf::Font font);

    // Public methods for menu interaction and display
    void openMenu(sf::Event& eventFromMain, Crosshair& xhair, sf::Clock& mainClock);
    void resetRedrawFlag();
    void drawMenu(sf::Event event);
    void toggleMenu(sf::Clock& mainClock, Crosshair& xhair);

    // Setters for menu properties
    void addInteractable(std::string name, Interactable::Type type, std::optional<std::function<void(float value)>> func);
    void setTotalPauseTime(sf::Time time);
    void setTitleString(std::string titleString);
    void setMenuPosition(sf::Vector2f position);
    void setTextSize(float size);   

    // Setters for menu colors
    void setBackgroundColor(sf::Color color);
    void setTextColor(sf::Color color);
    void setButtonColor(sf::Color color);
    void setSliderColor(sf::Color color);
    void setSliderComplementColor(sf::Color color);
    void setHoverColor(sf::Color color);
    void updateInteractableColors();
    void setButtonSize(sf::Vector2f size);
    void setMaxInteractables(int max);

    // Getters for menu state and properties
    bool getIsMenuOpen() const;
    bool needsRedraw() const;
    sf::Time getTotalTimeInMenu() const;
    std::string getInputStringForInteractableI(int i) const;
    Interactable* getInteractableI(int i) const;

private:
    // Internal methods for menu functionality
    void setInteractablePositions();
    void interactWithMenuItems();
    void handlePauseTime(sf::Clock& mainClock, Crosshair& xhair);
    float calculateMenuHeight();

    // Vector to store interactable elements (buttons, sliders, etc.)
    std::vector<Interactable*> interactables;

    // SFML objects and menu-related variables
    sf::RenderWindow& window;             // Reference to the main SFML window
    sf::RectangleShape backgroundRect;    // Rectangle shape for the menu background
    sf::RectangleShape menu;              // Rectangle shape for the menu
    sf::Font font;                        // Font for menu text
    sf::Text title;                       // Text for the menu title
    static sf::Clock buttonPressClock;    // Clock for managing button press cooldowns
    sf::Time pauseTimeStart;              // Time when the menu was last paused
    sf::Time totalPauseDuration;          // Total duration the menu has been paused

    // Colors for the menu
    sf::Color backgroundColor;       // Color of the menu background
    sf::Color buttonColor;           // Color of the interactables
    sf::Color sliderColor;           // Complementary color of the interactables
    sf::Color sliderColorComplement; // Color of the interactables when hovered over
    sf::Color textColor;             // Color of the text
    sf::Color hoverColor;            // Color of the interactable when hovered over

    // Boolean flags and menu parameters
    bool isMenuOpen = false;               // Flag to check if the menu is open
    bool positionUpdated = false;          // Flag to check if the position has been updated
    bool wasButtonPressedLastFrame = false;// Flag to check if a button was pressed in the last frame
    bool redraw = true;                    // Flag to indicate if the menu needs to be redrawn

    // Constants for menu layout and appearance
    float buttonPressCooldown = 0.01f; // Cooldown time for button presses
    float buttonLen = 200.f;             // Length of buttons
    float buttonWid = 100.f;             // Width of buttons
    float length = buttonLen + 20.f;     // Total length of the menu
    float width = buttonWid + 20.f;      // Total width of the menu
    float textSize = 30.f;               // Size of the text
};
