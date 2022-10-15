#pragma once
#include <list>
#include <string>
#include "raylib.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Explosion.h"

// Declatation of global variables
int screenWidth = 1280;
int screenHeight = 720;
Rectangle screen{ 0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
int currentPoints = 0;
int highScore = 0;
Texture2D bulletTexture;
Sound bulletSound[3];
Sound explosionSound[3];

std::list<Bullet> theBullets;
std::list<Asteroid> theAsteroids;
std::list<Explosion> theEffects;

