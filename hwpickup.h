/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwpickup.h): Pickup function interface
 *
 */

#ifndef HWPICKUPH
#define HWPICKUPH

#include "hwentity.h"

typedef struct _PICKUP {
    int         id;
    int         ognx, ogny;
    int         absx, absy;
    int         relx, rely;
    int         width, height;
    STATE           state;
    int         delay;
    int         offset;
    int         frame;
    int         data;
    int         trans;
    _PICKUP     *next;
    _PICKUP     *prev;
} PICKUP;

typedef struct _PICKUPNFO {
    char picfn[255];
    int pics;
    int width;
    int height;
    int anmfs;
    int anmfe;
    int anmdel;
    int data;
} PICKUPNFO;

/*
 * Variables
 */
extern PICKUPNFO    *nfoPickup;
extern int          g_NumPickups;

void ReadPickupNFO(void);
void DeletePickupNFO(void);
void BuildPickupList(void);
void ProcessPickupList(void);
void PerformPickup(PICKUP *pickup);
void PickupDie(PICKUP *pickup);

#endif