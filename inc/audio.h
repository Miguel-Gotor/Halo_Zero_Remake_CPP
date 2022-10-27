#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"

extern Sound snd_stepGrass;
extern Sound snd_stepGravel1;
extern Sound snd_stepGravel2;

extern Sound snd_jump2;
extern Sound snd_jump1;

extern Sound snd_gear;
extern Sound snd_rifle;
extern Sound snd_shotgun;
extern Sound snd_handgun;
extern Sound snd_melee;
extern Sound snd_lasergun;
extern Sound snd_minigun;
 
extern Sound snd_ambience;
extern Sound snd_noAmmo;
extern Sound snd_reload;
extern Sound snd_bullet;
extern Sound snd_pause;
extern Sound snd_unpause;
extern Sound snd_screenshot;
extern Sound snd_loop;
extern Sound snd_grenade;
extern Sound snd_enterCar;
extern Sound snd_leaveCar;
extern Sound snd_explosion;
extern Sound snd_damage;
extern Sound snd_death;
extern Sound snd_gameOver;
extern Sound snd_hints;
extern Sound snd_heartBeat;

extern Sound snd_knifeOut;
extern Sound snd_knifeSlash;

void InitAudio();
void CloseAudio();
void LoadSounds();
void UnloadSounds();

#endif // AUDIO_H