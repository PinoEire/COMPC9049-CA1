/*
* Author: Giuseppe De Francesco
* Year: 2022
* Product: Asteroids
*          Continous Assessment 1 for the module COMPC9049
*          DkIT - MSc in Games and Extended Reality 2022-2024
*/
#include "Explosion.h"

/// <summary>
/// The overridden method drawing the explosion on screen
/// </summary>
void Explosion::Draw()
{
    // If the animation is still playing then draw the explosion effect on the screen 
    if (playing) 
        DrawTexturePro(explosionTexture, objectRect, destination, objectCenter, rotation, WHITE);
}

/// <summary>
/// Method overridden to update the necessary variables' values
/// </summary>
/// <param name="deltaTime">The delta time</param>
void Explosion::Update(float deltaTime)
{
    // We do not call the base class method because the explosion is stationary,
    // so there is no need to compute any movement across X and Y coordinates.
    
    // Check if we are still playing the animation
    if (playing)
    {
        // Rotate 5 degrees per second
        rotation += 5 * deltaTime;
        // Increase the frame rate time counter
        frameCounter += deltaTime;
        // Check if it's time to move to the next fraame
        if (frameCounter >= frameTime)
        {
            // reset the frame rate time counter
            frameCounter = 0.0f;
            // Advance the frame
            currentFrame++;

            // check for the end of horizontal frames
            if (currentFrame == framesH)
            {
                // reset the horizontal frame
                currentFrame = 0;
                currentRow++;
                // Check for the last frame of the animation and, if so, kill the effect
                if (currentRow == framesV)
                {
                    currentRow = 0;     // Reset the row
                    playing = false;    // Signal no longet playing
                    mustDie = true;     // Signal that must be destroyed
                }
            }
        }
    }
    // update the row/column frame selectors
    objectRect.x = frameWidth * currentFrame;   // Column position
    objectRect.y = frameHeight * currentRow;    // Row position
}
