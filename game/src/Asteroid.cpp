#include "Asteroid.h"
#include <cstdint>
#include <algorithm>
#include <cmath>
#include "raymath.h"

void Asteroid::Draw()
{
	destination.x = position.x;
	destination.y = position.y;
	destination.width = objectRect.width * scale;
	destination.height = objectRect.height * scale;
	DrawTexturePro(asteroidTexture, objectRect, destination, pivot, rotation, WHITE);
}

void Asteroid::Update(float deltaTime) 
{
	GameObjectBase::Update(deltaTime);
	rotation += rotationSpeed * deltaTime;
	if (rotation >= 360)
		rotation = 0;
}

