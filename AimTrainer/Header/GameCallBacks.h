#pragma once

// This class defines callbacks for the game functions
// This enables the callbacks to be used in multiple classes


// Start menu callbacks
using StartCallBack = std::function<void(float value)>;
using OpenLeaderBoardCallBack = std::function<void(float value)>;

// Options menu callbacks
using MaxTargetCallBack = std::function<void(float value)>;
using SetTargetSizeCallBack = std::function<void(float value)>;
using SetMouseSensitivityCallBack = std::function<void(float value)>;

// Leaderboard callbacks
using ExitCallBack = std::function<void(float value)>;

// Game callbacks

// End menu callbacks
using RestartCallBack = std::function<void(float value)>;
using ReturnToTitleCallBack = std::function<void(float value)>;