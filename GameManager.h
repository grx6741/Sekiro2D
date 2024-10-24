#pragma once

#include <raylib.h>
#include <raygui.h>

#include <steam_api.h>

#include "GameStates.h"
#include "Player.h"
#include "Controller.h"

#include "KeyboardController.h"
#include "GamepadController.h"
#include "NetworkController.h"

class GameManager
{
private:
	GameState m_State;
	CSteamID m_LobbyID;

	CSteamID m_OtherPlayerID;

	bool m_Quit;
	bool m_IsOwnerOfLobby;

	Player m_Player1;
	Player m_Player2;

	Controller* m_Controller1;
	Controller* m_Controller2;

public:
	GameManager();
	bool RunFrame();
	~GameManager();

private:
	// Scene Renderers
	void RenderMainmenuScreen();
	void RenderWaitingScreen();
	void RenderInLobbyScreen();
	void RenderInGameScreen();

	// CallBacks ( CB )
	STEAM_CALLBACK(GameManager, OnGameLobbyJoinRequestedCB, GameLobbyJoinRequested_t);

	// CallResults ( CR )
	void OnCreateLobby(LobbyCreated_t* pCallback, bool bIOFailure);
	CCallResult<GameManager, LobbyCreated_t> m_CreateLobbyCR;

	void OnJoinLobby(LobbyEnter_t* pCallback, bool bIOFailure);
	CCallResult<GameManager, LobbyEnter_t> m_JoinLobbyCR;
};

