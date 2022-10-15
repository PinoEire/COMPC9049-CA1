#include "Asteroid.h"
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "raymath.h"

/// <summary>
/// The method drawing the asteroid on screen
/// </summary>
void Asteroid::Draw()
{
	destination.x = position.x;
	destination.y = position.y;
	destination.width = objectRect.width * scale;
	destination.height = objectRect.height * scale;
	DrawTexturePro(asteroidTexture, objectRect, destination, pivot, rotation, WHITE);
}

/// <summary>
/// Method to update the necessary variables' values
/// </summary>
/// <param name="deltaTime">The delta time</param>
void Asteroid::Update(float deltaTime)
{
	// Call the base class Update method
	GameObjectBase::Update(deltaTime);
	rotation += rotationSpeed * deltaTime;
	if (rotation >= 360)
		rotation = 0;
}

