#include "Explosion.h"

void Explosion::Draw()
{
    if (playing) 
        DrawTexturePro(explosionTexture, objectRect, destination, objectCenter, rotation, WHITE);
}

void Explosion::Update(float deltaTime)
{
    rotation += 5 * deltaTime;
    if (playing)
    {
        frameCounter += deltaTime;

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
