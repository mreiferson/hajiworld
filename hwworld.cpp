/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwworld.cpp): World data, loading, initializing
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <ddraw.h>
#include <digifx.h>
#include "hwworld.h"
#include "hwcollision.h"
#include "hwplayer.h"
#include "hwgfx.h"
#include "hwentity.h"
#include "hwenemy.h"
#include "hwengine.h"
#include "hwpickup.h"
#include "hwlevel.h"

LEVEL       level;                      // The structure containing level info
int     g_VOX = 0,          // The x offset for the current viewport
        g_VOY = 0;          // the y offset for the current viewport
int     g_VOX_origin, g_VOY_origin;
char        level_file[80];

OBJECTNFO *nfoObject;
int g_NumObjects;

void InitLevel(int levelnum)
{
    POLY tmpPoly;
    OBJECT *curObjectA, *curObjectB;
    POLY *curPoly;
    ifstream file;
    char buf[256];
    char fn[256];
    int i, j, sx, sy, tmp, ecount = 0, mpcount = 0, pcount = 0, polycount = 0;
    int id, layer, x, y, mpnum;
    int x1, x2, y1, y2, w, h;
    
    if (!strcmp(level_file, "")) {
        sprintf(fn, ".\\Levels\\level%d.lev", levelnum);
        file.open(fn, ios::in);
    } else {
        sprintf(fn, ".\\Levels\\%s", level_file);
        file.open(fn, ios::in);
    }
    
    while (file) {
        file >> buf;
        if (!strcmp("<INFO>", buf)) {
            file >> level.width;
            file >> level.height;
            file >> level.sectors_x;
            file >> level.sectors_y;
            level.sectors = (SECTOR **)malloc(sizeof(SECTOR) * level.sectors_y);
            for (i = 0; i < level.sectors_y; i++) {
                level.sectors[i] = (SECTOR *)malloc(sizeof(SECTOR) * level.sectors_x);
            }
            for (i = 0; i < level.sectors_y; i++)
                for (j = 0; j < level.sectors_x; j++) {
                    level.sectors[i][j].polyLst = NULL;
                }
            file >> player.ognx;
            file >> player.ogny;
            player.ogny -= nfoPlayer[player_whois].height;
            player.relx = player.ognx;
            player.rely = player.ogny;
            file >> g_VOX;
            file >> g_VOY;
            g_VOX_origin = g_VOX;
            g_VOY_origin = g_VOY;
            file >> level.num_enemies;
            if (level.num_enemies != 0) {
                level.enemies = (ENEMY *)malloc(sizeof(ENEMY) * level.num_enemies);
            } else {
                level.enemies = NULL;
            }
            level.enemyLst = NULL;
            file >> level.num_movepoly;
            if (level.num_movepoly != 0) {
                level.movepoly = (MOVEPOLY *)malloc(sizeof(MOVEPOLY) * level.num_movepoly);
            } else {
                level.movepoly = NULL;
            }
            level.movepolyLst = NULL;
            file >> level.num_pickups;
            if (level.num_pickups != 0) {
                level.pickups = (PICKUP *)malloc(sizeof(PICKUP) * level.num_pickups);
            } else {
                level.pickups = NULL;
            }
            level.pickupLst = NULL;
            file >> buf;
        } else if (!strcmp("<GATE>", buf)) {
            file >> x1;
            file >> y1;
            file >> x2;
            file >> y2;
            w = x2 - x1;
            h = y2 - y1;
            level.gate.verts[0].x = x1;
            level.gate.verts[0].y = y1;
            level.gate.verts[1].x = x1 + w;
            level.gate.verts[1].y = y1;
            level.gate.verts[2].x = x1 + w;
            level.gate.verts[2].y = y1 + h;
            level.gate.verts[3].x = x1;
            level.gate.verts[3].y = y1 + h;
            file >> buf;
        } else if (!strcmp("<ENEMY>", buf)) {
            file >> level.enemies[ecount].id;
            file >> level.enemies[ecount].ognx;
            file >> level.enemies[ecount].ogny;
            level.enemies[ecount].relx = level.enemies[ecount].ognx;
            level.enemies[ecount].rely = level.enemies[ecount].ogny;
            level.enemies[ecount].absx = -1;
            level.enemies[ecount].absy = -1;
            level.enemies[ecount].width = nfoEnemy[level.enemies[ecount].id].width;
            level.enemies[ecount].height = nfoEnemy[level.enemies[ecount].id].height;
            file >> tmp;
            if (tmp == 0) {
                level.enemies[ecount].direction = D_LEFT;
            } else {
                level.enemies[ecount].direction = D_RIGHT;
            }
            level.enemies[ecount].ogndir = level.enemies[ecount].direction;
            level.enemies[ecount].passable = nfoEnemy[level.enemies[ecount].id].passable;
            level.enemies[ecount].killpts = nfoEnemy[level.enemies[ecount].id].killpts;
            level.enemies[ecount].hitpts = nfoEnemy[level.enemies[ecount].id].hitpts;
            file >> level.enemies[ecount].data1;
            file >> level.enemies[ecount].data2;
            level.enemies[ecount].power = 100;
            level.enemies[ecount].next = NULL;
            level.enemies[ecount].prev = NULL;
            level.enemies[ecount].delay = 0;
            level.enemies[ecount].offset = 0;
            level.enemies[ecount].frame = 0;
            level.enemies[ecount].state = ES_UNACTIVATED;
            ecount++;
            file >> buf;
        } else if (!strcmp("<OBJECT>", buf)) {
            file >> id;
            file >> layer;
            file >> x;
            file >> y;
            file >> mpnum;
            
            switch (layer) {
                case 0:
                    if (level.foregroundLst == NULL) {
                        level.foregroundLst = (OBJECT *)malloc(sizeof(OBJECT));
                        curObjectA = level.foregroundLst;
                    } else {
                        curObjectA->next = (OBJECT *)malloc(sizeof(OBJECT));
                        curObjectA = curObjectA->next;
                    }
                    curObjectA->id = id;
                    curObjectA->x = x;
                    curObjectA->y = y;
                    curObjectA->mpnum = mpnum;
                    curObjectA->frame = 0;
                    curObjectA->count = 0;
                    if (curObjectA->mpnum != -1) {
                        level.movepoly[curObjectA->mpnum].ascObject = curObjectA;
                    }
                    curObjectA->next = NULL;
                    break;
                case 1:
                    if (level.topLst == NULL) {
                        level.topLst = (OBJECT *)malloc(sizeof(OBJECT));
                        curObjectB = level.topLst;
                    } else {
                        curObjectB->next = (OBJECT *)malloc(sizeof(OBJECT));
                        curObjectB = curObjectB->next;
                    }
                    curObjectB->id = id;
                    curObjectB->x = x;
                    curObjectB->y = y;
                    curObjectB->mpnum = mpnum;
                    curObjectB->frame = 0;
                    curObjectB->count = 0;
                    curObjectB->next = NULL;
                    break;
            }
            file >> buf;
        } else if (!strcmp("<POLY>", buf)) {
            tmpPoly.id = polycount;
            polycount++;
            file >> tmpPoly.num_verts;
            tmpPoly.verts = (VERT *)malloc(sizeof(VERT) * tmpPoly.num_verts);
            for (i = 0; i < tmpPoly.num_verts; i++) {
                file >> tmpPoly.verts[i].x;
                file >> tmpPoly.verts[i].y;
            }
            file >> tmp;
            if (tmp == 0) {
                tmpPoly.canwalkon = FALSE;
            } else {
                tmpPoly.canwalkon = TRUE;
            }
            file >> tmp;
            if (tmp == 0) {
                tmpPoly.canmovethrough = FALSE;
            } else {
                tmpPoly.canmovethrough = TRUE;
            }
            file >> tmp;
            if (tmp == 0) {
                tmpPoly.canjumpdown = FALSE;
            } else {
                tmpPoly.canjumpdown = TRUE;
            }
            tmpPoly.next = NULL;
            
            for (sy = 0; sy < level.sectors_y; sy++) {
                for (sx = 0; sx < level.sectors_x; sx++) {
                    if (Collision(sx * 640, sy * 448, 639, 447, &tmpPoly)) {
                        if (level.sectors[sy][sx].polyLst == NULL) {
                            level.sectors[sy][sx].polyLst = (POLY *)malloc(sizeof(POLY));
                            curPoly = level.sectors[sy][sx].polyLst;
                            curPoly->id = tmpPoly.id;
                            curPoly->num_verts = tmpPoly.num_verts;
                            curPoly->verts = (VERT *)malloc(sizeof(VERT) * curPoly->num_verts);
                            for (i = 0; i < curPoly->num_verts; i++) {
                                curPoly->verts[i].x = tmpPoly.verts[i].x;
                                curPoly->verts[i].y = tmpPoly.verts[i].y;
                            }
                            curPoly->canwalkon = tmpPoly.canwalkon;
                            curPoly->canmovethrough = tmpPoly.canmovethrough;
                            curPoly->canjumpdown = tmpPoly.canjumpdown;
                            curPoly->next = NULL;
                        } else {
                            curPoly = level.sectors[sy][sx].polyLst;
                            while (curPoly->next != NULL) {
                                curPoly = curPoly->next;
                            }
                            curPoly->next = (POLY *)malloc(sizeof(POLY));
                            curPoly = curPoly->next;
                            curPoly->id = tmpPoly.id;
                            curPoly->num_verts = tmpPoly.num_verts;
                            curPoly->verts = (VERT *)malloc(sizeof(VERT) * curPoly->num_verts);
                            for (i = 0; i < curPoly->num_verts; i++) {
                                curPoly->verts[i].x = tmpPoly.verts[i].x;
                                curPoly->verts[i].y = tmpPoly.verts[i].y;
                            }
                            curPoly->canwalkon = tmpPoly.canwalkon;
                            curPoly->canmovethrough = tmpPoly.canmovethrough;
                            curPoly->canjumpdown = tmpPoly.canjumpdown;
                            curPoly->next = NULL;
                        }
                    }
                }
            }
            free(tmpPoly.verts);
            
            file >> buf;
        } else if (!strcmp("<MOVEPOLY>", buf)) {
            file >> level.movepoly[mpcount].ognx;
            file >> level.movepoly[mpcount].ogny;
            level.movepoly[mpcount].x = level.movepoly[mpcount].ognx;
            level.movepoly[mpcount].y = level.movepoly[mpcount].ogny;
            file >> level.movepoly[mpcount].w;
            file >> level.movepoly[mpcount].h;
            file >> tmp;
            if (tmp == 0) {
                level.movepoly[mpcount].type = MPT_HORIZONTAL;
            } else if (tmp == 1) {
                level.movepoly[mpcount].type = MPT_VERTICAL;
            } else if (tmp == 2) {
                level.movepoly[mpcount].type = MPT_FALLING;
            }
            level.movepoly[mpcount].state = MPS_UNACTIVATED;
            file >> tmp;
            if (tmp == 0) {
                level.movepoly[mpcount].canwalkon = FALSE;
            } else {
                level.movepoly[mpcount].canwalkon = TRUE;
            }
            file >> tmp;
            if (tmp == 0) {
                level.movepoly[mpcount].canmovethrough = FALSE;
            } else {
                level.movepoly[mpcount].canmovethrough = TRUE;
            }
            file >> level.movepoly[mpcount].velx;
            file >> level.movepoly[mpcount].vely;
            file >> level.movepoly[mpcount].data1;
            file >> level.movepoly[mpcount].data2;
            level.movepoly[mpcount].ascObject = NULL;
            level.movepoly[mpcount].next = NULL;
            level.movepoly[mpcount].prev = NULL;
            mpcount++;
            file >> buf;
        } else if (!strcmp("<PICKUP>", buf)) {
            file >> level.pickups[pcount].id;
            file >> level.pickups[pcount].ognx;
            file >> level.pickups[pcount].ogny;
            level.pickups[pcount].relx = level.pickups[pcount].ognx;
            level.pickups[pcount].rely = level.pickups[pcount].ogny;
            level.pickups[pcount].absx = -1;
            level.pickups[pcount].absy = -1;
            level.pickups[pcount].width = nfoPickup[level.pickups[pcount].id].width;
            level.pickups[pcount].height = nfoPickup[level.pickups[pcount].id].height;
            level.pickups[pcount].data = nfoPickup[level.pickups[pcount].id].data;
            level.pickups[pcount].frame = 0;
            level.pickups[pcount].delay = 0;
            level.pickups[pcount].offset = 0;
            level.pickups[pcount].state = ES_UNACTIVATED;
            level.pickups[pcount].next = NULL;
            level.pickups[pcount].prev = NULL;
            pcount++;
            file >> buf;
        }
    }
    
    file.close();
}

void KillLevel(void)
{
    POLY *curPoly, *nexPoly;
    OBJECT *curObject, *nexObject;
    int i, j;
    
    free(level.pickups);
    level.pickupLst = NULL;
    free(level.enemies);
    level.enemyLst = NULL;
    free(level.movepoly);
    level.movepolyLst = NULL;
    
    for (i = 0; i < level.sectors_y; i++) {
        for (j = 0; j < level.sectors_y; j++) {
            curPoly = level.sectors[i][j].polyLst;
            if (curPoly) {
                while (curPoly->next != NULL) {
                    free(curPoly->verts);
                    curPoly = curPoly->next;
                }
                free(curPoly->verts);
                
                curPoly = level.sectors[i][j].polyLst;
                while (curPoly->next != NULL) {
                    nexPoly = curPoly->next;
                    free(curPoly);
                    curPoly = nexPoly;
                }
                free(curPoly);
                level.sectors[i][j].polyLst = NULL;
            }
        }
        free(level.sectors[i]);
    }
    free(level.sectors);
    
    curObject = level.foregroundLst;
    if (curObject) {
        while (curObject->next != NULL) {
            nexObject = curObject->next;
            free(curObject);
            curObject = nexObject;
        }
        free(curObject);
    }
    level.foregroundLst = NULL;
    
    curObject = level.topLst;
    if (curObject) {
        while (curObject->next != NULL) {
            nexObject = curObject->next;
            free(curObject);
            curObject = nexObject;
        }
        free(curObject);
    }
    level.topLst = NULL;
}

void ReadObjectNFO(void)
{
    ifstream file;
    char buf[256];
    int i = 0;
    
    file.open(".//Data//objects.hwd", ios::in);
    file >> g_NumObjects;
    
    nfoObject = (OBJECTNFO *)malloc(sizeof(OBJECTNFO) * g_NumObjects);
    
    file >> buf;
    
    while (file) {
        file >> buf;
        sprintf(nfoObject[i].picfn, ".//Gfx//%s", buf);
        file >> nfoObject[i].pics;
        file >> nfoObject[i].width;
        file >> nfoObject[i].height;
        file >> nfoObject[i].anmdel;
        file >> nfoObject[i].trans;
        file >> buf;
        i++;
        
        file >> buf;
    }
    
    file.close();
}

void DeleteObjectNFO(void)
{
    free(nfoObject);
}

int GetSectorList(POLY **curPoly, int x, int y, int w, int h)
{
    int num_sectors = 0;
    int which;
    int sx, sy;
    
    if (x < 0) {
        x = 0;
    }
    
    if (x > (level.width - w - 1)) {
        x = level.width - w - 1;
    }
    
    if (y < 0) {
        y = 0;
    }
    
    if (y > (level.height - h - 1)) {
        y = level.height - h - 1;
    }
    
    // determine the number of sectors haji is in
    if ((x % SECTOR_W) > ((x + w) % SECTOR_W)) {
        // we're on an x boundry
        num_sectors += 2;
        which = 0; // set the which variable to X (0)
    }
    if ((y % SECTOR_H) > ((y + h) % SECTOR_H)) {
        // we're on a y boundry
        num_sectors += 2;
        which = 1; // set the which variable to Y (1)
    }
    if (!num_sectors) { // no boundaries, just in one sector
        num_sectors = 1;
    }
    
    // link the curPoly list to the sectors we're in
    switch (num_sectors) {
        case 1: // we're in only one sector
            sx = x / SECTOR_W;
            sy = y / SECTOR_H;
            curPoly[0] = level.sectors[sy][sx].polyLst;
            break;
        case 2: // we're in 2 sectors
            if (!which) { // which 2 sectors (x or y boundry)?
                // we're in 2 sectors on a X boundry
                sx = x / SECTOR_W;
                sy = y / SECTOR_H;
                curPoly[0] = level.sectors[sy][sx].polyLst;
                sx++; // increment the x sector #
                curPoly[1] = level.sectors[sy][sx].polyLst;
            } else {
                // we're in 2 sectors on a Y boundry
                sx = x / SECTOR_W;
                sy = y / SECTOR_H;
                curPoly[0] = level.sectors[sy][sx].polyLst;
                sy++; // increment the y sector #
                curPoly[1] = level.sectors[sy][sx].polyLst;
            }
            break;
        case 4: // we're in 4 sectors (a cross-section)
            sx = x / SECTOR_W;
            sy = y / SECTOR_H;
            curPoly[0] = level.sectors[sy][sx].polyLst;
            sx++; // increment the x sector #
            curPoly[1] = level.sectors[sy][sx].polyLst;
            sy++; // increment the y sector #
            curPoly[2] = level.sectors[sy][sx].polyLst;
            sx--; // decrement the x sector #
            curPoly[3] = level.sectors[sy][sx].polyLst;
            break;
    }
    
    // return the # of sectors we found
    return num_sectors;
}

void BuildMovePolyList(void)
{
    int i;
    MOVEPOLY *curMovePoly;
    
    curMovePoly = level.movepolyLst;
    
    for (i = 0; i < level.num_movepoly; i++) {
        if (level.movepoly[i].state == MPS_UNACTIVATED) {
            if (Collision(level.movepoly[i].x, level.movepoly[i].y, level.movepoly[i].w, level.movepoly[i].h, g_VOX, g_VOY, 645, 453)) {
                if (curMovePoly == NULL) {
                    level.movepolyLst = (MOVEPOLY *)&level.movepoly[i];
                    curMovePoly = level.movepolyLst;
                    curMovePoly->state = MPS_ACTIVATED;
                } else {
                    while (curMovePoly->next != NULL) {
                        curMovePoly = curMovePoly->next;
                    }
                    curMovePoly->next = (MOVEPOLY *)&level.movepoly[i];
                    curMovePoly->next->prev = curMovePoly;
                    curMovePoly = curMovePoly->next;
                    curMovePoly->state = MPS_ACTIVATED;
                }
            }
        }
    }
}

void ProcessMovePolyList(void)
{
    MOVEPOLY *curMovePoly;
    
    curMovePoly = level.movepolyLst;
    
    if (curMovePoly != NULL) {
        while (1) {
            switch (curMovePoly->type) {
                case MPT_HORIZONTAL:
                    switch (curMovePoly->state) {
                        case MPS_ACTIVATED:
                            if (curMovePoly->velx > 0) {
                                curMovePoly->state = MPS_MOVINGRIGHT;
                            } else {
                                curMovePoly->state = MPS_MOVINGLEFT;
                            }
                            break;
                        case MPS_MOVINGLEFT:
                            if (curMovePoly->x <= curMovePoly->data1) {
                                curMovePoly->state = MPS_MOVINGRIGHT;
                                curMovePoly->velx = -curMovePoly->velx;
                            }
                            curMovePoly->x += curMovePoly->velx;
                            curMovePoly->ascObject->x += curMovePoly->velx;
                            break;
                        case MPS_MOVINGRIGHT:
                            if (curMovePoly->x >= curMovePoly->data2) {
                                curMovePoly->state = MPS_MOVINGLEFT;
                                curMovePoly->velx = -curMovePoly->velx;
                            }
                            curMovePoly->x += curMovePoly->velx;
                            curMovePoly->ascObject->x += curMovePoly->velx;
                            break;
                    }
                    break;
                case MPT_VERTICAL:
                    switch (curMovePoly->state) {
                        case MPS_ACTIVATED:
                            if (curMovePoly->vely > 0) {
                                curMovePoly->state = MPS_MOVINGDOWN;
                            } else {
                                curMovePoly->state = MPS_MOVINGUP;
                            }
                            break;
                        case MPS_MOVINGUP:
                            if (curMovePoly->y <= curMovePoly->data1) {
                                curMovePoly->state = MPS_MOVINGDOWN;
                                curMovePoly->vely = -curMovePoly->vely;
                            }
                            curMovePoly->y += curMovePoly->vely;
                            curMovePoly->ascObject->y += curMovePoly->vely;
                            break;
                        case MPS_MOVINGDOWN:
                            if (curMovePoly->y >= curMovePoly->data2) {
                                curMovePoly->state = MPS_MOVINGUP;
                                curMovePoly->vely = -curMovePoly->vely;
                            }
                            curMovePoly->y += curMovePoly->vely;
                            curMovePoly->ascObject->y += curMovePoly->vely;
                            break;
                    }
                    break;
                case MPT_FALLING:
                    switch (curMovePoly->state) {
                        case MPS_ACTIVATED:
                            curMovePoly->data2 = 0;
                            curMovePoly->vely = 0;
                            curMovePoly->state = MPS_NOTFALLING;
                            break;
                        case MPS_NOTFALLING:
                            if (player.mpptr == curMovePoly) {
                                curMovePoly->state = MPS_ABOUTTOFALL;
                            }
                            break;
                        case MPS_ABOUTTOFALL:
                            curMovePoly->data2++;
                            if (curMovePoly->data2 > curMovePoly->data1) {
                                if (player.mpptr == curMovePoly) {
                                    player.state = ES_JUMPINGDOWN;
                                    player.mpptr = NULL;
                                }
                                curMovePoly->state = MPS_FALLING;
                            }
                            break;
                        case MPS_FALLING:
                            curMovePoly->vely++;
                            curMovePoly->y += curMovePoly->vely;
                            curMovePoly->ascObject->y += curMovePoly->vely;
                            if (curMovePoly->y > level.height) {
                                curMovePoly->state = MPS_DONEFALLING;
                            }
                            break;
                    }
                    break;
            }
            curMovePoly = curMovePoly->next;
            if (curMovePoly == NULL) {
                break;
            }
        }
    }
}

void ProcessObjects(void)
{
    OBJECT *curObject;
    
    curObject = level.foregroundLst;
    
    if (curObject != NULL) {
        while (1) {
            curObject->count++;
            if (curObject->count > nfoObject[curObject->id].anmdel) {
                curObject->count = 0;
                curObject->frame++;
                if (curObject->frame >= nfoObject[curObject->id].pics) {
                    curObject->frame = 0;
                }
            }
            
            curObject = curObject->next;
            if (curObject == NULL) {
                break;
            }
        }
    }
    
    curObject = level.topLst;
    
    if (curObject != NULL) {
        while (1) {
            curObject->count++;
            if (curObject->count > nfoObject[curObject->id].anmdel) {
                curObject->count = 0;
                curObject->frame++;
                if (curObject->frame >= nfoObject[curObject->id].pics) {
                    curObject->frame = 0;
                }
            }
            
            curObject = curObject->next;
            if (curObject == NULL) {
                break;
            }
        }
    }
}
