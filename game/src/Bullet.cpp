#include "Bullet.h"
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "raymath.h"
#include <iostream>

extern Rectangle screen;

void Bullet::Draw()
{
	destination.x = position.x;
	destination.y = position.y;
	destination.width = bulletRect.width * scale;
	destination.height = bulletRect.height * scale;
	DrawTexturePro(bulletTexture, bulletRect, destination, pivot, rotation, WHITE);
}

Vector2 Bullet::GetPosition()
{
	return position;
}
