/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwsoundfx.h): sound fx functions
 *
 */

#ifndef HWSOUNDFXH
#define HWSOUNDFXH

#define FX_XLOGO        0
#define FX_HAJIJUMP 1
#define FX_SKID     2
#define FX_WHOOSH       3
#define FX_OUCH     4
#define FX_WALK     5
#define FX_RUN          6
#define FX_ENEMYDIE 7
#define FX_HAJIBURP 8
#define FX_NEWLIFE  9
#define FX_CURRYHIT 10
#define FX_HAJITHROW    11

void LoadSoundFX(void);
void KillSoundFX(void);

#endif