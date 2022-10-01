#pragma once
#include "GameObjectBase.h"

class Asteroid : public GameObjectBase
{
public:
	Asteroid(Texture2D theTexture)
		: GameObjectBase()
	{
		asteroidTexture = theTexture;
		tag = "asteroid";
		speed = 200;
	}
	void Draw();
	Vector2 GetPosition();
private:
	Texture2D asteroidTexture;
	Vector2 direction{};
	Vector2 position{};
};

