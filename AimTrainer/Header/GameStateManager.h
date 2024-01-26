#pragma once
#include <iostream>

// GameState defnition
enum class GameState {
	TitleScreen,
	OptionsMenu,
	LeaderBoardScreen,
	EndScreen,
	GameLoop,
	Paused,
	InsertNameMenu
};

/// <summary>
/// This class is for the game state manager, it is used to change the game state
/// and to open the different menus. It is used to make the code more readable and
/// to make it easier to change the game state. This class is passed to the different
/// menus so that they can change the game state.
/// </summary>

class GameStateManager {
public:
	virtual ~GameStateManager() = default;

	// Interface methods for changing game state
	virtual void changeGameState(GameState newState) = 0;
	virtual GameState getCurrentGameState() const = 0;

	virtual void startGame() = 0;

	// Interface methods for opening menus
	virtual void openLeaderBoardMenu() = 0;
	virtual void openOptionsMenu() = 0;
	virtual void openStartMenu() = 0;
	virtual void openEndScreen() = 0;
	virtual void openInsertNameMenu() = 0;

	// Interface methods for game interactions
	virtual void setName(std::string name) = 0;
	virtual void exit() = 0;

private:
	// Nothing here
};