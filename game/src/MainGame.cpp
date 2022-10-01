#include "raylib.h"
#include "raymath.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Ship.h"
#include <list>

extern int screenWidth;
extern int screenHeight;
extern std::list<Bullet> theBullets;
extern std::list<Asteroid> theAsteroids;

static const char* gameWindowTitle = "Asteroids Remake";

// Forward declarations
void DrawHUD();
void CleanLists();
Font stencil;
std::list<Bullet> bulletsToDie;
std::list<Asteroid> asteroidsToDie;


/// <summary>
/// Game execution entry point
/// </summary>
/// <param name="argc">Command line parameters' count</param>
/// <param name="argv">Command line parameters</param>
/// <returns>Game exit code. Not used at this time.</returns>
int main(int argc, char* argv[])
{
	// Locall computing variables
	Vector2 stickLeft{};
	Vector2 stickRight{};
	Vector2 tmpV2{};
	float deltaTime = 0;

	// Set MSAA 4X hint before windows creation
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	// Initialise the game window
	InitWindow(screenWidth, screenHeight, gameWindowTitle);
	// Initialize audio device
	InitAudioDevice();
	// Set our game to run at 144 frames-per-second
	SetTargetFPS(144);
	// Force early detection of the presence of the gamepad
	IsGamepadAvailable(0);


	// Initialise game objects
	Ship player(LoadTexture("resources/Textures/goShip.png"), LoadTexture("resources/Textures/goTurret.png"));
	Texture2D background = LoadTexture("resources/Textures/Nebula Aqua-Pink.png");
	Rectangle bgRect{ 0, 0, background.width, background.height };

	// Load the font
	stencil = LoadFontEx("resources/Fonts/StencilStd.otf", 18, 0, 0);
	// Set bilinear scale filter for better font scaling
	SetTextureFilter(stencil.texture, TEXTURE_FILTER_BILINEAR);

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update logic
		deltaTime = GetFrameTime();

		if (IsGamepadAvailable(0))
		{
			// Get the sticks' values
			stickLeft.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
			stickLeft.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
			stickRight.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
			stickRight.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

			// Move the player
			player.Move(stickLeft, deltaTime);
			// Apply turret rotation and shooting logic
			player.UpdateTurret(stickRight, deltaTime);
			// Update the bullets, if any
			for (auto it = theBullets.begin(); it != theBullets.end(); it++)
			{
				it->Update(deltaTime);
				if (it->IsToDie())
					bulletsToDie.push_back(*it);
			}
			for (auto it = theAsteroids.begin(); it != theAsteroids.end();)
			{
				it->Update(deltaTime);
				if (it->IsToDie())
					asteroidsToDie.push_back(*it);
			}
			CleanLists();
		}

		// Drawing logic
		BeginDrawing();
		// Clear the frame
		ClearBackground(RAYWHITE);
		// Draw the background 
		DrawTexturePro(background, bgRect, screen, Vector2Zero(), 0, WHITE);
		// Draw the HUD
		DrawHUD();

		if (!IsGamepadAvailable(0))
		{
			// Error message in case there is no gamepad attached
			DrawText("Cannot find any valid gamepad!", 0, 0, 48, RED);
		}
		else
		{
			player.Draw();
			// Draw the bullets, if any
			for (auto it = theBullets.begin(); it != theBullets.end(); it++)
				it->Draw();
			// Draw the asteroids, if any
			for (auto it = theAsteroids.begin(); it != theAsteroids.end(); it++)
				it->Draw();
		}
		EndDrawing();
	}

	// Close window and OpenGL context
	CloseWindow();
}

void DrawHUD()
{
	DrawTextEx(stencil, "Left stick to move, right stick to shoot", Vector2{ 10, 10 }, 18, 0, YELLOW);
}

void CleanLists()
{
	for (auto it = bulletsToDie.begin(); it != bulletsToDie.end(); it++)
		theBullets.remove(*it);
	for (auto it = asteroidsToDie.begin(); it != asteroidsToDie.end(); it++)
		theAsteroids.remove(*it);
	bulletsToDie.clear();
	asteroidsToDie.clear();
}