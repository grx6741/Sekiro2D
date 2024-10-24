#pragma once

#include <raylib.h>

#include "Common.h"

enum class PlayerAction {
	MOVE_LEFT,
	MOVE_RIGHT,
	JUMP,
	ACTIONS_COUNT
};

static const char* PlayerActionTable[(u32)PlayerAction::ACTIONS_COUNT] = {
	"MOVE_LEFT",
	"MOVE_RIGHT",
	"JUMP",
};

struct SpriteSheet {
	Texture texture;
	u8 frames_count;
	u32 duration; // in ms
};
