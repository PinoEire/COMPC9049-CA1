/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>

/// <summary>
/// Tags classifying the type of game object
/// </summary>
enum Tags
{
	player,
	asteroid,
	ufo,
	bullet
};

/// <summary>
/// Base class for the game objects. 
/// This is a simplification of what 
/// a fully fledged Game Object is supposed to be.
/// </summary>
class GameObjectBase
{
public:
	/// <summary>
	/// The constructor of the class GameObjectBase
	/// </summary>
	GameObjectBase()
	{
		// Generate a unique object identifier
		myId = GetUniqueObjectId();
	}

	/// <summary>
	/// The == operator necessary to use the lists
	/// </summary>
	/// <param name="other">The other object we are to be compared against</param>
	/// <returns>true if this is the same game object</returns>
	bool operator == (const GameObjectBase &other) const
	{
		return this->myId == other.myId;
	}

	/// <summary>
	/// Method retrieving the object's tag
	/// </summary>
	/// <returns></returns>
	Tags GetTag();

	/// <summary>
	/// Method to know if this instance is set to die
	/// </summary>
	/// <returns></returns>
	bool IsToDie();

	/// <summary>
	/// Virtual method to update the necessary variables' values
	/// </summary>
	/// <param name="deltaTime">The delta time</param>
	virtual void Update(float deltaTime);

	/// <summary>
	/// The virtual method drawing the object on screen
	/// </summary>
	virtual void Draw();
	
	/// <summary>
	/// Method checking rectangular collisions
	/// </summary>
	/// <param name="otherRect">The other object rectangle</param>
	/// <returns>true if colliding</returns>
	bool CheckCollision(Rectangle otherRect);

	/// <summary>
	/// Method checking rectangular collisions with a circular object
	/// </summary>
	/// <param name="otherCenter">The center of the other object</param>
	/// <param name="otherRadius">The radius of the other object</param>
	/// <returns>true if colliding</returns>
	bool CheckCollision(Vector2 otherCenter, float otherRadius);

	/// <summary>
	/// Retrieve teh currect stale for this object
	/// </summary>
	/// <returns>The current scale</returns>
	float GetScale();

	/// <summary>
	/// Retrieves the current speed for this object
	/// </summary>
	/// <returns>The current object speed</returns>
	float GetSpeed();

	/// <summary>
	/// Virtual method to retrieve the current object position
	/// </summary>
	/// <returns>The object position</returns>
	virtual Vector2 GetPosition();

	/// <summary>
	/// Retrieves the object radius
	/// </summary>
	/// <returns>The object radius</returns>
	float GetRadius();

	/// <summary>
	/// Method to tell the object that has to be destroyed
	/// </summary>
	void YouMustDie();

protected:
	// Field holding the object's radius
	float objectRadius;
	// Field holding the object's scale
	float scale{ 1.0f };
	// Field holding the object's rotation
	float rotation{ 0.0f };
	// Field holding the object's speed
	float speed{ 0.0f };
	// Field holding the time for this object to live befor to be removed from the scene
	float timeToLive{ 2.0f };
	// Field to signal if this object has been destroyed on the current frame
	bool mustDie = false;
	// Field holding the unique identifier for this object's instance
	std::string myId{};
	// Field holding the object's tag
	Tags tag{};
	// Field holding the object's center
	Vector2 objectCenter{};
	// Field holding the object's pivot
	Vector2 pivot{};
	// Field holding the object's direction
	Vector2 direction{};
	// Field holding the object's position
	Vector2 position{};
	// Field holding the object's rectangle bound
	Rectangle objectRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	// Field holding the object's screen destination rectangle bound
	Rectangle destination{ 0.0f, 0.0f, 0.0f, 0.0f };
	// Field holding the object's circularity status
	bool ImCircular = true;

private:
	// Field holding the object's tile to live counter
	float timeToLiveCounter = 0.0f;
	// Method generating the object's unique identifier
	std::string GetUniqueObjectId();
};

