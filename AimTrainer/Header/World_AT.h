#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <algorithm>

#include "GameTemplate.h"
#include "Target_AT.h"
#include "AimTrainerHashmap_AT.h"
#include "Crosshair_AT.h"
#include "MenuHandler_AT.h"
#include "SaveData_AT.h"
#include "GameCallBacks_AT.h"
#include "GameStateManager_AT.h"

/// <summary>
/// This is the main class for the game, it is used to create the arena and to
/// handle the different game states. It is also used to create the different
/// menus and to handle the different game interactions.
/// </summary>

namespace AimTrainer {
    class World : public GameStateManager {
    public:
        // Constructor
        World(sf::RenderWindow& window);

        // Main method for arena creation and interaction
        void createWorld(sf::Event event);

        // Inherited methods from GameStateManager and their implementations
        void changeGameState(GameState newState) override;
        GameState getCurrentGameState() const override;

        // Inherited methods for game interactions
        void startGame() override;
        void openLeaderBoardMenu() override;
        void openOptionsMenu() override;
        void openStartMenu() override;
        void openEndScreen() override;
        void openInsertNameMenu() override;
        void setName(std::string name) override;
        void exit() override;

    private:
        // Smart pointer to the menu handler
        std::unique_ptr<MenuHandler> menuHandler;

        // Variable to store the current game state
        GameState currentGameState = GameState::TitleScreen;

        // SFML objects and game-related variables
        sf::RenderWindow& window;
        std::vector<Target*> activeTargets;
        sf::VideoMode desktop;
        sf::Clock mainClock;
        sf::Font font;
        sf::RectangleShape arenaBox;
        sf::RectangleShape backgroundRect;
        std::vector<std::pair<std::string, int>> scores;

        // Game parameters
        int timerTime = 10; // Time for game in seconds
        int score = 0;
        float menuToggleCooldown = 0.5f;
        float buttonPressCooldown = 0.5f;
        float size = 10;
        float maxTargets = 10;
        std::string name = "Anonymous";

        // Boolean flags for game state and inputs
        bool wasButtonPressedLastFrame = false;
        bool mousePressed = false;
        bool targetHitOnCurrentClick = false;
        bool wasTabPressedLastFrame = false;

        // Game functions for logic and display
        void updateTargets();
        void handleTargetCollision(sf::Event event);
        void showTimer();
        void gameLoop(sf::Event event);
        void endGame();

        // Menu functions for different screens
        void handleTitleScreen(sf::Event event);
        void handleLeaderBoardScreen(sf::Event event);
        void handleOptionsMenu(sf::Event event);
        void handleEndScreen(sf::Event event);
        void handleInsertNameMenu(sf::Event event);

        // Game classes and utilities
        Hashmap_AT hashmap;
        Crosshair crosshair;
        SaveData leaderboardData;

        // Function to create targets
        Target* createTarget();
    };
}