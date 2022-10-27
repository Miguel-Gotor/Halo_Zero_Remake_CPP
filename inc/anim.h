#ifndef ANIM_H
#define ANIM_H

#include "raylib.h"
#define SPRITE_DIR "./assets/textures/sprites/"

static Color animColor = WHITE;

struct Animation
{
    Texture2D spriteSheet;

    int x = 0;
    int y = 0;

    int frameWidth;
    int frameHeight;
    int maxFrames;
    int frame = 0;

    int direction = 1;

    float timer = 0.0f;
    float timeStep;

    void init(const char *fileName, int frames, float time)
    {
        UnloadTexture(spriteSheet);
        Image temp = LoadImage(fileName); 
        ImageColorReplace(&temp, Color{ 87,137,251,255 }, BLANK);
        spriteSheet = LoadTextureFromImage(temp);
        UnloadImage(temp);

        maxFrames = frames;
        frameWidth = spriteSheet.width / frames;
        frameHeight = spriteSheet.height;
        timeStep = time;
    }

    void Draw()
    {
        DrawTextureRec
        (
            spriteSheet,
            Rectangle {(float)frameWidth * frame, 0.0, (float)frameWidth * direction, (float)frameHeight},
            Vector2   {(float)x , (float)y},
            animColor
        );
    }
};

extern Animation ani_upperBody;
extern Animation ani_lowerBody;
extern Animation ani_explosion;
extern Animation ani_warthog;

void InitAnimations();

 #endif // ANIM_H