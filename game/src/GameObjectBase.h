#pragma once
#include "raylib.h"
#include <string>

class GameObjectBase
{
public:
	GameObjectBase()
	{
		myId = GetUniqueObjectId();
	}
	bool operator == (const GameObjectBase &other) const
	{
		return this->myId == other.myId;
	}
	std::string GetTag();
	bool IsToDie();
	void Update(float deltaTime);
protected:
	float speed = 0;
	float timeToLive = 2;
	bool mustDie = false;
	std::string myId{};
	std::string tag{};
	Vector2 direction{};
	Vector2 position{};
private:
	float timeToLiveCounter = 0;
	std::string GetUniqueObjectId();
};

