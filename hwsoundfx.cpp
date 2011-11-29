/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwsoundfx.cpp): sound fx functions
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include "hwdsound.h"
#include "hwsoundfx.h"

void LoadSoundFX(void)
{
    LoadWAV(".\\SoundFX\\xlogo.wav");
    LoadWAV(".\\SoundFX\\hajijump.wav");
    SetSoundVolume(FX_HAJIJUMP, 50);
    LoadWAV(".\\SoundFX\\skid.wav");
    SetSoundVolume(FX_SKID, 75);
    LoadWAV(".\\SoundFX\\whoosh.wav");
    LoadWAV(".\\SoundFX\\ouch.wav");
    LoadWAV(".\\SoundFX\\walk.wav");
    SetSoundVolume(FX_WALK, 75);
    LoadWAV(".\\SoundFX\\run.wav");
    SetSoundVolume(FX_RUN, 75);
    LoadWAV(".\\SoundFX\\enemydie.wav");
    LoadWAV(".\\SoundFX\\hajiburp.wav");
    LoadWAV(".\\SoundFX\\newlife.wav");
    LoadWAV(".\\SoundFX\\curryhit.wav");
    LoadWAV(".\\SoundFX\\hajithrow.wav");
}

void KillSoundFX(void)
{
    DeleteAllSounds();
}
