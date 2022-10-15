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
		scale = 0.1f;
		rotation = 0.0f;
		pivot.x = bulletTexture.width / 2.0f * scale;
		pivot.y = bulletTexture.height / 2.0f * scale;
		objectRadius = pivot.x;
		position = thePosition;
		position.x = position.x - (2 + theDirection.x) * pivot.x;
		position.y = position.y - (2 + theDirection.y) * pivot.y;
		rotation = static_cast<float>(atan2(direction.y, direction.x) * static_cast<float>(RAD2DEG) + 90.0f);
		objectRect.width = static_cast<float>(bulletTexture.width);
		objectRect.height = static_cast<float>(bulletTexture.height);
		speed = 700;
		ImCircular = false;
	}

	void Draw() override;
private:
	Texture2D bulletTexture;
};

