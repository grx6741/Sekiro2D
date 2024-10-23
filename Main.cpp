#include "GameManager.h"

#include <steam_api.h>
#include <raylib.h>

int main() {
	SteamErrMsg err = { 0 };
	if ( SteamAPI_InitEx( &err ) != k_ESteamAPIInitResult_OK ) {
		TraceLog(LOG_ERROR, "Error connecting to Steam: %s", err);
	}

	TraceLog(LOG_INFO, "Connected to Steam");

	InitWindow(1280, 720, "Sekiro2D");

	bool quit = false;
	GameManager gManager;

	while ( !WindowShouldClose() && !quit ) {
		SteamAPI_RunCallbacks();
		BeginDrawing();
		{
			ClearBackground( RAYWHITE );
			quit = gManager.RunFrame();
		}
		EndDrawing();
	}

	CloseWindow();
}