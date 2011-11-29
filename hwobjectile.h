/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwobjectile.h): Objectile functions header
 *
 */

#ifndef HWOBJECTILEH
#define HWOBJECTILEH

typedef enum _OWNER     { E_PLAYER, E_ENEMY } OWNER;

typedef enum _OTYPE     { OT_FALLING, OT_ARCING, OT_MOVELEVEL } OTYPE;

typedef enum _OSTATE        { OS_INITIALIZED, OS_FALLING, OS_ARCING, OS_MOVELEVEL, OS_DIE_R, OS_DIE_L, OS_DIE_T, OS_DIE_B, OS_DEAD } OSTATE;

typedef struct _OBJECTILE {
    OWNER           owner;
    int         velx;
    int         vely;
    int         relx;
    int         rely;
    int         width;
    int         height;
    OTYPE           type;
    OSTATE      state;
    DIRECTION   dir;
    int         frame;
    int         a_delay;
    int         a_offset;
    _OBJECTILE  *next;
    _OBJECTILE  *prev;
} OBJECTILE;

typedef struct _OBJECTILENFO {
    char picfn[255];
    int pics;
    int reg_s;
    int reg_e;
    int die_r_s;
    int die_r_e;
    int die_l_s;
    int die_l_e;
    int die_t_s;
    int die_t_e;
    int die_b_s;
    int die_b_e;
    int velx;
    int vely;
    int width;
    int height;
} OBJECTILENFO;

/*
 * Variables
 */
extern OBJECTILENFO *nfoObjectile;
extern OBJECTILE        *objectiles;
extern int              g_NumObjectiles;

void ReadObjectileNFO(void);
void DeleteObjectileNFO(void);
void AddObjectile(OWNER owner, OTYPE type, DIRECTION dir, int x, int y);
void RemoveObjectile(OBJECTILE *objectile);
void ProcessObjectileList(void);

#endif