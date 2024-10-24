#pragma once

#include "Controller.h"
#include "Action.h"

#include <unordered_map>

#include <raylib.h>

class KeyboardController : public Controller {
private:
	std::unordered_map<PlayerAction, i32> m_KeyMappings;
public:
	KeyboardController();
	virtual std::tuple<PlayerAction, PlayerAction> nextAction();
};
