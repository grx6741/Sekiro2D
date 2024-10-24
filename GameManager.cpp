#include "GameManager.h"
#include "Common.h"

#include <cinttypes>

GameManager::GameManager() :
	m_State(GameState::MAINMENU),
	m_Quit(false),
	m_IsOwnerOfLobby(false),
	m_Controller1(nullptr),
	m_Controller2(nullptr),
	m_Player1({(float)(GetScreenWidth() * 0.3), 0}),
	m_Player2({(float)(GetScreenWidth() * 0.7), 0})
{
	SteamNetworkingUtils()->InitRelayNetworkAccess();
}

bool GameManager::RunFrame() {
	switch (m_State) {
	case GameState::MAINMENU:
		RenderMainmenuScreen();
		break;
	case GameState::WAITING:
		RenderWaitingScreen();
		break;
	case GameState::INLOBBY:
		RenderInLobbyScreen();
		break;
	case GameState::INGAME:
		RenderInGameScreen();
		break;
	}

	return m_Quit;
}

void GameManager::RenderMainmenuScreen() {
	if (GuiButton({ 0, 0, 200, 50 }, "Create Lobby")) {
		m_State = GameState::WAITING;

		// Create Lobby
		SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePrivate, 2);
		m_CreateLobbyCR.Set(hSteamAPICall, this, &GameManager::OnCreateLobby);
	}
	if (GuiButton({ 0, 60, 200, 50 }, "Play Offline")) {
		m_State = GameState::INGAME;

		m_Controller1 = new KeyboardController();
		m_Player1.setController(m_Controller1);

		m_Controller2 = new GamepadController();
		m_Player2.setController(m_Controller2);
	}
	if (GuiButton({ 0, 120, 200, 50 }, "Quit")) {
		m_Quit = true;
	}
}
void GameManager::RenderWaitingScreen() {
	DrawText("Creating Lobby...", 0, 0, 20, BLACK);
}
void GameManager::RenderInLobbyScreen() {
	DrawText(TextFormat("Lobby: %" PRIu64, m_LobbyID.ConvertToUint64()), 0, 0, 20, BLACK);
	GuiPanel({ 5, 30, (float)GetScreenWidth() - 10, 200 }, "Players");

	for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(m_LobbyID); i++) {
		CSteamID memberID = SteamMatchmaking()->GetLobbyMemberByIndex(m_LobbyID, i);

		// Works because there are only two players in a lobby
		if (m_OtherPlayerID.IsValid() && memberID.ConvertToUint64() == SteamUser()->GetSteamID().ConvertToUint64()) {
			m_OtherPlayerID = memberID;
		}

		const char* name = SteamFriends()->GetFriendPersonaName(memberID);
		int width = MeasureText(name, 20);
		DrawText(name, 15, 60 + i * 25, 20, BLACK);

		const char* ready = SteamMatchmaking()->GetLobbyData(m_LobbyID, name);
		DrawText(ready[0] == '1' ? "READY" : "NOT READY", 15 + width + 10, 60 + i * 25, 20, ready[0] == '1' ? GREEN : RED);
	}

	if (GuiButton({ 5, (float)(GetScreenHeight() * 0.9), 200, 50 }, "Invite Friend")) {
		SteamFriends()->ActivateGameOverlayInviteDialog(m_LobbyID);
	}

	if (m_IsOwnerOfLobby) {
		if (GuiButton({ 405, (float)(GetScreenHeight() * 0.9), 200, 50 }, "Start")) {
			SteamMatchmaking()->SetLobbyData(m_LobbyID, "READY", "1");
		}
	}

	const char* lobbyStatus = SteamMatchmaking()->GetLobbyData(m_LobbyID, "READY");
	// The lobby leader has started the game
	if (lobbyStatus[0] == '1') {
		m_State = GameState::INGAME;

		m_Controller1 = new KeyboardController();
		m_Player1.setController(m_Controller1);

		m_Controller2 = new NetworkController(&m_Player1, m_OtherPlayerID);
		m_Player2.setController(m_Controller2);
	}
}

void GameManager::RenderInGameScreen() {
	m_Player1.update(GetFrameTime());
	m_Player2.update(GetFrameTime());

	DrawRectangleRec(m_Player1.getRect(), RED);
	DrawRectangleRec(m_Player2.getRect(), GREEN);
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
	m_IsOwnerOfLobby = true;
	SteamMatchmaking()->SetLobbyData(m_LobbyID, "READY", "0");
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
}
