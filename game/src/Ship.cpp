#include "Ship.h"
#include <cstdint>
#include <algorithm>
#include <cmath>

void Ship::Draw()
{
	// Draw the ship
	DrawTextureEx(shipTexture, currentPosition, currentRotation, shipScale, WHITE);
	// Draw the turret
	DrawTextureEx(turretTexture, turretPosition, turretRotation, turretScale, WHITE);
}

void Ship::Move(Vector2 stickValues)
{
	currentPosition.x = std::clamp(currentPosition.x + stickValues.x * speed, (float)0, (float)screenWidth - shipTexture.width * shipScale);
	currentPosition.y = std::clamp(currentPosition.y + stickValues.y * speed, (float)0, (float)screenHeight - shipTexture.height * shipScale);
}

void Ship::UpdateTurret(Vector2 stickValues)
{
}

Vector2 Ship::GetPosition()
{
	return this->currentPosition;
}

void Ship::SetPosition(Vector2 stickValues)
{
	this->currentPosition = stickValues;
}
