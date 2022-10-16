/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
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

// List to hold references to the live bullets
std::list<Bullet> theBullets;
// List to hold references to the live asteroids
std::list<Asteroid> theAsteroids;
// List to hold references to the live explosions
std::list<Explosion> theEffects;

