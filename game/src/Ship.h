/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#pragma once
#include "GameObjectBase.h"

extern Rectangle screen;

/// <summary>
/// Ship class used for the player. It inherits from the class GameObjectBase
/// </summary>
class Ship : public GameObjectBase
{
public:
	/// <summary>
	/// Ship class constructor
	/// </summary>
	/// <param name="ship">The ship texture</param>
	/// <param name="turret">The turret texture</param>
	Ship(Texture2D ship, Texture2D turret)
		: GameObjectBase()	// Call the base class constructor
	{
		// Set the tag to player
		tag = Tags::player;
		// Set the ship texture from the parameter
		shipTexture = ship;
		// Set the turret texture from the parameter
		turretTexture = turret;
		// Set the scale for the textures - This is hard coded
		scale = 0.5f;

		// Set the ship rectangle
		shipRect.width = static_cast<float>(shipTexture.width);		// Set the ship width
		shipRect.height = static_cast<float>(shipTexture.height);	// Set the ship height

		// Set the ship pivot
		shipPivot.x = shipTexture.width / 2.0f * scale;		// Compute the X pivot
		shipPivot.y = shipTexture.height / 2.0f * scale;	// Compute the Y pivot

		// Set the visual destination rectange 
		destination.width = shipRect.width * scale;		// Compute the visual width
		destination.height = shipRect.height * scale;	// Compute  the visual height

		// this odd radius is necessary because the incorrect collision check methods in Raylib
		objectRadius = shipTexture.width / 3.0f * scale;

		// Set the visual destination rectange 
		turretRect.width = static_cast<float>(turretTexture.width);		// Compute the visual width
		turretRect.height = static_cast<float>(turretTexture.height);	// Compute  the visual height

		// Set the turret pivot
		turretPivot.x = turretTexture.width / 2.0f * turretScale;	// Compute the X pivot
		turretPivot.y = turretTexture.height / 2.0f * turretScale;	// Compute the Y pivot
		// Compute the turret's muzzle position
		muzzlePosition.x = muzzlePosition.x * turretScale;	// Compute the X
		muzzlePosition.y = muzzlePosition.y * turretScale;	// Compute the Y

		// Set the ship position at the center of the screen
		currentPosition.x = screen.width / 2.0f;	// Set X position
		currentPosition.y = screen.height / 2.0f;	// Set Y position

		// Set the turret position to be at the ship pivot
		turretPosition = currentPosition;
	}

	/// <summary>
	/// The method drawing the player's ship on screen
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Define the movement manager
	/// </summary>
	/// <param name="stickValues">Value read from the stick</param>
	/// <param name="deltaTime">The delta time</param>
	void Move(Vector2 stickValues, float deltaTime);

	/// <summary>
	/// Define the turret update method
	/// </summary>
	/// <param name="stickValues">Value read from the stick</param>
	/// <param name="deltaTime">The delta time</param>
	void UpdateTurret(Vector2 stickValues, float deltaTime);

	/// <summary>
	/// Returns the ship's position
	/// </summary>
	/// <returns>The ship's position</returns>
	Vector2 GetPosition() override;

private:
	const float turretScale = 0.25f;
	const float fireRate = 0.25f;

	int countBullets;

	Vector2 currentPosition{};
	Vector2 turretPosition{};
	Vector2 shipPivot{};
	Vector2 turretPivot{};
	Vector2 tmpV2{};
	Vector2 muzzlePosition{168.0f, 35.0f};

	float currentRotation = 0.0f;
	float turretRotation = 0.0f;
	float health = 1.0f;
	float maxSpeed = 500.0f;
	float fireRateCounter = 0.0f;
	
	Rectangle shipRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	Rectangle turretRect{0.0f, 0.0f, 0.0f, 0.0f };
	
	Texture2D shipTexture;
	Texture2D turretTexture;
};

