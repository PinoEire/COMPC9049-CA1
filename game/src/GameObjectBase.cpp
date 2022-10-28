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
	// Move the object on the screen
	position.x += speed * direction.x * deltaTime;	// X movement
	position.y += speed * direction.y * deltaTime;	// Y movement

	// Make the object looping in the screen window 
	if (position.x < 0)
		position.x = screen.width;	// re-enter fron the right
	if (position.y < 0)
		position.y = screen.height;	// re-enter fron the left
	if (position.x > screen.width)
		position.x = 0;				// re-enter fron the top
	if (position.y > screen.height)
		position.y = 0;				// re-enter fron the bottom

	// Implement a Time To Live feature for the game objects
	timeToLiveCounter += deltaTime;	// increase the TTL time counter
	// If it's time to die then set the mustDie flag
	if (timeToLiveCounter >= timeToLive)
		mustDie = true;
}

/// <summary>
/// Retrieve teh currect stale for this object
/// </summary>
/// <returns>The current scale</returns>
float GameObjectBase::GetScale()
{
	return scale;
}

void GameObjectBase::Draw()
{
}

/// <summary>
/// Method checking rectangular collisions
/// </summary>
/// <param name="otherRect">The other object rectangle</param>
/// <returns>true if colliding</returns>
bool GameObjectBase::CheckCollision(Rectangle otherRect)
{
	return CheckCollisionPointRec(position, otherRect);
}

/// <summary>
/// Method checking rectangular collisions with a circular object
/// </summary>
/// <param name="otherCenter">The center of the other object</param>
/// <param name="otherRadius">The radius of the other object</param>
/// <returns>true if colliding</returns>
bool GameObjectBase::CheckCollision(Vector2 otherCenter, float otherRadius)
{
	return CheckCollisionPointCircle(position, otherCenter, otherRadius);
}

/// <summary>
/// Method retrieving the object's tag
/// </summary>
/// <returns></returns>
Tags GameObjectBase::GetTag()
{
	return tag;
}

/// <summary>
/// Method to know if this instance is set to die
/// </summary>
/// <returns></returns>
bool GameObjectBase::IsToDie()
{
	return mustDie;
}

/// <summary>
/// Method to tell the object that has to be destroyed
/// </summary>
void GameObjectBase::YouMustDie()
{
	mustDie = true;
}

/// <summary>
/// Retrieves the object radius
/// </summary>
/// <returns>The object radius</returns>
float GameObjectBase::GetRadius()
{
	return objectRadius;
}

/// <summary>
/// Retrieves the current speed for this object
/// </summary>
/// <returns>The current object speed</returns>
float GameObjectBase::GetSpeed()
{
	return speed;
}

/// <summary>
/// Virtual method to retrieve the current object position
/// </summary>
/// <returns>The object position</returns>
Vector2 GameObjectBase::GetPosition()
{
	return position;
}
