#pragma once
#include "GameObjectBase.h"

class Bullet : public GameObjectBase
{
public:
	Bullet(Texture2D theBulletTexture, Vector2 theDirection, Vector2 thePosition)
		: GameObjectBase()
	{
		tag = Tags::bullet;
		timeToLive = 1.5;
		bulletTexture = theBulletTexture;
		direction = theDirection;
		scale = 0.1;
		rotation = 0;
		pivot.x = bulletTexture.width / 2 * scale;
		pivot.y = bulletTexture.height / 2 * scale;
		position = thePosition;
		position.x = position.x - (2 + theDirection.x) * pivot.x;
		position.y = position.y - (2 + theDirection.y) * pivot.y;
		rotation = atan2(direction.y, direction.x) * RAD2DEG + 90;
		objectRect.width = bulletTexture.width;
		objectRect.height = bulletTexture.height;
		speed = 700;
	}

	void Draw();
	Vector2 GetPosition();
private:
	Texture2D bulletTexture;
};

