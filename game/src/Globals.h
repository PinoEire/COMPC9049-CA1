#pragma once
#include "raylib.h"

static const int screenWidth = 1280;
static const int screenHeight = 720;
static const char *gameWindowTitle = "To Be Determined";

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Sound fxCoin;
