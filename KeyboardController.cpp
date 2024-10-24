#include "KeyboardController.h"

#include "raylib.h"

KeyboardController::KeyboardController() {
	m_KeyMappings[PlayerAction::MOVE_LEFT] = KEY_A;
	m_KeyMappings[PlayerAction::MOVE_RIGHT] = KEY_D;
	m_KeyMappings[PlayerAction::JUMP] = KEY_W;
}

std::tuple<PlayerAction, PlayerAction> KeyboardController::nextAction()
{
	PlayerAction action1 = PlayerAction::ACTIONS_COUNT;
	PlayerAction action2 = PlayerAction::ACTIONS_COUNT;

	// For Actions of Type 1
	if (IsKeyDown(m_KeyMappings[PlayerAction::MOVE_LEFT])) {
		action1 = PlayerAction::MOVE_LEFT;
	}
	if (IsKeyDown(m_KeyMappings[PlayerAction::MOVE_RIGHT])) {
		action1 = PlayerAction::MOVE_RIGHT;
	}

	// For Actions of Type 1
	if (IsKeyPressed(m_KeyMappings[PlayerAction::JUMP])) {
		action2 = PlayerAction::JUMP;
	}
	return std::make_tuple(action1, action2);
}
