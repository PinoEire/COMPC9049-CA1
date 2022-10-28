/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#include "Ship.h"
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <list>
#include<string>
#include "raymath.h"
#include "Bullet.h"

// Declare the globally defined variable holding the bullets
extern std::list<Bullet> theBullets;
// Declare the globally defined variable holding the bullet texture
extern Texture2D bulletTexture;
// Declare the globally defined variable holding the shooting sounds
extern Sound bulletSound[3];

/// <summary>
/// The overridden method drawing the player's ship on screen
/// </summary>
void Ship::Draw()
{
	// Common destination update for Ship and Turret

	// Update the X position on the screen
	destination.x = currentPosition.x;
	// Update the Y position on the screen
	destination.y = currentPosition.y;

	// Update destination for teh Ship

	// Update the ship's width scale
	destination.width = shipRect.width * scale;
	// Update the ship's height scale
	destination.height = shipRect.height * scale;
	// Draw the ship
	DrawTexturePro(shipTexture, shipRect, destination, shipPivot, currentRotation, WHITE);

	// Update destination for teh Turret

	// Update the turret's width scale
	destination.width = turretRect.width * turretScale;
	// Update the turret's height scale
	destination.height = turretRect.height * turretScale;
	// Draw the turret
	DrawTexturePro(turretTexture, turretRect, destination, turretPivot, turretRotation, WHITE);
}

/// <summary>
/// Define the movement manager
/// </summary>
/// <param name="stickValues">Value read from the stick</param>
/// <param name="deltaTime">The delta time</param>
void Ship::Move(Vector2 stickValues, float deltaTime)
{
	if (Vector2LengthSqr(stickValues) > 0.05)  // Account for the dead zone
	{
		// Compute the new player's coordinate on X
		currentPosition.x += stickValues.x * maxSpeed * deltaTime;
		// Compute the new player's coordinate on Y
		currentPosition.y += stickValues.y * maxSpeed * deltaTime;

		// Make the ship constrained in the screen
		// so that if leaves on a side it appears
		// back from the opposite side
		if (currentPosition.x < 0)
			currentPosition.x = screen.width;	// Re-enter on the left
		if (currentPosition.y < 0)
			currentPosition.y = screen.height;	// Re-enter on the bottom
		if (currentPosition.x > screen.width)
			currentPosition.x = 0;				// Re-enter on the left
		if (currentPosition.y > screen.height)
			currentPosition.y = 0;				// Re-enter on the top
		
		// Compute and apply the ship rotation
		currentRotation = atan2(stickValues.y, stickValues.x) * RAD2DEG + 90;
		// Move the turret along
		turretPosition = currentPosition;
	}
}

/// <summary>
/// Define the turret update method
/// </summary>
/// <param name="stickValues">value read from the stick</param>
/// <param name="deltaTime">The delta time</param>
void Ship::UpdateTurret(Vector2 stickValues, float deltaTime)
{
	// Check if we have to move
	if (Vector2LengthSqr(stickValues) > 0.05)  // Account for the dead zone
	{
		// Increment the timer
		fireRateCounter += deltaTime;
		// Compute the new turret rotation
		turretRotation = atan2(stickValues.y, stickValues.x) * RAD2DEG + 90;
		// Check if the fire rete is reached
		if (fireRateCounter >= fireRate)
		{
			// Restart the timer
			fireRateCounter = 0;
			// Compute the bullet position at the muzzle
			float rotRad = turretRotation * DEG2RAD;	// We need the rotation in radians
			// Compute the X coordinate of the muzzle on the screen
			float x = (cosf(rotRad) * (muzzlePosition.x - turretPivot.x)) -
						(sinf(rotRad) * (muzzlePosition.y - turretPivot.y)) + turretPosition.x;
			// Compute the Y coordinate of the muzzle on the screen
			float y = (sinf(rotRad) * (muzzlePosition.x - turretPivot.x)) +
						(cosf(rotRad) * (muzzlePosition.y - turretPivot.y)) + turretPosition.y;
			// Crreate the muzzle position vector
			Vector2 pos{ x, y };
			// Spawn the new bullet
			Bullet bullet{ bulletTexture, stickValues, pos };
			// Add the new bullet to the list
			theBullets.push_back(bullet);
			// Play the shooting sound picking one at random
			PlaySound(bulletSound[GetRandomValue(0, 2)]);
		}
	}
	else // Sticks not touched
	{
		// Set the turret rotation to be the same as the ship
		turretRotation = currentRotation;
	}
}

/// <summary>
/// Returns the ship's position
/// </summary>
/// <returns>The ship's position</returns>
Vector2 Ship::GetPosition()
{
	return currentPosition; // returns the ship's position
}
