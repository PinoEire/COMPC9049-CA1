#include "Ship.h"
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <list>
#include<string>
#include "raymath.h"
#include "Bullet.h"

extern std::list<Bullet> theBullets;

void Ship::Draw()
{
	// Draw the ship
	destination.x = currentPosition.x;
	destination.y = currentPosition.y;

	destination.width = shipRect.width * scale;
	destination.height = shipRect.height * scale;
	DrawTexturePro(shipTexture, shipRect, destination, shipPivot, currentRotation, WHITE);
	// Draw the turret
	destination.width = turretRect.width * turretScale;
	destination.height = turretRect.height * turretScale;
	DrawTexturePro(turretTexture, turretRect, destination, turretPivot, turretRotation, WHITE);
}

void Ship::Move(Vector2 stickValues, float deltaTime)
{
	if (Vector2LengthSqr(stickValues) > 0.05)  // Account for the dead zone
	{
		currentPosition.x += stickValues.x * maxSpeed * deltaTime;
		currentPosition.y += stickValues.y * maxSpeed * deltaTime;
		if (currentPosition.x < 0)
			currentPosition.x = screen.width;
		if (currentPosition.y < 0)
			currentPosition.y = screen.height;
		if (currentPosition.x > screen.width)
			currentPosition.x = 0;
		if (currentPosition.y > screen.height)
			currentPosition.y = 0;
		currentRotation = atan2(stickValues.y, stickValues.x) * RAD2DEG + 90;
		turretPosition = currentPosition;
	}
}

void Ship::UpdateTurret(Vector2 stickValues, float deltaTime)
{
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
			float rotRad = turretRotation * DEG2RAD;		
			float x = (cosf(rotRad) * (muzzlePosition.x - turretPivot.x)) -
						(sinf(rotRad) * (muzzlePosition.y - turretPivot.y)) + turretPosition.x;
			float y = (sinf(rotRad) * (muzzlePosition.x - turretPivot.x)) +
						(cosf(rotRad) * (muzzlePosition.y - turretPivot.y)) + turretPosition.y;
			Vector2 pos{ x, y };
			// Spawn the new bullet
			Bullet bullet{ bulletTexture, stickValues, pos };
			// Add the new bullet to the list
			theBullets.push_back(bullet);
		}
	}
	else
	{
		turretRotation = currentRotation;
	}
}

Vector2 Ship::GetPosition()
{
	return this->currentPosition;
}

void Ship::SetPosition(Vector2 stickValues)
{
	this->currentPosition = stickValues;
}

Rectangle Ship::GetScreenRect()
{
	return Rectangle{ destination.x - pivot.x, destination.y - pivot.y, destination.width, destination.height };
}
