#pragma once

/// <summary>
/// This class contains the game state manager used
/// for controlling which things appear on the screen
/// </summary>

// Enum class for game states
enum class GameState_PG {
	Start,
	GameLoop,
	LandMassInteraction,
	OptionsMenu,
	End
};

class GameStateManager_PG {
public:
	// Change the GameState
	void changeGameState(GameState_PG gameState) { this->currGameState; }

	// Grab the GameState
	GameState_PG getCurrentGameState() { return currGameState; }
private:
	// Variable that stores the GameState
	GameState_PG currGameState;
};