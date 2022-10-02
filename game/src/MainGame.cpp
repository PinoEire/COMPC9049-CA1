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
void CheckCollisions();
void SpawnAsteroids(float deltaTime, Texture2D theAsteroidTexture[]);

Font stencil;
std::list<Bullet> bulletsToDie;
std::list<Asteroid> asteroidsToDie;
float asteroidsSpawningTimer = 0;
float const asteroidsSpawnTime = 6;
int const maxAsteroidsToSpawn = 2;

/// <summary>
/// Game execution entry point
/// </summary>
/// <param name="argc">Command line parameters' count</param>
/// <param name="argv">Command line parameters</param>
/// <returns>Game exit code. Not used at this time.</returns>
int main(int argc, char* argv[])
{
	// Local computing variables
	Vector2 stickLeft{};
	Vector2 stickRight{};
	Vector2 tmpV2{};
	float deltaTime = 0;
	Texture2D asteroidTexture[2];


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

	// Initialise basic game objects
	Ship player(LoadTexture("resources/Textures/goShip.png"), LoadTexture("resources/Textures/goTurret.png"));
	Texture2D background = LoadTexture("resources/Textures/Nebula Aqua-Pink.png");
	Rectangle bgRect{ 0, 0, background.width, background.height };
	asteroidTexture[0] = LoadTexture("resources/Textures/Asteroid1.png");
	asteroidTexture[1] = LoadTexture("resources/Textures/Asteroid2.png");


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

			// Call the method to see if we want to spawn new asteroids
			SpawnAsteroids(deltaTime, asteroidTexture);

			// Move the player
			player.Move(stickLeft, deltaTime);
			// Apply turret rotation and shooting logic
			player.UpdateTurret(stickRight, deltaTime);
			// Update the bullets, if any
			for (auto it = theBullets.begin(); it != theBullets.end(); it++)
			{
				// Call the object Update method
				it->Update(deltaTime);
				// if the object must die then add it to the death list
				if (it->IsToDie())
					bulletsToDie.push_back(*it);
			}
			for (auto it = theAsteroids.begin(); it != theAsteroids.end(); it++)
			{
				// Call the object Update method
				it->Update(deltaTime);
				// if the object must die then add it to the death list
				if (it->IsToDie())
					asteroidsToDie.push_back(*it);
			}
			// Check collisions
			CheckCollisions();
			// Process the death list
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

/// <summary>
/// Method used to draw the Heads Up Display (HUD)
/// </summary>
void DrawHUD()
{
	DrawTextEx(stencil, "Left stick to move, right stick to shoot", Vector2{ 10, 10 }, 18, 0, YELLOW);
}

/// <summary>
/// Method checking the collisions between objects
/// </summary>
void CheckCollisions()
{
	for (auto bullet = theBullets.begin(); bullet != theBullets.end(); bullet++)
	{
		for (auto asteroid = theAsteroids.begin(); asteroid != theAsteroids.end(); asteroid++)
		{
			if (bullet->CheckCollision(asteroid->GetPosition(), asteroid->GetRadius()))
			{
				asteroid->YouMustDie();
			}
		}
	}
}

/// <summary>
/// Method in charge of spawning new asteroids
/// </summary>
/// <param name="deltaTime">The delta time between frames</param>
/// <param name="theAsteroidTexture">The list of asteroids textures</param>
void SpawnAsteroids(float deltaTime, Texture2D theAsteroidTexture[])
{
	// Increment the timer
	asteroidsSpawningTimer += deltaTime;
	// Check if we match the spawn rate
	if (asteroidsSpawningTimer >= asteroidsSpawnTime)
	{
		// Reset the timer
		asteroidsSpawningTimer = 0;
		// Establish how many asteroids to spawn
		int max = GetRandomValue(1, maxAsteroidsToSpawn);
		// Spawn the new asteroids
		for (int i = 0; i < max; i++)
		{
			// Pick a random texture index
			int idx = GetRandomValue(0, 1);
			// Randomise the position based on odd/even spawning asteroid
			if ((i + 1) % 2 == 0)
			{
				// Create a new asteroid
				Asteroid tmp{ theAsteroidTexture[idx], Vector2{(float)GetRandomValue(0, screen.width), -50}, 200, 0.2 };
				// Add the new asteroid to the list
				theAsteroids.push_back(tmp);
			}
			else
			{
				// Create a new asteroid
				Asteroid tmp{ theAsteroidTexture[idx], Vector2{-50, (float)GetRandomValue(0, screen.height)}, 200, 0.2 };
				// Add the new asteroid to the list
				theAsteroids.push_back(tmp);
			}
		}
	}
}

/// <summary>
/// Method in charge of destroying the dead items from the lists
/// </summary>
void CleanLists()
{
	for (auto it = bulletsToDie.begin(); it != bulletsToDie.end(); it++)
		theBullets.remove(*it);
	for (auto it = asteroidsToDie.begin(); it != asteroidsToDie.end(); it++)
		theAsteroids.remove(*it);
	bulletsToDie.clear();
	asteroidsToDie.clear();
}