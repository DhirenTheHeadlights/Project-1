#pragma once

/// This singleton class contains the game state manager used
/// for controlling which things appear on the screen

namespace PirateGame {
	// Enum class for game states


	namespace GameState {

		enum class State {
			Start,
			GameLoop,
			LandMassInteraction,
			OptionsMenu,
			End
		};

		// Variable that stores the State
		inline State currGameState = State::Start;
		// Track if the game has started
		inline bool gameStarted = false;

		// Change the State
		inline void changeGameState(State gameState) { 
			currGameState = gameState;
			if (gameState == State::GameLoop) {
				gameStarted = true;
			}
		}

	};
}