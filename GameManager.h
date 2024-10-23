#pragma once

#include <raylib.h>
#include <raygui.h>
#include <steam_api.h>


#include "GameStates.h"

class GameManager
{
private:
	GameState m_State;
	bool m_Quit;
	CSteamID m_LobbyID;

public:
	GameManager();
	bool RunFrame();
	~GameManager();

private:
	// CallBacks ( CB )
	STEAM_CALLBACK(GameManager, OnGameLobbyJoinRequestedCB, GameLobbyJoinRequested_t);

	// CallResults ( CR )
	void OnCreateLobby(LobbyCreated_t* pCallback, bool bIOFailure);
	CCallResult<GameManager, LobbyCreated_t> m_CreateLobbyCR;

	void OnJoinLobby(LobbyEnter_t* pCallback, bool bIOFailure);
	CCallResult<GameManager, LobbyEnter_t> m_JoinLobbyCR;
};

