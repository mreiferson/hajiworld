/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwenemy.h): enemy include file
 *
 */

#ifndef HWENEMYH
#define HWENEMYH

typedef struct _ENEMY {
    int         id;
    int         power;
    int         ognx, ogny;
    int         absx, absy;
    int         relx, rely;
    int         velx, vely;
    int         width, height;
    STATE           state;
    DIRECTION   ogndir;
    DIRECTION   direction;
    int         frame;
    BOOL            passable;
    int         data1;
    int         data2;
    int         hitpts;
    int         killpts;
    int         delay;
    int         offset;
    MOVEPOLY        *mpptr;
    _ENEMY      *next;
    _ENEMY      *prev;
} ENEMY;

typedef struct _ENEMYNFO {
    char picfn[255];
    int pics;
    int width;
    int height;
    int walkfs, walkfe;
    int anmdel;
    int velx, vely;
    int hitpts;
    int killpts;
    bool passable;
} ENEMYNFO;

void ReadEnemyNFO(void);
void DeleteEnemyNFO(void);
void BuildEnemyList(void);
void EnemyDie(ENEMY *enemy);
void ProcessEnemyList(void);

extern ENEMYNFO *nfoEnemy;
extern int          g_NumEnemies;

#endif