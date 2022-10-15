#pragma once
#include "GameObjectBase.h"

extern Sound explosionSound[3];

class Explosion : public GameObjectBase
{
public:
	Explosion(Texture2D theTexture, Vector2 thePosition, int HorizontalFrames, int VerticalFrames, int theFrameRate)
		: GameObjectBase()
	{
		position = thePosition;
		explosionTexture = theTexture;
		framesH = static_cast<float>(HorizontalFrames);
		framesV = static_cast<float>(VerticalFrames);
		frameTime = 1.0f / (float)theFrameRate;

		frameWidth = explosionTexture.width / framesH;
		frameHeight = explosionTexture.height / framesH;

		objectRect.width = frameWidth;
		objectRect.height = frameHeight;

		objectCenter.x = frameWidth / 2.0f * scale;
		objectCenter.y = frameHeight / 2.0f * scale;

		scale = 1.0f;

		destination.x = position.x;
		destination.y = position.y;
		destination.width = objectRect.width * scale;
		destination.height = objectRect.height * scale;
		PlaySound(explosionSound[GetRandomValue(0, 2)]);
	}
	void Draw() override;
	void Update(float deltaTime) override;
private:
	bool playing = true;

	float framesH = 5.0f;
	float framesV = 5.0f;
	float currentFrame = 0.0f;
	float currentRow = 0.0f;
	float frameWidth = 0.0f;
	float frameHeight = 0.0f;
	float frameTime = 1.0f /30.0f;
	float frameCounter = 1.0f;

	Texture2D explosionTexture;
};