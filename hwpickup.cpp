/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwpickup.cpp): Pickup functions
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <mmsystem.h>
#include <dsound.h>
#include "hwdsound.h"
#include "hwengine.h"
#include "hwpickup.h"
#include "hwworld.h"
#include "hwcollision.h"
#include "hwentity.h"
#include "hwenemy.h"
#include "hwplayer.h"
#include "hwsoundfx.h"
#include "hwlevel.h"

PICKUPNFO   *nfoPickup;
int g_NumPickups;

void ReadPickupNFO(void)
{
    ifstream file;
    char buf[256];
    int i = 0;
    
    file.open(".//Data//pickups.hwd", ios::in);
    file >> g_NumPickups;
    
    nfoPickup = (PICKUPNFO *)malloc(sizeof(PICKUPNFO) * g_NumPickups);
    
    file >> buf;
    
    while (file) {
        file >> buf;
        sprintf(nfoPickup[i].picfn, ".//Gfx//%s", buf);
        file >> nfoPickup[i].pics;
        file >> nfoPickup[i].width;
        file >> nfoPickup[i].height;
        file >> nfoPickup[i].anmfs;
        file >> nfoPickup[i].anmfe;
        file >> nfoPickup[i].anmdel;
        file >> nfoPickup[i].data;
        file >> buf;
        i++;
        
        file >> buf;
    }
    
    file.close();
}

void DeletePickupNFO(void)
{
    free(nfoPickup);
}

void BuildPickupList(void)
{
    int i;
    PICKUP *curPickup;
    
    curPickup = level.pickupLst;
    
    for (i = 0; i < level.num_pickups; i++) {
        if (level.pickups[i].state == ES_UNACTIVATED) {
            if (Collision(level.pickups[i].relx, level.pickups[i].rely, level.pickups[i].width, level.pickups[i].height, g_VOX, g_VOY, 645, 453)) {
                if (curPickup == NULL) {
                    level.pickupLst = (PICKUP *)&level.pickups[i];
                    curPickup = level.pickupLst;
                    curPickup->state = ES_ACTIVATED;
                } else {
                    while (curPickup->next != NULL) {
                        curPickup = curPickup->next;
                    }
                    curPickup->next = (PICKUP *)&level.pickups[i];
                    curPickup->next->prev = curPickup;
                    curPickup = curPickup->next;
                    curPickup->state = ES_ACTIVATED;
                }
            }
        }
    }
}

void ProcessPickupList(void)
{
    PICKUP *curPickup, *savPickup;
    
    curPickup = level.pickupLst;
    
    while (curPickup != NULL) {
        switch (curPickup->state) {
            case ES_ACTIVATED:
                curPickup->state = ES_STANDING;
                curPickup->absx = curPickup->relx - g_VOX;
                curPickup->absy = curPickup->rely - g_VOY + 32;
                break;
            case ES_STANDING:
                curPickup->absx = curPickup->relx - g_VOX;
                curPickup->absy = curPickup->rely - g_VOY + 32;
                break;
            case ES_DYING:
                curPickup->absx = curPickup->relx - g_VOX;
                curPickup->absy = curPickup->rely - g_VOY + 32;
                curPickup->trans -= 5;
                if (curPickup->trans < 0) {
                    savPickup = curPickup->next;
                    PickupDie(curPickup);
                    curPickup = savPickup;
                }
                break;
            case ES_DEAD:
                curPickup->absx = curPickup->relx - g_VOX;
                curPickup->absy = curPickup->rely - g_VOY + 32;
                break;
        }
        if (curPickup == NULL) {
            break;
        }
        curPickup->delay++;
        if (curPickup->delay > nfoPickup[curPickup->id].anmdel) {
            curPickup->delay = 0;
            curPickup->offset++;
            if (curPickup->offset > nfoPickup[curPickup->id].anmfe) {
                curPickup->offset = nfoPickup[curPickup->id].anmfs;
            }
        }
        curPickup->frame = curPickup->offset;
        curPickup = curPickup->next;
    }
}

void PerformPickup(PICKUP *pickup)
{
    switch (pickup->id) {
        case 0:
            PlaySound(FX_NEWLIFE, 0);
            player.lives += pickup->data;
            break;
        case 1:
        case 2:
            PlaySound(FX_HAJIBURP, 0);
            player.power += pickup->data;
            if (player.power > 100) {
                player.power = 100;
            }
            break;
        case 3:
        case 4:
            PlaySound(FX_WHOOSH, 0);
            player.score += pickup->data;
            break;
        case 5:
            PlaySound(FX_WHOOSH, 0);
            player.objectiles += pickup->data;
            break;
    }
}

void PickupDie(PICKUP *pickup)
{
    if (pickup->prev && pickup->next) {
        pickup->prev->next = pickup->next;
        pickup->next->prev = pickup->prev;
    } else if (pickup->prev && !pickup->next) {
        pickup->prev->next = NULL;
    } else if (!pickup->prev && pickup->next) {
        pickup->next->prev = NULL;
        level.pickupLst = pickup->next;
    } else {
        level.pickupLst = NULL;
    }
    
    pickup->next = NULL;
    pickup->prev = NULL;
    pickup->state = ES_DEAD;
}