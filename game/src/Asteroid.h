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
/// Asteroid class. It specialises the GameObjectBase class.
/// </summary>
class Asteroid : public GameObjectBase
{
public:
	/// <summary>
	/// Constructor for the Asteroid class
	/// </summary>
	/// <param name="theTexture">Texture of this instance of the class</param>
	/// <param name="thePosition">The initial position of this asteroid on the screen</param>
	/// <param name="theSpeed">Movement speed for this asteroid</param>
	/// <param name="theScale">Visual scale of this asteroid instance</param>
	Asteroid(Texture2D theTexture, Vector2 thePosition, float theSpeed, float theScale)
		: GameObjectBase()	// Execute the base class constructor
	{
		// Make the asteroid live almost forever
		timeToLive = FLT_MAX;
		// Assign the texture from the consctuctor's parameters
		asteroidTexture = theTexture;
		// Assign a tad to yhr object
		tag = Tags::asteroid;
		// Assign the speed from the consctuctor's parameters
		speed = theSpeed;
		// Assign the scale from the consctuctor's parameters
		scale = theScale;
		// Assign the position from the consctuctor's parameters
		position = thePosition;
		// Assign a random rotation
		rotation = static_cast<float>(GetRandomValue(1, 359));
		// Assign a random rotation speed
		rotationSpeed = static_cast<float>(GetRandomValue(10, 100));
		// Set the X pivot of the asteroid
		pivot.x = asteroidTexture.width / 2.0f * scale;
		// Set the Y pivot of the asteroid
		pivot.y = asteroidTexture.height / 2.0f * scale;
		// Pick a random direction
		while (Vector2LengthSqr(direction) == 0.0f)
		{
			// Pick X direction
			direction.x = GetRandomValue(-180, 180) / 360.0f;
			// Pick Y direction
			direction.y = GetRandomValue(-180, 180) / 360.0f;
		}
		// Set the object's rectangle width
		objectRect.width = static_cast<float>(asteroidTexture.width);
		// Set the object's rectangle height
		objectRect.height = static_cast<float>(asteroidTexture.height);
		// Compute the visual width
		destination.width = objectRect.width * scale;
		// Compute the visual height
		destination.height = objectRect.height * scale;
		// Set the circular flag 
		ImCircular = true;
		// compute the visual radius of the object
		objectRadius = asteroidTexture.width * scale;
	}

	/// <summary>
	/// The method drawing the asteroid on screen overridden from base class
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Method override to update the necessary variables' values
	/// </summary>
	/// <param name="deltaTime">The delta time</param>
	void Update(float deltaTime) override;
private:
	// field used to store the rotation speed
	float rotationSpeed;
	// Field used to store the aasteroid texture
	Texture2D asteroidTexture;
};

