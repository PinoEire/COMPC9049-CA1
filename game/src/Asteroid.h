#pragma once
#include "GameObjectBase.h"

/// <summary>
/// Asteroid class. It specialises the GameObjectBase class.
/// </summary>
class Asteroid : public GameObjectBase
{
public:
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
		rotation = (float)GetRandomValue(1, 359);
		// Assign a random rotation speed
		rotationSpeed = (float)GetRandomValue(10, 100);
		// Set the X pivot of the asteroid
		pivot.x = asteroidTexture.width / 2.0f * scale;
		// Set the Y pivot of the asteroid
		pivot.y = asteroidTexture.height / 2.0f * scale;
		// Pick a random direction
		while (Vector2LengthSqr(direction) == 0)
		{
			// Pick X direction
			direction.x = (float)GetRandomValue(-180, 180) / (float)360;
			// Pick Y direction
			direction.y = (float)GetRandomValue(-180, 180) / (float)360;
		}
		// Set the object's rectangle width
		objectRect.width = static_cast<float>(asteroidTexture.width);
		// Set the object's rectangle height
		objectRect.height = static_cast<float>(asteroidTexture.height);
		// Set the circular flag 
		ImCircular = true;
		// compute the visual radius of the object
		objectRadius = asteroidTexture.width * scale;
	}
	// Define the Draw method
	void Draw() override;
	// Define the Update methos
	void Update(float deltaTime) override;
private:
	float rotationSpeed;
	Texture2D asteroidTexture;
};

