#pragma once
#include "GameObjectBase.h"

extern Sound explosionSound[3];

class Explosion : public GameObjectBase
{
public:
	Explosion(Texture2D theTexture, Vector2 thePosition)
		: GameObjectBase()
	{
		position = thePosition;

		explosionTexture = theTexture;

		frameWidth = explosionTexture.width / framesH;
		frameHeight = explosionTexture.height / framesH;

		objectRect.width = frameWidth;
		objectRect.height = frameHeight;

		objectCenter.x = frameWidth / 2 * scale;
		objectCenter.y = frameHeight / 2 * scale;

		scale = 1.0f;

		destination.x = position.x;
		destination.y = position.y;
		destination.width = objectRect.width * scale;
		destination.height = objectRect.height * scale;
		PlaySound(explosionSound[GetRandomValue(0, 2)]);
	}
	void Draw();
	void Update(float deltaTime);
private:
	bool playing = true;
	float framesH = 5;
	float framesV = 5;
	float currentFrame = 0;
	float currentRow = 0;
	float frameWidth = 0.0f;
	float frameHeight = 0.0f;
	float frameTime = 1.0f /30.0f;
	float frameCounter = 1.0f;
	Texture2D explosionTexture;
};