/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#include "raylib.h"
#include "raymath.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Ship.h"
#include "Explosion.h"
#include <list>

// Declare the globally defined variable holding the screen width
extern int screenWidth;
// Declare the globally defined variable holding the screen height
extern int screenHeight;
// Declare the globally defined variable holding the bullets
extern std::list<Bullet> theBullets;
// Declare the globally defined variable holding the asteroids
extern std::list<Asteroid> theAsteroids;
// Declare the globally defined variable holding the explosions
extern std::list<Explosion> theEffects;
// Declare the globally defined variable holding the player's score
extern int currentPoints;
// Declare the globally defined variable holding the bullet texture
extern Texture2D bulletTexture;
// Declare the globally defined variable holding the bullets' sounds
extern Sound bulletSound[3];
// Declare the globally defined variable holding the explosions' sounds
extern Sound explosionSound[3];

// Declare the constant strings
static const char* gameWindowTitle = "Asteroids Remake";
static const char* STR_HEALT = "Health: ";
static const char* STR_SCORE = "SCORE: ";

/*
/ Forward declarations of methods
*/

/// <summary>
/// Method used to draw the Heads Up Display (HUD)
/// </summary>
void DrawHUD();

/// <summary>
/// Method in charge of destroying the dead items from the lists
/// </summary>
void CleanLists();

/// <summary>
/// Method checking the collisions between bullets and asteroids
/// </summary>
void CheckBulletsCollisions();

/// <summary>
/// Method checking the collisions between player and asteroids
/// </summary>
void CheckPlayerCollisions();

/// <summary>
/// A method to randomly return either 1 or -1
/// </summary>
/// <returns>Either 1 or -1</returns>
int GetRandomOneOrMinusOne();

/// <summary>
/// Spawn the debris of a destroyed asteroid
/// </summary>
/// <param name="origin">Coordinate where to spawn the new pieces</param>
/// <param name="newScale">Visual scale to apply to the new rubble</param>
/// <param name="newSpeed">The speed to assign to the new debris</param>
void SpawnAsteroids(float deltaTime);

/// <summary>
/// Spawn the debris of a destroyed asteroid
/// </summary>
/// <param name="origin">Coordinate where to spawn the new pieces</param>
/// <param name="newScale">Visual scale to apply to the new rubble</param>
/// <param name="newSpeed">The speed to assign to the new debris</param>
void SpawnRubble(Vector2 origin, float newScale, float newSpeed);
void CreateAnAsteroid(Vector2 thePosition, float theSpeed, float theScale);

/*
/ Forward declarations of variables
*/

// Constant field used to hold the asteroids spawn rate
float const asteroidsSpawnTime{ 6.0f };
// Constant field used to hold the maximum camera offset for the camera shaking effect
float const cameraOffsetMax{ 8.0f };
// Constant field used to hold the maximum camera angle for the camera shaking effect
float const cameraAngleMax{ 4.0f };

// Pointer field used to hold the reference to the instance of the ship class
Ship* thePlayer;

// Field used to hold the stencil font
Font stencil{};
// Field used to hold the life icon rectangle bound
Rectangle lifeIconRect{};
// Field used to hold the life icon visual destination rectangle bound
Rectangle lifeIconDestination{};
// list used to hold the bullets that must die
std::list<Bullet> bulletsToDie{};
// list used to hold the asteroids that must die
std::list<Asteroid> asteroidsToDie{};
// list used to hold the explosions that must die
std::list<Explosion> ExplosionsToDie{};
// Field used to hold the maximum amount of asteroids to spawn at a time
int const maxAsteroidsToSpawn{ 2 };
// Field used to hold the amount of lives left
int lives{ 4 };
// Field used to hold the asteroids spawn time counter
float asteroidsSpawningTimer{ 0.0f };
// Field used to hold the current level of camera shaking effect
float cameraShakeLevel{ 0.0f };
// Field used to hold the current player's healt
float health{ 1.0f };
// Field used to hold the playing status of the game
bool isPlaying{ false };
// Field used to hold the flag to signal that we are showing the player's death
bool showPlayerDeath{ false };
// Field used to hold the explosions' spritesheets
Texture2D explosionSpritesheets[2]{};
// Field used to hold the asteroids' textures
Texture2D asteroidTextures[2]{};
// Field used to hold the life icon
Texture2D lifeIcon{};

/// <summary>
/// Game execution entry point
/// </summary>
/// <param name="argc">Command line parameters' count</param>
/// <param name="argv">Command line parameters</param>
/// <returns>Game exit code. Not used at this time.</returns>
int main(int argc, char* argv[])
{
	/*
	/ Local variables
	*/

	// Field used to hold the reading of the left gamepad's stick
	Vector2 stickLeft{};
	// Field used to hold the reading of the right gamepad's stick
	Vector2 stickRight{};
	// Field used to hold temporary vector calculations
	Vector2 tmpV2{};
	// Field used to hold the delta time (tame passed since last frame execution)
	float deltaTime{ 0.0f };
	// Field used to hold the value of the play time of the background music to make it loop
	float timePlayed{ 0.0f };
	// Field used to hold the delay for the player's death effect before to show a new ship
	float playerDeathCounter{ 0.0f };
	// Field used to hold the camera object
	Camera2D camera{};
	// Field used to hold the camera offset (initialised to zero)
	Vector2 cameraOffset = Vector2Zero();
	// Field used to hold the camera angle
	float cameraAngle{ 0.0f };

	/*
	/ Raylib system initialization
	*/

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

	/*
	/ Game elements initialization
	*/

	// Load the bullet texture
	bulletTexture = LoadTexture("resources/Textures/bullet.png");
	// create the player
	thePlayer = new Ship(LoadTexture("resources/Textures/goShip.png"), LoadTexture("resources/Textures/goTurret.png"));
	// define and load the background texture
	Texture2D background = LoadTexture("resources/Textures/Nebula Aqua-Pink.png");
	// define and load the background texture's bounding rectabgle
	Rectangle bgRect{ 0.0f, 0.0f, static_cast<float>(background.width), static_cast<float>(background.height) };

	// bgRectDest is the screenk destination rectangle for the background image.
	// Adding off-screed margin so to allow the camera shake effect to 
	// present on screen without white borders.
	Rectangle bgRectDest{ -200.0f, -200.0f, static_cast<float>(screen.width + 400), static_cast<float>(screen.height + 400) };

	// Load the first astreroid's texture
	asteroidTextures[0] = LoadTexture("resources/Textures/Asteroid1.png");
	// Load the second astreroid's texture
	asteroidTextures[1] = LoadTexture("resources/Textures/Asteroid2.png");
	// define and load the gamepad texture
	Texture2D gamepadTexture = LoadTexture("resources/Textures/gamepad.png");
	// Define and load the background music
	Music music = LoadMusicStream("resources/Audio/bkmusic.mp3");
	// Load the first explosion's spritesheet
	explosionSpritesheets[0] = LoadTexture("resources/Textures/Explosion01_5x5.png");
	// Load the second explosion's spritesheet
	explosionSpritesheets[1] = LoadTexture("resources/Textures/Explosion02_5x5.png");
	// Load the life icon texture
	lifeIcon = LoadTexture("resources/Textures/lifeIcon.png");
	// Initialise the life icon's bounding box's rectangle
	lifeIconRect = { 0, 0, static_cast<float>(lifeIcon.width), static_cast<float>(lifeIcon.height) };
	// Initialise the life icon's bounding box's visual destination rectangle
	lifeIconDestination = { 0, 0, lifeIcon.width * 0.5f, lifeIcon.height * 0.5f };

	// Load the first explosion's sound
	explosionSound[0] = LoadSound("resources/Audio/Explosion 25.wav");
	// Load the second explosion's sound
	explosionSound[1] = LoadSound("resources/Audio/Explosion 26.wav");
	// Load the third explosion's sound
	explosionSound[2] = LoadSound("resources/Audio/Explosion 27.wav");
	// Load the first shooting's sound
	bulletSound[0] = LoadSound("resources/Audio/shoot1.wav");
	// Load the second shooting's sound
	bulletSound[1] = LoadSound("resources/Audio/shoot2.wav");
	// Load the third shooting's sound
	bulletSound[2] = LoadSound("resources/Audio/shoot3.wav");
	// Define and load the voice over sound for the Game Over announcement
	Sound gameOver = LoadSound("resources/Audio/NarratorVoice_gameOver.mp3");
	// Load the stencil font for the HUD
	stencil = LoadFontEx("resources/Fonts/StencilStd.otf", 18, 0, 0);
	// Set bilinear scale filter for better font scaling
	SetTextureFilter(stencil.texture, TEXTURE_FILTER_BILINEAR);

	// Start the backgroung music
	PlayMusicStream(music);
	// Set the music volume to 10%
	SetMusicVolume(music, 0.1f);

	/*
	/ Main game loop
	*/
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
			// Manage life and death
		if (health == 0.0f)
		{
			lives--;
			health = 1.0f;
			// Manage last death
			if (lives < 0) {
				isPlaying = false;
				PlaySound(gameOver);
				// Reset the camera in case we were in the middle of shaking
				cameraOffset.x = 0.0f;	// Zero out X offset
				cameraOffset.y = 0.0f;	// Zero out Y offset
				cameraAngle = 0.0f;		// Zero out the angle
				// Assign the X and Y offset
				camera.offset = cameraOffset;
				// Assign the rotation
				camera.rotation = cameraAngle;
			}
			// Set the death and create the explosion
			showPlayerDeath = true;
			playerDeathCounter = 0.0f;
			// Creathe the explosion effect picking one at random
			Explosion explosion{ explosionSpritesheets[GetRandomValue(0, 1)], thePlayer->GetPosition(), 5, 5, 30 };
			// Add the effect to the effects list
			theEffects.push_back(explosion);
		}

		if (isPlaying && IsGamepadAvailable(0))
		{
			// Get the sticks' values
			stickLeft.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);	// Get the left stick's X value
			stickLeft.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);	// Get the left stick's Y value
			stickRight.x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);	// Get the right stick's X value
			stickRight.y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);	// Get the right stick's Y value

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
				playerDeathCounter += deltaTime;	// count the player death interval
				if (playerDeathCounter >= 1.5f)		// check if we reached 1.5 seconds
					showPlayerDeath = false;		// and, if so, reset the delay flag
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
			// Update the asteroids, if any
			for (auto it = theAsteroids.begin(); it != theAsteroids.end(); it++)
			{
				// Call the object Update method
				it->Update(deltaTime);
				// if the object must die then add it to the death list
				if (it->IsToDie())
					asteroidsToDie.push_back(*it);
			}
			// Update effects, if any
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
			cameraOffset.x = cameraShakeLevel * cameraOffsetMax * GetRandomOneOrMinusOne();	// Generate X offset
			cameraOffset.y = cameraShakeLevel * cameraOffsetMax * GetRandomOneOrMinusOne();	// Generate Y offset
			cameraAngle = cameraShakeLevel * cameraAngleMax * GetRandomOneOrMinusOne();		// Generate rotation
			// Assign the camera offset
			camera.offset = cameraOffset;
			// Assign the camera rotation
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
				// Set the playing status to true
				isPlaying = true;
				// force to spawn a set of asteroids immediately
				asteroidsSpawningTimer = 100.0f;
				// zero out the score counter
				currentPoints = 0;
				// reset the max lives
				lives = 4;
				// reset the player's health
				health = 1.0f;
				// Clean up all the death lists
				CleanLists();
				// clear the asteroids 
				theAsteroids.clear();
				// clear the bullets
				theBullets.clear();
			}
		}

		/*
		* DRAWING LOGIC
		*/

		// Tell Raylib that we are about to draw the current render texture
		BeginDrawing();
		// start the 2D camera
		BeginMode2D(camera);
		// Clear the frame
		ClearBackground(RAYWHITE);
		// Draw the background 
		DrawTexturePro(background, bgRect, bgRectDest, Vector2Zero(), 0, WHITE);

		if (isPlaying) // if the game is playing draw the game objects
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
		else // if we are not playing, draw the initial screen
		{
			// Draw the opening image
			DrawTexturePro(gamepadTexture, Rectangle{0, 0, (float)gamepadTexture.width, (float)gamepadTexture.height}, screen, Vector2Zero(), 0, WHITE);
		}
		// Stop the 2D camera
		EndMode2D();
		// Tell Raylib to finalise and push the render texture
		EndDrawing();
	}

	// Unload music and close the audio device
	UnloadMusicStream(music);
	// Close the audio device
	CloseAudioDevice();
	// Close window and OpenGL context
	CloseWindow();

	/*
	* There is no need for further 
	* cleaning up unloading the resources 
	* because that will be done automatically 
	* by the operating system
	* when the main game ends.
	*/
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
	// Draw the lives
	for (int i = 0; i < lives; i++)
	{
		lifeIconDestination.x = screen.width - (i + 1) * lifeIconDestination.width;
		lifeIconDestination.y = 20;
		DrawTexturePro(lifeIcon, lifeIconRect, lifeIconDestination, Vector2Zero(), 0, WHITE);
	}
	// Draw health the bar 
	// using colours red/yellow/green based on 
	// how much health we have left
	Color hbColor;			// Define a local color field
	if (health >= 0.7f)		// if we have at least 70% life
		hbColor = GREEN;	// set the draw field to green
	else if (health >= 0.4f)// or if we have more than 40% life
		hbColor = YELLOW;	// set the draw field to yellow
	else					// otherwise
		hbColor = RED;		// set the draw field to red

	// Get the lenght of the text so that we can compute the correct horizontal position 
	auto textLenght = MeasureTextEx(stencil, STR_HEALT, 18, 0);
	// Draw the health text
	DrawTextEx(stencil, STR_HEALT, Vector2{ screen.width / 2 - textLenght.x, 10 }, 18, 0, YELLOW);
	// Draw the health bar
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
		// Using two circles because the CircleRec gives imprecise results
		if (CheckCollisionCircles(asteroid->GetPosition(), asteroid->GetRadius(), 
									thePlayer->GetPosition(), thePlayer->GetRadius()))
		{
			// Increment the camera shake level
			cameraShakeLevel = Clamp(cameraShakeLevel += 0.2f, 0.0f, 1.0f);
			// Signal the asteroid he must die
			asteroid->YouMustDie();
			// decrease the player's health
			health = Clamp(health - 0.15f, 0.0f, 1.0f);
			// Instantiate an explosion effect at the asteroid'd position
			Explosion explosion{ explosionSpritesheets[GetRandomValue(0, 1)], asteroid->GetPosition(), 5, 5, 30 };
			// Add the effect to the list of the current playing effects
			theEffects.push_back(explosion);
		}
	}
}

/// <summary>
/// Method checking the collisions between bullets and asteroids
/// </summary>
void CheckBulletsCollisions()
{
	// Parse  all the bullets alive
	for (auto bullet = theBullets.begin(); bullet != theBullets.end(); bullet++)
	{
		// For each bullet parse all the asteroids
		for (auto asteroid = theAsteroids.begin(); asteroid != theAsteroids.end(); asteroid++)
		{
			// check if the current bullet hits the current asteroid
			if (bullet->CheckCollision(asteroid->GetPosition(), asteroid->GetRadius()))
			{
				// Add points to the current player's score
				currentPoints += 5;
				// Tell the bullet to die
				bullet->YouMustDie();
				// tell the asteroid to die
				asteroid->YouMustDie();
				// new scale for the rubble
				float newScale = asteroid->GetScale() / 2;
				// if the new scale is at least 3% of the full asteroid then spawn new rubble
				if (newScale >= 0.03)
				{
					// Spawn rubble at the asterois position and increasing their speed by 20%
					SpawnRubble(asteroid->GetPosition(), newScale, asteroid->GetSpeed() * 1.2f);
				}
				// Create an explosion effect at the asteroid's position
				Explosion explosion{ explosionSpritesheets[GetRandomValue(0, 1)], asteroid->GetPosition(), 5, 5, 30 };
				// Add the explosion to the list of live effects
				theEffects.push_back(explosion);
				// stop parsing the asteroids, move to the next bullet, if any
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