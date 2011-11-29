/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwplayer.h): player functions, haji stuff
 *
 */

#ifndef HWPLAYERH
#define HWPLAYERH

#define PLAYER_MIDY     192 // the midpoint on the Y-axis (where the camera moves, haji stops)
#define PLAYER_MIDX     304 // the midpoint on the X-axis (where the camera moves, haji stops)

#include "hwentity.h"

typedef struct _PLAYER {
    long                    score;
    int                 level;
    int                 power;
    int                 lives;
    int                 objectiles;
    int                 ognx, ogny;
    int                 absx, absy;
    int                 relx, rely;
    int                 velx, vely;
    int                 animindex;
    int                 count_skid;
    int                 count_stop;
    DIRECTION           direction;
    STATE                   state;
    STATE                   sub_state;
    STATE                   save_state;
    BOOL                    invulnerable;
    MOVEPOLY                *mpptr;
    POLY                    *pptr;
    int                 invcount;
} PLAYER;

typedef enum _PLAYER_WHOIS { WHOIS_HAJI, WHOIS_SUMI } PLAYER_WHOIS;

typedef struct _PLAYERNFO {
    char picfn[255];
    int pics;
    int width;
    int height;
    int aulen;
    int iulen;
    int uplen;
    int wlen;
    int invlen;
    int idlelen;
    int mvwx;
    int mvjx;
    int mvrx;
    int mvy;
    int anmdel;
    int skidwait;
    int i_stand;
    int i_walk_s;
    int i_walk_e;
    int i_skid_s;
    int i_skid_e;
    int i_duck_s;
    int i_duck_e;
    int i_run_s;
    int i_run_e;
    int i_jump1_s;
    int i_jump1_e;
    int i_jump2_s;
    int i_jump2_e;
    int i_idle1_s;
    int i_idle1_e;
    int i_idle2_s;
    int i_idle2_e;
    int i_idle3_s;
    int i_idle3_e;
    int i_die_s;
    int i_die_e;
    int i_t_stand_s;
    int i_t_stand_e;
    int i_t_walk_s;
    int i_t_walk_e;
    int i_t_run_s;
    int i_t_run_e;
    int i_t_duck_s;
    int i_t_duck_e;
    int i_t_jump_s;
    int i_t_jump_e;
    int ts_stand;
    int ts_walk;
    int ts_run;
    int ts_duck;
    int ts_jump;
} PLAYERNFO;

/*
 * Variables
 */
extern PLAYERNFO        *nfoPlayer;
extern int              g_NumPlayers;
extern PLAYER           player;
extern PLAYER_WHOIS player_whois;

void ReadPlayerNFO(void);
void DeletePlayerNFO(void);
void InitPlayer(void);
void RestartPlayer(void);
void KillPlayer(void);
void ProcessHaji(void);

#endif