#pragma once
#include "GameObjectBase.h"

class Asteroid : public GameObjectBase
{
public:
	Asteroid(Texture2D theTexture, Vector2 thePosition, float theSpeed, float theScale)
		: GameObjectBase()
	{
		timeToLive = LONG_MAX;
		asteroidTexture = theTexture;
		tag = Tags::asteroid;
		speed = theSpeed;
		scale = theScale;
		position = thePosition;
		rotation = (float)GetRandomValue(1, 359);
		rotationSpeed = (float)GetRandomValue(10, 100);
		pivot.x = asteroidTexture.width / 2 * scale;
		pivot.y = asteroidTexture.height / 2 * scale;
		while (Vector2LengthSqr(direction) == 0)
		{
			direction.x = (float)GetRandomValue(-180, 180) / (float)360;
			direction.y = (float)GetRandomValue(-180, 180) / (float)360;
		}
		objectRect.width = asteroidTexture.width;
		objectRect.height = asteroidTexture.height;
		ImCircular = true;
	}
	void Draw();
	Vector2 GetPosition();
	void Update(float deltaTime);
private:
	float rotationSpeed;
	Texture2D asteroidTexture;
};

