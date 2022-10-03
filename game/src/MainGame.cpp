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
extern int currentPoints;
extern int highScore;

static const char* gameWindowTitle = "Asteroids Remake";

// Forward declarations
void DrawHUD();
void CleanLists();
void CheckCollisions();
void SpawnAsteroids(float deltaTime);
void SpawnRubble(Vector2 origin, float newScale, float newSpeed);
void CreateAnAsteroid(Vector2 thePosition, float theSpeed, float theScale);

Font stencil;
std::list<Bullet> bulletsToDie;
std::list<Asteroid> asteroidsToDie;
float asteroidsSpawningTimer = 0;
float const asteroidsSpawnTime = 6;
int const maxAsteroidsToSpawn = 2;
Texture2D asteroidTextures[2];
Texture2D lifeIcon;
Rectangle lifeIconRect{};
Rectangle lifeIconDestination{};
bool isPlaying = false;
int lives = 4;

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
	float timePlayed = 0;

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

	// Initialise scene game objects
	Ship player(LoadTexture("resources/Textures/goShip.png"), LoadTexture("resources/Textures/goTurret.png"));
	Texture2D background = LoadTexture("resources/Textures/Nebula Aqua-Pink.png");
	Rectangle bgRect{ 0, 0, background.width, background.height };
	asteroidTextures[0] = LoadTexture("resources/Textures/Asteroid1.png");
	asteroidTextures[1] = LoadTexture("resources/Textures/Asteroid2.png");
	Texture2D gamepadTexture = LoadTexture("resources/Textures/gamepad.png");
	Music music = LoadMusicStream("resources/Audio/bkmusic.mp3");

	lifeIcon = LoadTexture("resources/Textures/lifeIcon.png");
	lifeIconRect = { 0, 0, (float)lifeIcon.width, (float)lifeIcon.height };
	lifeIconDestination = { 0, 0, lifeIcon.width * 0.5f, lifeIcon.height * 0.5f };


	// Load the font
	stencil = LoadFontEx("resources/Fonts/StencilStd.otf", 18, 0, 0);
	// Set bilinear scale filter for better font scaling
	SetTextureFilter(stencil.texture, TEXTURE_FILTER_BILINEAR);

	// Start the backgroung music
	PlayMusicStream(music);
	SetMusicVolume(music, 0.1);

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update logic
		deltaTime = GetFrameTime();
		// Play the music
		UpdateMusicStream(music);
		timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music);
		if (timePlayed >= 1.0f)
			SeekMusicStream(music, 0);

		if (isPlaying && IsGamepadAvailable(0))
		{
			// Get the sticks' values
			stickLeft.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
			stickLeft.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
			stickRight.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
			stickRight.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

			// Check collisions
			CheckCollisions();

			// Call the method to see if we want to spawn new asteroids
			SpawnAsteroids(deltaTime);

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
			// Process the death list
			CleanLists();
		} 
		else if (!isPlaying)
		{
			// Intro screen
			if (GetGamepadButtonPressed() == GAMEPAD_BUTTON_RIGHT_FACE_DOWN)
			{
				isPlaying = true;
				asteroidsSpawningTimer = 100;
				currentPoints = 0;
				lives = 4;
				CleanLists();
				theAsteroids.clear();
				theBullets.clear();
			}
		}

		// Drawing logic
		BeginDrawing();
		// Clear the frame
		ClearBackground(RAYWHITE);
		// Draw the background 
		DrawTexturePro(background, bgRect, screen, Vector2Zero(), 0, WHITE);

		if (isPlaying)
		{
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
		}
		else
		{
			// Draw the opening image
			DrawTexturePro(gamepadTexture, Rectangle{0, 0, (float)gamepadTexture.width, (float)gamepadTexture.height}, screen, Vector2Zero(), 0, WHITE);
		}
		// End Drawing logic
		EndDrawing();
	}

	// Unload music and close the audio device
	UnloadMusicStream(music);
	CloseAudioDevice();
	// Close window and OpenGL context
	CloseWindow();
}

/// <summary>
/// Method used to draw the Heads Up Display (HUD)
/// </summary>
void DrawHUD()
{
	// Draw score
	std::string score{ "SCORE: " + std::to_string(currentPoints)};
	DrawTextEx(stencil, score.c_str(), Vector2{10, 10}, 18, 0, YELLOW);
	// Draw FPS
	std::string fps{ "FPS: " + std::to_string(GetFPS()) };
	DrawTextEx(stencil, fps.c_str(), Vector2{ 10, screen.height - 20 }, 18, 0, YELLOW);
	// Draw lives
	for (int i = 0; i < lives; i++)
	{
		lifeIconDestination.x = screen.width - (i + 1) * lifeIconDestination.width;
		lifeIconDestination.y = 20;
		DrawTexturePro(lifeIcon, lifeIconRect, lifeIconDestination, Vector2Zero(), 0, WHITE);
	}
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
				currentPoints += 5;
				bullet->YouMustDie();
				asteroid->YouMustDie();
				float newScale = asteroid->GetScale() / 2;
				if (newScale >= 0.03)
				{
					SpawnRubble(asteroid->GetPosition(), newScale, asteroid->GetSpeed() * 1.2);
				}
				break;
			}
		}
	}
}

void SpawnRubble(Vector2 origin, float newScale, float newSpeed)
{
	// Decide how many smaller asteroids to spawn
	int howMany = GetRandomValue(3, 5);
	// Spawn the rubble
	for (int i = 0; i < howMany; i++)
	{
		CreateAnAsteroid(origin, newSpeed, newScale);
	}
}

/// <summary>
/// Method in charge of spawning new asteroids
/// </summary>
/// <param name="deltaTime">The delta time between frames</param>
/// <param name="theAsteroidTexture">The list of asteroids textures</param>
void SpawnAsteroids(float deltaTime)
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
		// Variable for the new position
		Vector2 newPosition{};
		// Spawn the new asteroids
		for (int i = 0; i < max; i++)
		{
			// Pick a new random position
			if (GetRandomValue(0, 1) == 0)
				newPosition = { (float)GetRandomValue(0, screen.width), -50 };
			else
				newPosition = { -50, (float)GetRandomValue(0, screen.height) };
			CreateAnAsteroid(newPosition, 200, 0.2);
		}
	}
}

void CreateAnAsteroid(Vector2 thePosition, float theSpeed, float theScale)
{
	// Pick a random texture index
	int idx = GetRandomValue(0, 1);
	// Create a new asteroid
	Asteroid tmp{ asteroidTextures[idx], thePosition, theSpeed, theScale };
	// Add the new asteroid to the list
	theAsteroids.push_back(tmp);
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