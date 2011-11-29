/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwcollision.h): Collision Detection Functions
 *
 */

#ifndef HWCOLLISIONH
#define HWCOLLISIONH

int Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, int *right, int *left, int *top, int *bottom);
int Collision(int x2, int y2, int w2, int h2, int *right, int *left, int *top, int *bottom);
int Collision(int x1, int y1, int w1, int h1, POLY *poly2);
int Collision(int x1, int y1, int w1, int h1, POLY *poly2, int *right, int *left, int *top, int *bottom);
int Collision(POLY *poly2, int *right, int *left, int *top, int *bottom);

// collision player width's/height's
#define COLLISION_X     player.relx+(nfoPlayer[player_whois].width/3)
#define COLLISION_W     nfoPlayer[player_whois].width-((nfoPlayer[player_whois].width/3)*2)
#define COLLISION_Y     player.rely+(nfoPlayer[player_whois].height/10)
#define COLLISION_H     nfoPlayer[player_whois].height-(nfoPlayer[player_whois].height/10)
// ducking collision player width's/height's
#define COLLISION_DX        player.relx+(nfoPlayer[player_whois].width/3)
#define COLLISION_DW        nfoPlayer[player_whois].width-((nfoPlayer[player_whois].width/3)*2)
#define COLLISION_DY        player.rely+(nfoPlayer[player_whois].height/2)
#define COLLISION_DH        nfoPlayer[player_whois].height-(nfoPlayer[player_whois].height/2)
// jumping collision player width's/height's
#define COLLISION_JX        player.relx+(nfoPlayer[player_whois].width/3)
#define COLLISION_JW        nfoPlayer[player_whois].width-((nfoPlayer[player_whois].width/3)*2)
#define COLLISION_JY        player.rely+(nfoPlayer[player_whois].height/10)
#define COLLISION_JH        nfoPlayer[player_whois].height-(nfoPlayer[player_whois].height/2)

#endif