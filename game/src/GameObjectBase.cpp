#include "GameObjectBase.h"

extern Rectangle screen;

std::string GameObjectBase::GetUniqueObjectId()
{
	std::string retValue{};
	for (int i = 0; i < 32; i++)
		retValue += (char)GetRandomValue(64, 126);
	return retValue;
}

void GameObjectBase::Update(float deltaTime)
{
	position.x += speed * direction.x * deltaTime;
	position.y += speed * direction.y * deltaTime;
	if (position.x < 0)
		position.x = screen.width;
	if (position.y < 0)
		position.y = screen.height;
	if (position.x > screen.width)
		position.x = 0;
	if (position.y > screen.height)
		position.y = 0;
	timeToLiveCounter += deltaTime;
	if (timeToLiveCounter >= timeToLive)
		mustDie = true;
}



std::string GameObjectBase::GetTag()
{
	return tag;
}

bool GameObjectBase::IsToDie()
{
	return mustDie;
}