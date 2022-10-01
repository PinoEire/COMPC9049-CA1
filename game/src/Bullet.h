#pragma once
#include "GameObjectBase.h"

class Bullet : public GameObjectBase
{
public:
	Bullet(Texture2D theBulletTexture, Vector2 theDirection, Vector2 thePosition)
		: GameObjectBase()
	{
		tag = "bullet";
		timeToLive = 1.5;
		bulletTexture = theBulletTexture;
		direction = theDirection;
		pivot.x = bulletTexture.width / 2 * scale;
		pivot.y = bulletTexture.height / 2 * scale;
		position = thePosition;
		position.x = position.x - (2 + theDirection.x) * pivot.x;
		position.y = position.y - (2 + theDirection.y) * pivot.y;
		rotation = atan2(direction.y, direction.x) * RAD2DEG + 90;
		bulletRect.width = bulletTexture.width;
		bulletRect.height = bulletTexture.height;
		speed = 700;
	}

	void Draw();
	Vector2 GetPosition();
private:
	float scale = 0.1;
	float rotation = 0;
	
	Texture2D bulletTexture;
	Vector2 pivot{};

	Rectangle bulletRect{ 0, 0, 0, 0 };
	Rectangle destination{ 0, 0, 0, 0 };
};

