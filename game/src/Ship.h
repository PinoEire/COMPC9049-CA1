#pragma once
#include "raylib.h"
#include "Globals.h"

class Ship
{
public:
	Ship(Texture2D ship, Texture2D turret)
	{
		shipTexture = ship;
		turretTexture = turret;
		currentPosition.x = screenWidth / 2 - ship.width * shipScale / 2;
		currentPosition.y = screenHeight / 2 - ship.height * shipScale / 2;
		turretPosition.x = screenWidth / 2 - turretTexture.width * turretScale / 2;
		turretPosition.y = screenHeight / 2 - turretTexture.height * turretScale / 2;
	}
	void Draw();
	void Move(Vector2 stickValues);
	void UpdateTurret(Vector2 stickValues);
	Vector2 GetPosition();
	void SetPosition(Vector2 stickValues);

private:
	Vector2 currentPosition{};
	Vector2 turretPosition{};
	float shipScale = 0.5;
	float turretScale = 0.25;
	float currentRotation = 0;
	float turretRotation = 0;
	float health = 1;
	float speed = 20;
	Texture2D shipTexture;
	Texture2D turretTexture;
};

