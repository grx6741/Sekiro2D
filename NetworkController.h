#pragma once

#include "Controller.h"
#include "Player.h"

#include <steam_api.h>

/*
 * Get input from over the network and apply here
 * and simulate movement here
 */
class NetworkController : public Controller {
private:
	SteamNetworkingIdentity m_OtherPlayer;
	Player* m_CurrPlayer;
public:
	// Will not modify the player, just needs the current action the player has pressed
	NetworkController(Player* currentPlayer, CSteamID otherPlayer);

	virtual std::tuple<PlayerAction, PlayerAction> nextAction();
	virtual void update(f32 dt);

private:
	// Callbacks
	STEAM_CALLBACK(NetworkController, OnSteamNetMessageSessionFailed, SteamNetworkingMessagesSessionFailed_t);
	STEAM_CALLBACK(NetworkController, OnSteamNetMessageSessionRequest, SteamNetworkingMessagesSessionRequest_t);
};
