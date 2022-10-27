#include "raylib.h"
#include "text.h"

#define TEXT_DIR "./assets/textures/"

void LoadTextures()
{
    t2D_weapon  = LoadTexture(TEXT_DIR "sprites/weapon.png");
    t2D_grass   = LoadTexture(TEXT_DIR "backgrounds/grass.png");
    t2D_sea     = LoadTexture(TEXT_DIR "backgrounds/sea.png");
    t2D_sky     = LoadTexture(TEXT_DIR "backgrounds/sky.png");
    t2D_warthog = LoadTexture (TEXT_DIR "sprites/warthog.png");
    t2D_grenade = LoadTexture (TEXT_DIR "sprites/grenade.png");
    t2D_masterChief = LoadTexture (TEXT_DIR "sprites/masterChief.png");
    t2D_death = LoadTexture(TEXT_DIR "sprites/die.png");
}

void UnloadTextures()
{
    UnloadTexture(t2D_weapon);
    UnloadTexture(t2D_grass);
    UnloadTexture(t2D_sea);
    UnloadTexture(t2D_sky);
    UnloadTexture(t2D_warthog);
    UnloadTexture(t2D_grenade);
    UnloadTexture(t2D_masterChief);
    UnloadTexture(t2D_death);
}