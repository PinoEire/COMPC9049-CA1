/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#include "GameObjectBase.h"

extern Rectangle screen;

/// <summary>
/// Generate a 32 characters long unique identifier
/// </summary>
/// <returns>The game object unique identifier</returns>
std::string GameObjectBase::GetUniqueObjectId()
{
	// define the string we want to return
	std::string retValue{};
	// Loop 32 times
	for (int i = 0; i < 32; i++)
		// Concatenate a random ASCII character to the return variable
		retValue += (char)GetRandomValue(64, 126);
	// return the unique identifier to the caller
	return retValue;
}

/// <summary>
/// Base Update method executing 
/// the canonical update tasks.
/// </summary>
/// <param name="deltaTime">The delta time</param>
void GameObjectBase::Update(float deltaTime)
{
	// Move the object
	position.x += speed * direction.x * deltaTime;
	position.y += speed * direction.y * deltaTime;

	// Make the object looping in the screen window 
	if (position.x < 0)
		position.x = screen.width;
	if (position.y < 0)
		position.y = screen.height;
	if (position.x > screen.width)
		position.x = 0;
	if (position.y > screen.height)
		position.y = 0;
	// Implement a Time To Live feature for the game objects
	timeToLiveCounter += deltaTime;
	if (timeToLiveCounter >= timeToLive)
		mustDie = true;
}

float GameObjectBase::GetScale()
{
	return scale;
}

void GameObjectBase::Draw()
{
}

bool GameObjectBase::CheckCollision(Rectangle otherRect)
{
	return CheckCollisionPointRec(position, otherRect);
}

bool GameObjectBase::CheckCollision(Vector2 otherCenter, float otherRadius)
{
	return CheckCollisionPointCircle(position, otherCenter, otherRadius);
}


Tags GameObjectBase::GetTag()
{
	return tag;
}

bool GameObjectBase::IsToDie()
{
	return mustDie;
}

void GameObjectBase::YouMustDie()
{
	mustDie = true;
}

float GameObjectBase::GetRadius()
{
	return objectRadius;
}

float GameObjectBase::GetSpeed()
{
	return speed;
}

Vector2 GameObjectBase::GetPosition()
{
	return position;
}
