#pragma once

#include "Controller.h"

#include <string>

class GamepadController : public Controller {
private:
	std::string m_Name;
	i32 m_GamepadID;
public:
	GamepadController();
	virtual std::tuple<PlayerAction, PlayerAction> nextAction();
};
