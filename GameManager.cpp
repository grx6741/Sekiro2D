#include "GameManager.h"
#include <cinttypes>

GameManager::GameManager() :
	m_State(GameState::MAINMENU),
	m_Quit(false),
	m_Ready(false)
{

}

bool GameManager::RunFrame() {
	switch (m_State) {
	case GameState::MAINMENU:
		if (GuiButton({ 0, 0, 200, 50 }, "Create Lobby")) {
			m_State = GameState::WAITING;
			SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePrivate, 2);
			m_CreateLobbyCR.Set(hSteamAPICall, this, &GameManager::OnCreateLobby);
		}
		if (GuiButton({ 0, 120, 200, 50 }, "Quit")) {
			m_Quit = true;
		}
		break;
	case GameState::WAITING:
		DrawText("Creating Lobby...", 0, 0, 20, BLACK);
		break;
	case GameState::INLOBBY:
		DrawText(TextFormat("Lobby: %" PRIu64, m_LobbyID.ConvertToUint64()), 0, 0, 20, BLACK);
		GuiPanel({ 5, 30, (float)GetScreenWidth() - 10, 200 }, "Players");

		for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(m_LobbyID); i++) {
			CSteamID memberID = SteamMatchmaking()->GetLobbyMemberByIndex(m_LobbyID, i);
			const char* name = SteamFriends()->GetFriendPersonaName(memberID);
			int width = MeasureText(name, 20);
			DrawText(name, 15, 60 + i * 25, 20, BLACK);

			const char* ready = SteamMatchmaking()->GetLobbyData(m_LobbyID, name);
			DrawText(ready[0] == '1' ? "READY" : "NOT READY", 15 + width + 10, 60 + i * 25, 20, ready[0] == '1' ? GREEN : RED);
		}

		if (GuiButton({ 5, (float)(GetScreenHeight() * 0.9), 200, 50}, "Invite Friend")) {
			SteamFriends()->ActivateGameOverlayInviteDialog(m_LobbyID);
		}

		if (GuiButton({ 205, (float)(GetScreenHeight() * 0.9), 200, 50}, "Toggle Ready")) {
			m_Ready = !m_Ready;
			SteamMatchmaking()->SetLobbyData(m_LobbyID, SteamFriends()->GetPersonaName(), m_Ready ? "1" : "0" );
		}
		break;
	case GameState::INGAME:
		break;
	}

	return m_Quit;
}

GameManager::~GameManager() {
	if (m_LobbyID.IsLobby()) {
		SteamMatchmaking()->LeaveLobby(m_LobbyID);
	}
}

// CallBacks

void GameManager::OnGameLobbyJoinRequestedCB(GameLobbyJoinRequested_t* pCallback) {
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->JoinLobby(pCallback->m_steamIDLobby);
	m_JoinLobbyCR.Set(hSteamAPICall, this, &GameManager::OnJoinLobby);
}


// CalResults

void GameManager::OnCreateLobby(LobbyCreated_t* pCallback, bool bIOFailure) {
	if (bIOFailure) {
		TraceLog(LOG_ERROR, "CreateLobby() Failed");
	}

	if (pCallback->m_eResult != k_EResultOK || pCallback->m_ulSteamIDLobby == 0) {
		TraceLog(LOG_ERROR, "Failed to Create Lobby [EResult: %d]", pCallback->m_eResult);
		return;
	}

	TraceLog(LOG_INFO, "Create Lobby SteamID: %" PRIu64, pCallback->m_ulSteamIDLobby);

	m_LobbyID.SetFromUint64(pCallback->m_ulSteamIDLobby);
	m_State = GameState::INLOBBY;

	// Set to Not ready when just joined the lobby
	SteamMatchmaking()->SetLobbyData(m_LobbyID, SteamFriends()->GetPersonaName(), m_Ready ? "1" : "0" );
}


void GameManager::OnJoinLobby(LobbyEnter_t* pCallback, bool bIOFailure) {
	if (bIOFailure) {
		TraceLog(LOG_ERROR, "JoinLobby() Failed");
	}

	if (pCallback->m_EChatRoomEnterResponse == k_EChatRoomEnterResponseError) {
		TraceLog(LOG_ERROR, "Failed Joining Lobby %" PRIu64, pCallback->m_ulSteamIDLobby);
	}

	TraceLog(LOG_INFO, "joined Lobby %" PRIu64 "Successfully", pCallback->m_ulSteamIDLobby);

	m_LobbyID = pCallback->m_ulSteamIDLobby;
	m_State = GameState::INLOBBY;

	// Set to Not ready when just joined the lobby
	SteamMatchmaking()->SetLobbyData(m_LobbyID, SteamFriends()->GetPersonaName(), m_Ready ? "1" : "0" );
}
