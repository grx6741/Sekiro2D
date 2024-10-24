#include "Player.h"

Player::Player(Vector2 initial_pos)
	: m_Controller(nullptr)
{
	m_Pos = initial_pos;
	m_Vel = { 0, 5000 };
}

std::tuple<PlayerAction, PlayerAction> Player::getNextAction()
{
	// Return invalid action if m_Controller is not set
	if (m_Controller == nullptr) return { PlayerAction::ACTIONS_COUNT, PlayerAction::ACTIONS_COUNT };

	auto action = m_Controller->nextAction();

	currentAction1 = std::get<0>(action);
	currentAction2 = std::get<1>(action);

	return action;
}

bool Player::isPlayerOnGround()
{
	return m_Pos.y + PLAYER_HEIGHT >= GROUND_Y;
}

void Player::update(f32 dt) {
	if (m_Controller) m_Controller->update(dt);

	std::tuple<PlayerAction, PlayerAction> action = this->getNextAction();

	// Switch case over the Action Type 1
	switch (std::get<0>(action)) {
	case PlayerAction::MOVE_LEFT:
		m_Vel.x = -WALK_SPEED;
		break;
	case PlayerAction::MOVE_RIGHT:
		m_Vel.x = WALK_SPEED;
		break;
	case PlayerAction::ACTIONS_COUNT:
		// Do nothing
		break;
	}

	// Switch case over the Action Type 2
	switch (std::get<1>(action)) {
	case PlayerAction::JUMP:
		// Jump only when on ground (No Double jumps)
		if (this->isPlayerOnGround()) {
			m_Pos.y = GROUND_Y - PLAYER_HEIGHT - 10;
			m_Vel.y = JUMP_FORCE;
		}
		break;
	case PlayerAction::ACTIONS_COUNT:
		// Do nothing
		break;
	}

	m_Vel.y += GRAVITY_FORCE * dt;

	m_Pos.x += m_Vel.x * dt;
	m_Pos.y += m_Vel.y * dt;

	// If player is on ground 
	if (this->isPlayerOnGround()) {
		// Stop player from going below the floor
		m_Pos.y = GROUND_Y - PLAYER_HEIGHT;

		// Set the velocity to be zero
		m_Vel = { 0, 0 };
	}
}

void Player::setController(Controller* controller) {
	if (m_Controller == nullptr) {
		m_Controller = controller;
	}
}

std::tuple<PlayerAction, PlayerAction> Player::getCurrentAction()
{
	return { currentAction1, currentAction2 };
}

Rectangle Player::getRect()
{
	return {m_Pos.x, m_Pos.y, PLAYER_WIDTH, PLAYER_HEIGHT};
}

Player::~Player() {
	if (m_Controller != nullptr)
		delete m_Controller;
}
