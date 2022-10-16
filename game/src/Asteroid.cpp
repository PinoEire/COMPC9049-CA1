/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#include "Asteroid.h"

/// <summary>
/// The method drawing the asteroid on screen overridden from base class
/// </summary>
void Asteroid::Draw()
{
	// Set the visual position on X
	destination.x = position.x;
	// Set the visual position on Y
	destination.y = position.y;
	// Draw the asteroid on the screen
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

