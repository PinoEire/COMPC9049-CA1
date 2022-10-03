#pragma once
#include "GameObjectBase.h"

extern Rectangle screen;

class Ship : public GameObjectBase
{
public:
	Ship(Texture2D ship, Texture2D turret)
		: GameObjectBase()
	{
		tag = Tags::player;
		shipTexture = ship;
		turretTexture = turret;
		scale = 0.5;

		shipRect.width = shipTexture.width;
		shipRect.height = shipTexture.height;
		shipPivot.x = shipTexture.width / 2 * scale;
		shipPivot.y = shipTexture.height / 2 * scale;

		destination.width = shipRect.width * scale;
		destination.height = shipRect.height * scale;

		// this odd radius is necessary because the incorrect collision check methods in Raylib
		objectRadius = shipTexture.width / 3 * scale;

		turretRect.width = turretTexture.width;
		turretRect.height= turretTexture.height;

		turretPivot.x = turretTexture.width / 2 * turretScale;
		turretPivot.y = turretTexture.height / 2 * turretScale;
		muzzlePosition.x = muzzlePosition.x * turretScale;
		muzzlePosition.y = muzzlePosition.y * turretScale;

		currentPosition.x = screen.width / 2;
		currentPosition.y = screen.height / 2;

		turretPosition = currentPosition;
	}
	void Draw();
	void Move(Vector2 stickValues, float deltaTime);
	void UpdateTurret(Vector2 stickValues, float deltaTime);
	Vector2 GetPosition();
	void SetPosition(Vector2 stickValues);
	Rectangle GetScreenRect();
private:
	const float turretScale = 0.25;
	const float fireRate = 0.25;

	int countBullets;

	Vector2 currentPosition{};
	Vector2 turretPosition{};
	Vector2 shipPivot{};
	Vector2 turretPivot{};
	Vector2 tmpV2{};
	Vector2 muzzlePosition{168, 35};

	float currentRotation = 0;
	float turretRotation = 0;
	float health = 1;
	float maxSpeed = 500;
	float fireRateCounter = 0;
	
	Rectangle shipRect{0, 0, 0, 0};
	Rectangle turretRect{0, 0, 0, 0};
	
	Texture2D shipTexture;
	Texture2D turretTexture;
};

