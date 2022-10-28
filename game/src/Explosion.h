/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#pragma once
#include "GameObjectBase.h"

// Reference the externally defined sounds
extern Sound explosionSound[3];

/// <summary>
/// Class for the explosion effect. Inherits from GameObjectBase
/// </summary>
class Explosion : public GameObjectBase
{
public:
	/// <summary>
	/// Constructor for the explosion class
	/// </summary>
	/// <param name="theTexture">The texture for the spritesheet used for this explosion</param>
	/// <param name="thePosition">The initial position where to spawn this explosion</param>
	/// <param name="HorizontalFrames">How many horizontal frames are in the spritesheet</param>
	/// <param name="VerticalFrames">How many vertical frames are in the spritesheet</param>
	/// <param name="theFrameRate">The dexpected frame rate for this spritesheet</param>
	Explosion(Texture2D theTexture, Vector2 thePosition, int HorizontalFrames, int VerticalFrames, int theFrameRate)
		: GameObjectBase() // Execute the base class constructor
	{
		// Assign the tag for this object
		position = thePosition;
		// Assign the texture to the instance's field
		explosionTexture = theTexture;
		// Assign the horizontal frames
		framesH = HorizontalFrames;
		// Assign the vertical frames
		framesV = VerticalFrames;
		// Assign the frames per second rate for the animation
		frameTime = 1.0f / static_cast<float>(theFrameRate);
		// Assign the frame width
		frameWidth = explosionTexture.width / framesH;
		// Assign the frame height
		frameHeight = explosionTexture.height / framesH;
		// Assign the object's width
		objectRect.width = frameWidth;
		// Assign the object's height
		objectRect.height = frameHeight;
		// Assign the object's center on X
		objectCenter.x = frameWidth / 2.0f * scale;
		// Assign the object's center on Y
		objectCenter.y = frameHeight / 2.0f * scale;
		// Assign the object's scale
		scale = 1.0f;
		// Assign the object's destination on X
		destination.x = position.x;
		// Assign the object's destination on Y
		destination.y = position.y;
		// Assign the object's screen destination width
		destination.width = objectRect.width * scale;
		// Assign the object's screen destination height
		destination.height = objectRect.height * scale;
		// Play a random sound for this explosion
		PlaySound(explosionSound[GetRandomValue(0, 2)]);
	}
	/// <summary>
	/// The overridden method drawing the explosion on screen
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Method overridden to update the necessary variables' values
	/// </summary>
	/// <param name="deltaTime">The delta time</param>
	void Update(float deltaTime) override;

private:
	// Field holding the playing status flag
	bool playing{ true };
	// Field holding the amount of horizontal frames
	int framesH{ 5 };
	// Field holding the amount of vertical frames
	int framesV{ 5 };
	// Field holding the current frame
	int currentFrame{ 0 };
	// Field holding the current animation row
	int currentRow{ 0 };
	// Field holding the frame's width
	int frameWidth{ 0 };
	// Field holding the frame's height
	int frameHeight{ 0 };
	// Field holding the frames per second rate
	float frameTime{ 1.0f / 30.0f };
	// Field holding the value of the frames' rate time counter
	float frameCounter{ 1.0f };
	// Field holding the explosion's spritesheet
	Texture2D explosionTexture{};
};