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
