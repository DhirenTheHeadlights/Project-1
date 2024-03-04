#pragma once

/// This class contains the game state manager used
/// for controlling which things appear on the screen

namespace PirateGame {
	// Enum class for game states
	enum class GameState {
		Start,
		GameLoop,
		LandMassInteraction,
		OptionsMenu,
		End
	};

	class GameStateManager {
	public:
		// Change the GameState
		void changeGameState(GameState gameState) { this->currGameState = gameState; }

		// Grab the GameState
		GameState& getCurrentGameState() { return currGameState; }
	private:
		// Variable that stores the GameState
		GameState currGameState;
	};
}