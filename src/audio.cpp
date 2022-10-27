#include "audio.h"
#define SND_DIR "./assets/sounds/"
void LoadSounds()
{
    // snd_gameOver    = LoadSound(SND_DIR "gameOver2.ogg");
    snd_jump1       = LoadSound(SND_DIR "jump.ogg");
    snd_jump2       = LoadSound(SND_DIR "jump0.ogg");
    snd_stepGrass   = LoadSound(SND_DIR "stepGrass.ogg");
    snd_stepGravel1 = LoadSound(SND_DIR "stepGravel1.ogg");
    snd_stepGravel2 = LoadSound(SND_DIR "stepGravel2.ogg");
    snd_gear        = LoadSound(SND_DIR "gear.ogg");
    snd_rifle       = LoadSound(SND_DIR "rifle.ogg");
    snd_shotgun     = LoadSound(SND_DIR "shotgun.ogg");
    snd_handgun     = LoadSound(SND_DIR "handgun.ogg");
    snd_melee       = LoadSound(SND_DIR "melee.ogg");
    snd_ambience    = LoadSound(SND_DIR "wav/ambience.wav");
    snd_noAmmo      = LoadSound(SND_DIR "noAmmo.ogg");
    snd_reload      = LoadSound(SND_DIR "reload.ogg");
    snd_bullet      = LoadSound(SND_DIR "bullet.ogg");
    snd_pause       = LoadSound(SND_DIR "pause.ogg");
    snd_unpause     = LoadSound(SND_DIR "unpause.ogg");
    snd_screenshot  = LoadSound(SND_DIR "screenshot.ogg");
    snd_loop        = LoadSound(SND_DIR "loop.ogg");
    snd_grenade     = LoadSound(SND_DIR "grenade.ogg");
    snd_enterCar    = LoadSound(SND_DIR "enterCar.ogg");
    snd_leaveCar    = LoadSound(SND_DIR "leaveCar.ogg");
    snd_explosion   = LoadSound(SND_DIR "explosion.ogg");
    snd_damage      = LoadSound(SND_DIR "damage.ogg");
    snd_death       = LoadSound(SND_DIR "death.ogg");
    snd_hints       = LoadSound(SND_DIR "hints.ogg");
    snd_heartBeat   = LoadSound(SND_DIR "heartBeat.ogg");
    snd_lasergun    = LoadSound(SND_DIR "lasergun.ogg");
    snd_minigun     = LoadSound(SND_DIR "minigun.ogg");
    snd_knifeOut    = LoadSound(SND_DIR "knife.ogg");
    snd_knifeSlash  = LoadSound(SND_DIR "wav/knife.wav");
}

void UnloadSounds()
{
    UnloadSound(snd_jump1);
    UnloadSound(snd_jump2);
    UnloadSound(snd_stepGrass);
    UnloadSound(snd_stepGravel1);
    UnloadSound(snd_stepGravel2);
    UnloadSound(snd_gear);
    UnloadSound(snd_rifle);
    UnloadSound(snd_shotgun);
    UnloadSound(snd_handgun);
    UnloadSound(snd_melee);
    UnloadSound(snd_ambience);
    UnloadSound(snd_noAmmo);
    UnloadSound(snd_reload);
    UnloadSound(snd_bullet);
    UnloadSound(snd_pause);
    UnloadSound(snd_unpause);
    UnloadSound(snd_screenshot);
    UnloadSound(snd_loop);
    UnloadSound(snd_grenade);
    UnloadSound(snd_enterCar);
    UnloadSound(snd_leaveCar);
    UnloadSound(snd_explosion);
    UnloadSound(snd_damage);
    UnloadSound(snd_death);
    UnloadSound(snd_hints);
    UnloadSound(snd_heartBeat);
    UnloadSound(snd_lasergun);
    UnloadSound(snd_minigun);
}

void InitAudio()
{
    InitAudioDevice();
    LoadSounds();
    SetSoundVolume(snd_stepGrass, 3.F);
    SetSoundVolume(snd_hints, 8.F);
    SetSoundVolume(snd_ambience, 4.F);
    SetSoundVolume(snd_knifeSlash, 4.F);
}

void CloseAudio()
{
    CloseAudioDevice();
    UnloadSounds();
}