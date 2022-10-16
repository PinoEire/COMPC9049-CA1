/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#include "Explosion.h"

/// <summary>
/// The method drawing the explosion on screen
/// </summary>
void Explosion::Draw()
{
    if (playing) 
        DrawTexturePro(explosionTexture, objectRect, destination, objectCenter, rotation, WHITE);
}

/// <summary>
/// Method to update the necessary variables' values
/// </summary>
/// <param name="deltaTime">The delta time</param>
void Explosion::Update(float deltaTime)
{
    // Rotate 5 degrees per second
    rotation += 5 * deltaTime;
    // Check if we are still playing the animation
    if (playing)
    {
        // Increase the frame rate counter
        frameCounter += deltaTime;
        // Check if it's time to move to the next fraame
        if (frameCounter >= frameTime)
        {
            frameCounter = 0.0f;
            currentFrame++;

            if (currentFrame >= framesH)
            {
                currentFrame = 1;
                currentRow++;

                if (currentRow >= framesV)
                {
                    currentRow = 1;
                    playing = false;
                    mustDie = true;
                }
            }
        }
    }

    objectRect.x = frameWidth * currentFrame;
    objectRect.y = frameHeight * currentRow;
}
