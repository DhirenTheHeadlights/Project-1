#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <optional>
#include <variant>

#include "PlayerInputBox_AT.h"

/// <summary>
/// This class is for the interactables, it is used to create the different
/// interactables and to handle their interactions. This class contains the
/// different types of interactables and their properties. It also contains
/// the different functions for handling the interactions. The 4 different
/// types of interactables are: Button, Slider, InfoBox and InputBox.
/// </summary>

// Function type for interactable actions: either takes a float argument or no arguments
using InteractableFunctionVariant = std::variant<std::function<void(float value)>, std::function<void()>>;

// Optional function type for interactables, can be empty
using InteractableFunction = std::optional<InteractableFunctionVariant>;

class Interactable {
public:
    // Enum representing the different types of interactables
    enum class Type {
        Button,
        Slider,
        InfoBox,
        InputBox
    };

    // Constructor
    Interactable(InteractableFunction func, Type type);

    // Methods for setting up and interacting with the interactable
    void createInteractable(std::string name, float length, float width, float textSize);
    void setPos(sf::Vector2f position);
    void interact(sf::RenderWindow& window, sf::Clock& cooldown);
    void draw(sf::RenderWindow& window, sf::Event event);

    // Color setting functions
    void setTextColor(sf::Color color);
    void setButtonColor(sf::Color color);
    void setSliderColor(sf::Color color);
    void setSliderComplementColor(sf::Color color);
    void setHoverColor(sf::Color color);

    // Setters for interactable properties
    void setInteractableSize(sf::Vector2f size);
    void setInteractableTextSize(float size);

    // Getters for interactable properties
    Interactable::Type getInteractableType() const;
    sf::Vector2f getSize() const;
    sf::Vector2f getPos() const;
    PlayerInputBox& getSliderInputBox();
    PlayerInputBox& getInputBox();
    std::string getInputString();

private:
    // SFML shapes for representing the interactable
    sf::RectangleShape rectHorizontal; // Main rectangle shape for interactable
    sf::RectangleShape rectVertical;   // Vertical part of the slider
    sf::RectangleShape backgroundRect; // Background rectangle
    PlayerInputBox inputBox;           // Input box for the input box interactable

    // SFML font and text for interactable label
    sf::Font font;
    sf::Text text;

    // Positioning and dimensions
    sf::Vector2f defaultPos;      // Default position of interactable
    sf::Vector2f rectPosVertical; // Position of the vertical rectangle (slider)

    // Interactable properties and actions
    InteractableFunction interactableFunc;  // Function to call on interaction
    Type interactableType;                  // Type of the interactable (Button, Slider, InfoBox)
    PlayerInputBox sliderInputBox;          // Input box for the sliders only. This is used to be more precise with the slider movement.

    // Dimensions and offset
    float length = 1;             // Length of the interactable
    float width = 1;              // Width of the interactable
    float textSize = 1;           // Text size for interactable label
    float offset = 0;             // Offset used for slider movement
    float inputBoxOffset = 10.0f; // Offset used for the input box

    // Colors for the interactable
    sf::Color textColor;             // Color of the text
    sf::Color buttonColor;           // Color of the button
    sf::Color sliderColor;           // Color of the slider
    sf::Color sliderColorComplement; // Complementary color of the slider
    sf::Color hoverColor;            // Color of the interactable when hovered over

    // Private methods for handling interactions
    void buttonPress(sf::RenderWindow& window, sf::Clock& cooldown);
    bool isMouseOverButton(sf::RenderWindow& window) const;
    void callButtonAction() const;
    void sliderDrag(sf::RenderWindow& window);
    void updateSliderPositionVariable(const sf::Vector2f& mousePos);
    void callSliderAction();
};
