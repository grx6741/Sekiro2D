#include "NetworkController.h"

#include <cinttypes>

NetworkController::NetworkController(Player* currentPlayer, CSteamID otherPlayer) {
	m_OtherPlayer.SetSteamID(otherPlayer);
	m_CurrPlayer = currentPlayer;

	TraceLog(LOG_INFO, "[Network Controller] Player1 %s : %" PRIu64, SteamFriends()->GetPersonaName(), SteamUser()->GetSteamID().ConvertToUint64());
	TraceLog(LOG_INFO, "[Network Controller] Player2 %s : %" PRIu64, SteamFriends()->GetFriendPersonaName(otherPlayer), otherPlayer.ConvertToUint64());
}

std::tuple<PlayerAction, PlayerAction> NetworkController::nextAction()
{
	PlayerAction action1 = PlayerAction::ACTIONS_COUNT;
	PlayerAction action2 = PlayerAction::ACTIONS_COUNT;

	SteamNetworkingMessage_t* messages[10];
	int messagesCount = SteamNetworkingMessages()->ReceiveMessagesOnChannel( 0, messages, 10);

	if (messagesCount > 0) {
		const void* msg = messages[0]->GetData();
		PlayerAction* actions = (PlayerAction*)msg;

		action1 = actions[0];
		action2 = actions[1];
	}

	return { action1, action2 };
}

void NetworkController::update(f32 dt) {
	auto action = m_CurrPlayer->getCurrentAction();

	PlayerAction actions[2] = { std::get<0>(action), std::get<1>(action) };

	EResult res = SteamNetworkingMessages()->SendMessageToUser(m_OtherPlayer, actions, sizeof(actions), k_nSteamNetworkingSend_Reliable , 0);

	switch (res) {
	case k_EResultOK:
		break;
	case k_EResultNoConnection:
		TraceLog(LOG_ERROR, "Failed to Create Session");
		break;
	case k_nSteamNetworkingSend_AutoRestartBrokenSession :
		TraceLog(LOG_ERROR, "Wierd Error");
		break;
	}
}

// Callbacks

void NetworkController::OnSteamNetMessageSessionFailed(SteamNetworkingMessagesSessionFailed_t* pCallback) {
	TraceLog(LOG_ERROR, "%s", pCallback->m_info.m_szEndDebug);
}

void NetworkController::OnSteamNetMessageSessionRequest(SteamNetworkingMessagesSessionRequest_t* pCallback) {
	bool res = SteamNetworkingMessages()->AcceptSessionWithUser(pCallback->m_identityRemote);
}
