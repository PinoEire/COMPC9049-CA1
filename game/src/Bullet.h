/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#pragma once
#include "GameObjectBase.h"

/// <summary>
/// The class for the player's bullets. Inherits from GameObjectBase
/// </summary>
class Bullet : public GameObjectBase
{
public:
	/// <summary>
	/// Constructor for the instances of the bullet shot by the player
	/// </summary>
	/// <param name="theBulletTexture">The texture for this bullet</param>
	/// <param name="theDirection">The direction of this bullet</param>
	/// <param name="thePosition">The initial position where to spawn this bullet</param>
	Bullet(Texture2D theBulletTexture, Vector2 theDirection, Vector2 thePosition)
		: GameObjectBase() // Execute the base class constructor
	{
		// Assign the tag for this object
		tag = Tags::bullet;
		// define how much time this object can live
		timeToLive = 1.5;
		// Assign the texture to the instance's field
		bulletTexture = theBulletTexture;
		// Assign the direction field
		direction = theDirection;
		// Assign the scale
		scale = 0.1f;
		// Assign the rotation
		rotation = 0.0f;
		// Set the X pivot of the bullet
		pivot.x = bulletTexture.width / 2.0f * scale;
		// Set the Y pivot of the bullet
		pivot.y = bulletTexture.height / 2.0f * scale;
		// Assign the bullet's radius
		objectRadius = pivot.x;
		// Assign the initial position on the screen
		position = thePosition;
		// Adjust the X position considering the pivot
		position.x = position.x - (2 + theDirection.x) * pivot.x;
		// Adjust the Y position considering the pivot
		position.y = position.y - (2 + theDirection.y) * pivot.y;
		// Compute the correct rotation for the bullet
		rotation = static_cast<float>(atan2(direction.y, direction.x) * static_cast<float>(RAD2DEG) + 90.0f);
		// Set the bullet width
		objectRect.width = static_cast<float>(bulletTexture.width);
		// Set the bullet heigh
		objectRect.height = static_cast<float>(bulletTexture.height);
		// Set the bullet speed
		speed = 700;
		// Set the shape circularity flag
		ImCircular = false;
	}

	/// <summary>
	/// The overridden method drawing the bullet on screen
	/// </summary>
	void Draw() override;
private:
	// Define the field to hold the texture for the instance
	Texture2D bulletTexture;
};

