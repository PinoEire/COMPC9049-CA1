#include "raylib.h"
#include "raymath.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Ship.h"
#include "Explosion.h"
#include <list>

// Declare the external references to the global variables
extern int screenWidth;
extern int screenHeight;
extern std::list<Bullet> theBullets;
extern std::list<Asteroid> theAsteroids;
extern std::list<Explosion> theEffects;
extern int currentPoints;
extern int highScore;
extern Texture2D bulletTexture;
extern Sound bulletSound[3];
extern Sound explosionSound[3];

// Declare the constants
static const char* gameWindowTitle = "Asteroids Remake";
static const char* STR_HEALT = "Health: ";
static const char* STR_SCORE = "SCORE: ";

// Forward declarations of methods
void DrawHUD();
void CleanLists();
void CheckBulletsCollisions();
void CheckPlayerCollisions();
int GetRandomOneOrMinusOne();
void SpawnAsteroids(float deltaTime);
void SpawnRubble(Vector2 origin, float newScale, float newSpeed);
void CreateAnAsteroid(Vector2 thePosition, float theSpeed, float theScale);

// Forward declarations of variables
Font stencil;
std::list<Bullet> bulletsToDie;
std::list<Asteroid> asteroidsToDie;
std::list<Explosion> ExplosionsToDie;
float asteroidsSpawningTimer = 0.0f;
float const asteroidsSpawnTime = 6.0f;
int const maxAsteroidsToSpawn = 2;
Texture2D asteroidTextures[2];
Texture2D lifeIcon;
Rectangle lifeIconRect{};
Rectangle lifeIconDestination{};
Ship *thePlayer;
bool isPlaying = false;
int lives = 4;
float cameraShakeLevel = 0.0f;
float const cameraOffsetMax = 8.0f;
float const cameraAngleMax = 4.0f;
float health = 1.0f;
bool showPlayerDeath = false;
Texture2D explosionSpritesheets[2];

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
	float deltaTime = 0.0f;
	float timePlayed = 0.0f;
	float playerDeathCounter = 0.0f;

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
	bulletTexture = LoadTexture("resources/Textures/bullet.png");
	thePlayer = new Ship(LoadTexture("resources/Textures/goShip.png"), LoadTexture("resources/Textures/goTurret.png"));
	Texture2D background = LoadTexture("resources/Textures/Nebula Aqua-Pink.png");
	Rectangle bgRect{ 0.0f, 0.0f, static_cast<float>(background.width), static_cast<float>(background.height) };

	// bgRectDest is the screenk destination rectangle for the background image.
	// Adding off-screed margin so to allow the camera shake effect to 
	// present on screen without white borders.
	Rectangle bgRectDest{ -200.0f, -200.0f, static_cast<float>(screen.width + 400), static_cast<float>(screen.height + 400) };
	
	asteroidTextures[0] = LoadTexture("resources/Textures/Asteroid1.png");
	asteroidTextures[1] = LoadTexture("resources/Textures/Asteroid2.png");
	Texture2D gamepadTexture = LoadTexture("resources/Textures/gamepad.png");
	Music music = LoadMusicStream("resources/Audio/bkmusic.mp3");
	Camera2D camera{0};
	Vector2 cameraOffset = Vector2Zero();
	float cameraAngle = 0.0f;

	explosionSpritesheets[0] = LoadTexture("resources/Textures/Explosion01_5x5.png");
	explosionSpritesheets[1] = LoadTexture("resources/Textures/Explosion02_5x5.png");

	lifeIcon = LoadTexture("resources/Textures/lifeIcon.png");
	lifeIconRect = { 0, 0, (float)lifeIcon.width, (float)lifeIcon.height };
	lifeIconDestination = { 0, 0, lifeIcon.width * 0.5f, lifeIcon.height * 0.5f };

	// Load sounds
	explosionSound[0] = LoadSound("resources/Audio/Explosion 25.wav");
	explosionSound[1] = LoadSound("resources/Audio/Explosion 26.wav");
	explosionSound[2] = LoadSound("resources/Audio/Explosion 27.wav");
	bulletSound[0] = LoadSound("resources/Audio/shoot1.wav");
	bulletSound[1] = LoadSound("resources/Audio/shoot2.wav");
	bulletSound[2] = LoadSound("resources/Audio/shoot3.wav");
	Sound gameOver = LoadSound("resources/Audio/NarratorVoice_gameOver.mp3");


	// Load the font
	stencil = LoadFontEx("resources/Fonts/StencilStd.otf", 18, 0, 0);
	// Set bilinear scale filter for better font scaling
	SetTextureFilter(stencil.texture, TEXTURE_FILTER_BILINEAR);

	// Start the backgroung music
	PlayMusicStream(music);
	SetMusicVolume(music, 0.1f);

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		/*
		* UPDATE LOGIC
		*/

		// Reset camera
		camera.zoom = 1.0f;
		camera.target = { 0, 0 };

		// Update logic
		deltaTime = GetFrameTime();
		// Play the music
		UpdateMusicStream(music);
		timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music);
		// Loop the music
		if (timePlayed >= 1.0f)
			SeekMusicStream(music, 0);

		// Check if the player is playing and the gamepad is available
		if (isPlaying && IsGamepadAvailable(0))
		{
			// Get the sticks' values
			stickLeft.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
			stickLeft.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
			stickRight.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
			stickRight.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

			// Manage life and death
			if (health == 0.0f)
			{
				lives--;
				health = 1.0f;
				// Manage last death
				if (lives < 0) {
					isPlaying = false;
					PlaySound(gameOver);
				}
				// Set the death and create the explosion
				showPlayerDeath = true;
				playerDeathCounter = 0.0f;
				// Creathe the explosion effect picking one at random
				Explosion explosion{ explosionSpritesheets[GetRandomValue(0, 1)], thePlayer->GetPosition(), 5, 5, 30 };
				// Add the effect to the effects list
				theEffects.push_back(explosion);
			}

			// Check the bullets' collisions
			CheckBulletsCollisions();
			// Check the player's collisions
			CheckPlayerCollisions();

			// Call the method to see if we want to spawn new asteroids
			SpawnAsteroids(deltaTime);

			// Process player movement is they are not dead
			if (!showPlayerDeath)
			{
				// Move the player
				thePlayer->Move(stickLeft, deltaTime);
				// Apply turret rotation and shooting logic
				thePlayer->UpdateTurret(stickRight, deltaTime);
			}
			else
			{
				// If the player is dead we don't draw draw it for 1.5 seconds
				playerDeathCounter += deltaTime;
				if (playerDeathCounter >= 1.5f)
					showPlayerDeath = false;
			}

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
			// Update effects
			for (auto it = theEffects.begin(); it != theEffects.end(); it++)
			{
				// Call the object Update method
				it->Update(deltaTime);
				// if the object must die then add it to the death list
				if (it->IsToDie())
					ExplosionsToDie.push_back(*it);
			}
			// Process the death list
			CleanLists();

			// Update camera shaking level 
			// (using a basic algorithm here instead of the usual perlin noise for the sake of simplicity)
			cameraOffset.x = cameraShakeLevel * cameraOffsetMax * GetRandomOneOrMinusOne();
			cameraOffset.y = cameraShakeLevel * cameraOffsetMax * GetRandomOneOrMinusOne();
			cameraAngle = cameraShakeLevel * cameraAngleMax * GetRandomOneOrMinusOne();
			camera.offset = cameraOffset;
			camera.rotation = cameraAngle;
			// reduce the level od shaking
			cameraShakeLevel = Clamp(cameraShakeLevel -= deltaTime, 0.0f, 1.0f);
		}
		// If not playing then present the intro screen
		else if (!isPlaying)
		{
			// To start a new game when pressing the "A" on the gamepad 
			// we reset the necessary variables
			if (GetGamepadButtonPressed() == GAMEPAD_BUTTON_RIGHT_FACE_DOWN)
			{
				isPlaying = true;
				// force to spawn a set of asteroids immediately
				asteroidsSpawningTimer = 100.0f;
				currentPoints = 0;
				lives = 4;
				health = 1.0f;
				CleanLists();
				theAsteroids.clear();
				theBullets.clear();
			}
		}

		/*
		* DRAWING LOGIC
		*/

		BeginDrawing();
		// start the 2D camera
		BeginMode2D(camera);
		// Clear the frame
		ClearBackground(RAYWHITE);
		// Draw the background 
		DrawTexturePro(background, bgRect, bgRectDest, Vector2Zero(), 0, WHITE);

		if (isPlaying)
		{
			// Is there a gamepad?
			if (!IsGamepadAvailable(0))
			{
				// Error message in case there is no gamepad attached and nothing else
				DrawText("Cannot find any valid gamepad!", 0, 0, 48, RED);
			}
			// Yes, there is a gamepad, so we draw the game's items
			else
			{
				// Check if we have to draw the player or not
				if (!showPlayerDeath)
				{
					// Draw the player
					thePlayer->Draw();
				}
				// Draw the bullets, if any
				for (auto it = theBullets.begin(); it != theBullets.end(); it++)
					it->Draw();
				// Draw the asteroids, if any
				for (auto it = theAsteroids.begin(); it != theAsteroids.end(); it++)
					it->Draw();
				// Draw the effects, if any
				for (auto it = theEffects.begin(); it != theEffects.end(); it++)
					it->Draw();
			}
			// Draw the HUD
			DrawHUD();
		}
		else
		{
			// Draw the opening image
			DrawTexturePro(gamepadTexture, Rectangle{0, 0, (float)gamepadTexture.width, (float)gamepadTexture.height}, screen, Vector2Zero(), 0, WHITE);
		}
		// Stop the 2D camera
		EndMode2D();
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
/// A method to randomly return either 1 or -1
/// </summary>
/// <returns>Either 1 or -1</returns>
int GetRandomOneOrMinusOne()
{
	// We get a random between 0 and 1, and then 
	// return -1 if we get 0, otherwise 1
	return GetRandomValue(0, 1) == 0 ? -1 : 1;
}

/// <summary>
/// Method used to draw the Heads Up Display (HUD)
/// </summary>
void DrawHUD()
{
	// Draw score
	std::string score{ STR_SCORE + std::to_string(currentPoints)};
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
	// Draw health the bar 
	// using colours red/yellow/green based on 
	// how much health we have left
	Color hbColor;
	if (health >= 0.7f)
		hbColor = GREEN;
	else if (health >= 0.4f)
		hbColor = YELLOW;
	else
		hbColor = RED;
	// Get the lenght of the text so that we can compute the correct horizontal position 
	auto textLenght = MeasureTextEx(stencil, STR_HEALT, 18, 0);
	DrawTextEx(stencil, STR_HEALT, Vector2{ screen.width / 2 - textLenght.x, 10 }, 18, 0, YELLOW);
	DrawRectangle(static_cast<int>(screen.width / 2), 8, static_cast<int>(200 * health), 20, hbColor);
}

/// <summary>
/// Method checking the collisions between player and asteroids
/// </summary>
void CheckPlayerCollisions()
{
	// Parse all the asteroid and check if we are colliding
	for (auto asteroid = theAsteroids.begin(); asteroid != theAsteroids.end(); asteroid++)
	{
		// Using two circles because the Circle
		if (CheckCollisionCircles(asteroid->GetPosition(), asteroid->GetRadius(), 
									thePlayer->GetPosition(), thePlayer->GetRadius()))
		{
			cameraShakeLevel = Clamp(cameraShakeLevel += 0.2f, 0.0f, 1.0f);
			asteroid->YouMustDie();
			health = Clamp(health - 0.15f, 0.0f, 1.0f);
		}
	}
}

/// <summary>
/// Method checking the collisions between bullets and asteroids
/// </summary>
void CheckBulletsCollisions()
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
					SpawnRubble(asteroid->GetPosition(), newScale, asteroid->GetSpeed() * 1.2f);
				}
				Explosion explosion{ explosionSpritesheets[GetRandomValue(0, 1)], asteroid->GetPosition(), 5, 5, 30 };
				theEffects.push_back(explosion);
				break;
			}
		}
	}
}

/// <summary>
/// Spawn the debris of a destroyed asteroid
/// </summary>
/// <param name="origin">Coordinate where to spawn the new pieces</param>
/// <param name="newScale">Visual scale to apply to the new rubble</param>
/// <param name="newSpeed">The speed to assign to the new debris</param>
void SpawnRubble(Vector2 origin, float newScale, float newSpeed)
{
	// Decide how many smaller asteroids to spawn
	int howMany = GetRandomValue(3, 5);
	// Spawn the rubble
	for (int i = 0; i < howMany; i++)
	{
		// Spawn the new asteroids
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
			// Pick a new random position off the screen
			if (GetRandomValue(0, 1) == 0) // if zero
				// random X position
				newPosition = { static_cast<float>(GetRandomValue(0, static_cast<int>(screen.width))), -50.0f };
			else
				// random Y position
				newPosition = { -50.0f, static_cast<float>(GetRandomValue(0, static_cast<int>(screen.height))) };
			// Call the method to create the new asteroid
			CreateAnAsteroid(newPosition, 200.0f, 0.2f);
		}
	}
}

/// <summary>
/// Create an instance of the Asterois class 
/// and add it to the list of live asteroids
/// </summary>
/// <param name="thePosition">Position where to spawn the asteroid</param>
/// <param name="theSpeed">Speed of the new asteroid</param>
/// <param name="theScale">Visual scale of the new asteroid</param>
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
	// Remove all dead bullets
	for (auto it = bulletsToDie.begin(); it != bulletsToDie.end(); it++)
		theBullets.remove(*it);
	// Remove all dead asteroids
	for (auto it = asteroidsToDie.begin(); it != asteroidsToDie.end(); it++)
		theAsteroids.remove(*it);
	// Remove all dead explosions
	for (auto it = ExplosionsToDie.begin(); it != ExplosionsToDie.end(); it++)
		theEffects.remove(*it);
	// Clear the "to die" lists
	bulletsToDie.clear();		// Clear the bullets
	asteroidsToDie.clear();		// Clear the asteroids
	ExplosionsToDie.clear();	// Clear the explosions
}