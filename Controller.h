#pragma once

#include <tuple>

#include "Action.h"

/*
* There are two types of action
* 1. Some actions require the key to be held
* 2. Some actions only should be triggered on key press
*/

class Controller {
public:
	//                 Action Type1, Action Type2
	virtual std::tuple<PlayerAction, PlayerAction> nextAction() = 0;
	virtual inline void update(f32 dt) {};
};
