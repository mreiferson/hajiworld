/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwobjectile.cpp): Objectile functions
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
#include "hwentity.h"
#include "hwworld.h"
#include "hwenemy.h"
#include "hwobjectile.h"
#include "hwcollision.h"
#include "hwpickup.h"
#include "hwlevel.h"
#include "hwplayer.h"
#include "hwsoundfx.h"

OBJECTILE       *objectiles = NULL;
OBJECTILENFO    *nfoObjectile;
int g_NumObjectiles;

void ReadObjectileNFO(void)
{
    ifstream file;
    char buf[256];
    int i = 0;
    
    file.open(".//Data//objectiles.hwd", ios::in);
    file >> g_NumObjectiles;
    
    nfoObjectile = (OBJECTILENFO *)malloc(sizeof(OBJECTILENFO) * g_NumObjectiles);
    
    file >> buf;
    
    while (file) {
        file >> buf;
        sprintf(nfoObjectile[i].picfn, ".//Gfx//%s", buf);
        file >> nfoObjectile[i].pics;
        file >> nfoObjectile[i].reg_s;
        file >> nfoObjectile[i].reg_e;
        file >> nfoObjectile[i].die_r_s;
        file >> nfoObjectile[i].die_r_e;
        file >> nfoObjectile[i].die_l_s;
        file >> nfoObjectile[i].die_l_e;
        file >> nfoObjectile[i].die_t_s;
        file >> nfoObjectile[i].die_t_e;
        file >> nfoObjectile[i].die_b_s;
        file >> nfoObjectile[i].die_b_e;
        file >> nfoObjectile[i].velx;
        file >> nfoObjectile[i].vely;
        file >> nfoObjectile[i].width;
        file >> nfoObjectile[i].height;
        file >> buf;
        i++;
        
        file >> buf;
    }
    
    file.close();
}

void DeleteObjectileNFO(void)
{
    free(nfoObjectile);
}

void AddObjectile(OWNER owner, OTYPE type, DIRECTION dir, int x, int y)
{
    OBJECTILE *curObjectile;
    
    curObjectile = objectiles;
    
    if (curObjectile == NULL) {
        objectiles = (OBJECTILE *)malloc(sizeof(OBJECTILE));
        curObjectile = objectiles;
        curObjectile->next = NULL;
        curObjectile->prev = NULL;
    } else {
        while (curObjectile->next != NULL) {
            curObjectile = curObjectile->next;
        }
        curObjectile->next = (OBJECTILE *)malloc(sizeof(OBJECTILE));
        curObjectile->next->prev = curObjectile;
        curObjectile = curObjectile->next;
        curObjectile->next = NULL;
    }
    
    curObjectile->dir = dir;
    curObjectile->owner = owner;
    curObjectile->type = type;
    curObjectile->relx = x;
    curObjectile->rely = y;
    curObjectile->velx = (dir == D_LEFT) ? -nfoObjectile[type].velx : nfoObjectile[type].velx;
    curObjectile->vely = nfoObjectile[type].vely;
    curObjectile->width = nfoObjectile[type].width;
    curObjectile->height = nfoObjectile[type].height;
    curObjectile->state = OS_INITIALIZED;
    curObjectile->frame = nfoObjectile[type].reg_s;
    curObjectile->a_offset = curObjectile->frame;
    curObjectile->a_delay = 0;
}

void RemoveObjectile(OBJECTILE *objectile)
{
    OBJECTILE *curObjectile;
    
    curObjectile = objectile;
    
    if (objectile->prev && objectile->next) {
        objectile->prev->next = objectile->next;
        objectile->next->prev = objectile->prev;
    } else if (objectile->prev && !objectile->next) {
        objectile->prev->next = NULL;
    } else if (!objectile->prev && objectile->next) {
        objectile->next->prev = NULL;
        objectiles = objectile->next;
    } else {
        objectiles = NULL;
    }
    
    objectile->next = NULL;
    objectile->prev = NULL;
    objectile->state = OS_DEAD;
    
    free(curObjectile);
}

void ObjectileCheck(OBJECTILE *objectile)
{
    POLY *curPoly[4];
    ENEMY *curEnemy;
    int collision;
    int num_sectors;
    int i, right, left, top, bottom;
    
    switch (objectile->owner) {
        case E_PLAYER:
            curEnemy = level.enemyLst;
            
            if (curEnemy != NULL) {
                while (1) {
                    if ((curEnemy->state != ES_DEAD) && (curEnemy->state != ES_DYING)) {
                        collision = Collision(objectile->relx, objectile->rely, objectile->width, objectile->height, curEnemy->relx, curEnemy->rely, curEnemy->width, curEnemy->height, &right, &left, &top, &bottom);
                        
                        if (collision) {
                            if (right && !left) {
                                objectile->state = OS_DIE_R;
                                objectile->a_offset = nfoObjectile[objectile->type].die_r_s;
                            } else if (left && !right) {
                                objectile->state = OS_DIE_L;
                                objectile->a_offset = nfoObjectile[objectile->type].die_l_s;
                            } else if (top && !bottom) {
                                objectile->state = OS_DIE_T;
                                objectile->a_offset = nfoObjectile[objectile->type].die_t_s;
                            } else if (bottom && !top) {
                                objectile->state = OS_DIE_B;
                                objectile->a_offset = nfoObjectile[objectile->type].die_b_s;
                            }
                            player.score += curEnemy->killpts;
                            curEnemy->state = ES_DYING;
                            curEnemy->offset = nfoEnemy[curEnemy->id].walkfe;
                            PlaySound(FX_CURRYHIT, 0);
                            PlaySound(FX_ENEMYDIE, 0);
                            return;
                        } else {
                            curEnemy = curEnemy->next;
                            if (curEnemy == NULL) {
                                break;
                            }
                        }
                    } else {
                        curEnemy = curEnemy->next;
                        if (curEnemy == NULL) {
                            break;
                        }
                    }
                }
            }
            
            if ((objectile->relx + objectile->width) <= 0) {
                objectile->state = OS_DIE_L;
                objectile->a_offset = nfoObjectile[objectile->type].die_l_s;
                return;
            }
            
            if (objectile->relx >= level.width) {
                objectile->state = OS_DIE_R;
                objectile->a_offset = nfoObjectile[objectile->type].die_r_s;
                return;
            }
            
            if (objectile->rely >= level.height) {
                objectile->state = OS_DIE_B;
                objectile->a_offset = nfoObjectile[objectile->type].die_b_s;
                return;
            }
            
            num_sectors = GetSectorList(curPoly, objectile->relx, objectile->rely, objectile->width, objectile->height);
            
            for (i = 0; i < num_sectors; i++) {
                if (curPoly[i]) {
                    while (1) {
                        collision = Collision(objectile->relx, objectile->rely, objectile->width, objectile->height, curPoly[i], &right, &left, &top, &bottom);
                        
                        if (right && !left && collision && !curPoly[i]->canmovethrough) {
                            objectile->state = OS_DIE_R;
                            objectile->a_offset = nfoObjectile[objectile->type].die_r_s;
                            i = num_sectors;
                            PlaySound(FX_CURRYHIT, 0);
                            return;
                        } else if (left && !right && collision && !curPoly[i]->canmovethrough) {
                            objectile->state = OS_DIE_L;
                            objectile->a_offset = nfoObjectile[objectile->type].die_l_s;
                            i = num_sectors;
                            PlaySound(FX_CURRYHIT, 0);
                            return;
                        }
                        
                        if (top && !bottom && collision && !curPoly[i]->canmovethrough) {
                            objectile->state = OS_DIE_T;
                            objectile->a_offset = nfoObjectile[objectile->type].die_t_s;
                            i = num_sectors;
                            PlaySound(FX_CURRYHIT, 0);
                            return;
                        } else if (bottom && !top && collision) {
                            objectile->state = OS_DIE_B;
                            objectile->a_offset = nfoObjectile[objectile->type].die_b_s;
                            i = num_sectors;
                            PlaySound(FX_CURRYHIT, 0);
                            return;
                        }
                        
                        if (curPoly[i]->next == NULL) {
                            break;
                        }
                        curPoly[i] = curPoly[i]->next;
                    }
                }
            }
            break;
        case E_ENEMY:
            break;
    }
}

void ProcessObjectileList(void)
{
    OBJECTILE *curObjectile, *savObjectile;
    
    curObjectile = objectiles;
    
    if (curObjectile != NULL) {
        while (1) {
            switch (curObjectile->state) {
                case OS_INITIALIZED:
                    switch (curObjectile->type) {
                        case OT_FALLING:
                            curObjectile->state = OS_FALLING;
                            break;
                        case OT_ARCING:
                            curObjectile->state = OS_ARCING;
                            break;
                        case OT_MOVELEVEL:
                            curObjectile->state = OS_MOVELEVEL;
                            break;
                    }
                    break;
                case OS_FALLING:
                case OS_ARCING:
                    curObjectile->vely++;
                    curObjectile->relx += curObjectile->velx;
                    curObjectile->rely += curObjectile->vely;
                    curObjectile->a_delay++;
                    if (curObjectile->a_delay > 1) {
                        curObjectile->a_delay = 0;
                        curObjectile->a_offset++;
                        if (curObjectile->a_offset > nfoObjectile[curObjectile->type].reg_e) {
                            curObjectile->a_offset = nfoObjectile[curObjectile->type].reg_s;
                        }
                    }
                    curObjectile->frame = curObjectile->a_offset;
                    ObjectileCheck(curObjectile);
                    break;
                case OS_MOVELEVEL:
                    curObjectile->relx += curObjectile->velx;
                    curObjectile->rely += curObjectile->vely;
                    ObjectileCheck(curObjectile);
                    break;
                case OS_DIE_R:
                    curObjectile->a_delay++;
                    if (curObjectile->a_delay > 1) {
                        curObjectile->a_delay = 0;
                        curObjectile->a_offset++;
                        if (curObjectile->a_offset > nfoObjectile[curObjectile->type].die_r_e) {
                            curObjectile->a_offset--;
                            curObjectile->state = OS_DEAD;
                        }
                    }
                    curObjectile->frame = curObjectile->a_offset;
                    break;
                case OS_DIE_L:
                    curObjectile->a_delay++;
                    if (curObjectile->a_delay > 1) {
                        curObjectile->a_delay = 0;
                        curObjectile->a_offset++;
                        if (curObjectile->a_offset > nfoObjectile[curObjectile->type].die_l_e) {
                            curObjectile->a_offset--;
                            curObjectile->state = OS_DEAD;
                        }
                    }
                    curObjectile->frame = curObjectile->a_offset;
                    break;
                case OS_DIE_T:
                    curObjectile->a_delay++;
                    if (curObjectile->a_delay > 1) {
                        curObjectile->a_delay = 0;
                        curObjectile->a_offset++;
                        if (curObjectile->a_offset > nfoObjectile[curObjectile->type].die_t_e) {
                            curObjectile->a_offset--;
                            curObjectile->state = OS_DEAD;
                        }
                    }
                    curObjectile->frame = curObjectile->a_offset;
                    break;
                case OS_DIE_B:
                    curObjectile->a_delay++;
                    if (curObjectile->a_delay > 1) {
                        curObjectile->a_delay = 0;
                        curObjectile->a_offset++;
                        if (curObjectile->a_offset > nfoObjectile[curObjectile->type].die_b_e) {
                            curObjectile->a_offset--;
                            curObjectile->state = OS_DEAD;
                        }
                    }
                    curObjectile->frame = curObjectile->a_offset;
                    break;
                case OS_DEAD:
                    savObjectile = curObjectile->next;
                    RemoveObjectile(curObjectile);
                    curObjectile = savObjectile;
                    break;
            }
            
            if (curObjectile != NULL) {
                curObjectile = curObjectile->next;
            }
            if (curObjectile == NULL) {
                break;
            }
        }
    }
}