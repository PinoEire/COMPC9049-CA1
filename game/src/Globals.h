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
/*
/	Declatation of global variables
*/

// The width of the game screen space
int screenWidth = 1280;
// The height of the game screen space
int screenHeight = 720;
// The rectangle describing the screen space
Rectangle screen{ 0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
// Field holding the current score
int currentPoints = 0;
// Field holding the texture for the bullet
Texture2D bulletTexture;
// Array holding shooting sounds
Sound bulletSound[3];
// Array holding explosion sounds
Sound explosionSound[3];
// List to hold references to the live bullets
std::list<Bullet> theBullets;
// List to hold references to the live asteroids
std::list<Asteroid> theAsteroids;
// List to hold references to the live explosions
std::list<Explosion> theEffects;
