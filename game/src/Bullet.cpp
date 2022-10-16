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

extern Rectangle screen;

/// <summary>
/// The method drawing the bullet on screen
/// </summary>
void Bullet::Draw()
{
	destination.x = position.x;
	destination.y = position.y;
	destination.width = objectRect.width * scale;
	destination.height = objectRect.height * scale;
	DrawTexturePro(bulletTexture, objectRect, destination, pivot, rotation, WHITE);
}
