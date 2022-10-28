/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#include "Bullet.h"
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "raymath.h"
#include <iostream>

/// <summary>
/// The overridden method drawing the bullet on screen
/// </summary>
void Bullet::Draw()
{
	// Set the visual destination rectangle
	destination.x = position.x;						// Set X position
	destination.y = position.y;						// Set Y position
	destination.width = objectRect.width * scale;	// Set the scaled width
	destination.height = objectRect.height * scale;	// Set the scaled height
	// Draw the bullet
	DrawTexturePro(bulletTexture, objectRect, destination, pivot, rotation, WHITE);
}
