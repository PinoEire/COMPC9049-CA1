#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>

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
	// Constructor
	GameObjectBase()
	{
		// Generate a unique object identifier
		myId = GetUniqueObjectId();
	}
	// Adding the == operator necessary to use the lists
	bool operator == (const GameObjectBase &other) const
	{
		return this->myId == other.myId;
	}
	Tags GetTag();
	bool IsToDie();
	virtual void Update(float deltaTime);
	virtual void Draw();
	bool CheckCollision(Rectangle otherRect);
	bool CheckCollision(Vector2 otherCenter, float otherRadius);
	float GetScale();
	float GetSpeed();
	virtual Vector2 GetPosition();
	float GetRadius();
	void YouMustDie();

protected:
	float objectRadius;
	float scale = 1.0f;
	float rotation = 0.0f;
	float speed = 0.0f;
	float timeToLive = 2.0f;

	bool mustDie = false;

	std::string myId{};

	Tags tag{};

	Vector2 objectCenter{};
	Vector2 pivot{};
	Vector2 direction{};
	Vector2 position{};
	Rectangle objectRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	Rectangle destination{ 0.0f, 0.0f, 0.0f, 0.0f };
	
	bool ImCircular = true;

private:
	float timeToLiveCounter = 0.0f;
	std::string GetUniqueObjectId();
};

