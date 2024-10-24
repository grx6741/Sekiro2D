#pragma once

#include "Common.h"
#include "Action.h"
#include "Controller.h"

#define GRAVITY_FORCE 1000
#define JUMP_FORCE -500
#define GROUND_Y ((float) (GetScreenHeight() * 0.9))

#define WALK_SPEED 500
#define ROLL_SPEED 200

#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 70

class Player {
private:
	Controller* m_Controller;

	Vector2 m_Pos;
	Vector2 m_Vel;

	PlayerAction currentAction1;
	PlayerAction currentAction2;

private:
	std::tuple<PlayerAction, PlayerAction> getNextAction();
	bool isPlayerOnGround();

public:
	Player(Vector2 initial_pos);
	~Player();
	void update(f32 dt);
	void setController(Controller* controller);

	Rectangle getRect();
	std::tuple<PlayerAction, PlayerAction> getCurrentAction();
};
