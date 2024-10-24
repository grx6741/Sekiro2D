#include "GamepadController.h"

#include "raygui.h"

GamepadController::GamepadController() {
	m_GamepadID = 0;
	if (IsGamepadAvailable(m_GamepadID)) {
		m_Name = GetGamepadName(m_GamepadID);
		TraceLog(LOG_INFO, "[GamepadController] Controller Detected : %s", m_Name.c_str());
	}
}

std::tuple<PlayerAction, PlayerAction> GamepadController::nextAction()
{
	PlayerAction action1 = PlayerAction::ACTIONS_COUNT;
	PlayerAction action2 = PlayerAction::ACTIONS_COUNT;

	if (IsGamepadAvailable(m_GamepadID)) {
		m_Name = GetGamepadName(m_GamepadID);

		// Action Type 1
		if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
			action1 = PlayerAction::MOVE_LEFT;
		}
		if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
			action2 = PlayerAction::MOVE_RIGHT;
		}

		// Action Type 2
		if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
			action2 = PlayerAction::JUMP;
		}
	}

	return std::make_tuple(action1, action2);
}
