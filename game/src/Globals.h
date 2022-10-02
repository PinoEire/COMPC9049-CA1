#pragma once
#include <list>
#include <string>
#include "raylib.h"
#include "Asteroid.h"
#include "Bullet.h"

int screenWidth = 1280;
int screenHeight = 720;
Rectangle screen{ 0, 0, screenWidth, screenHeight };
int currentPoints = 0;
int highScore = 0;

std::list<Bullet> theBullets;
std::list<Asteroid> theAsteroids;

