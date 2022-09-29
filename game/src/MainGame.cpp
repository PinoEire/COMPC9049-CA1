#include "raylib.h"
#include "Globals.h"
#include "Ship.h"

/// <summary>
/// Game execution entry point
/// </summary>
/// <param name="argc">Command line parameters' count</param>
/// <param name="argv">Command line parameters</param>
/// <returns>Game exit code. Not used at this time.</returns>
int main(int argc, char* argv[])
{
	// Locall variables
	bool wantFire = false;
	Vector2 stickLeft{};
	Vector2 stickRight{};

	// Set MSAA 4X hint before windows creation
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	// Initialise the game window
	InitWindow(screenWidth, screenHeight, gameWindowTitle);
	// Initialize audio device
	InitAudioDevice();
	// Set our game to run at 60 frames-per-second
	SetTargetFPS(60);

	// Initialise game objects
	Ship player(LoadTexture("resources/goShip.png"), LoadTexture("resources/goTurret.png"));

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update logic
		wantFire = false;

		if (IsGamepadAvailable(0))
		{
			stickLeft.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
			stickLeft.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
			stickRight.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
			stickRight.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

			if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
				wantFire = true;

			player.Move(stickLeft);
			player.UpdateTurret(stickRight);
		}

		// Drawing logic
		BeginDrawing();
		ClearBackground(RAYWHITE);
		if (!IsGamepadAvailable(0))
		{
			DrawText("Cannot find any valid gamepad!", 0, 0, 48, RED);
		}
		else
		{
			player.Draw();
		}
		EndDrawing();
	}

	// Close window and OpenGL context
	CloseWindow();
}
