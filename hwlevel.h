#ifndef HWLEVELH
#define HWLEVELH

// a level containing a 2D array of sectors and level info
// and a linked list of foreground objects
// and a linked list of top objects
typedef struct _LEVEL {
    int     width;
    int     height;
    int     sectors_x;
    int     sectors_y;
    SECTOR  **sectors;
    int     num_movepoly;
    int     num_enemies;
    int     num_pickups;
    PICKUP  *pickupLst;
    PICKUP  *pickups;
    ENEMY       *enemyLst;
    ENEMY       *enemies;
    MOVEPOLY *movepolyLst;
    MOVEPOLY *movepoly;
    OBJECT  *foregroundLst;
    OBJECT  *topLst;
    GATE        gate;
} LEVEL;

extern LEVEL        level;                              // The structure containing level info

#endif