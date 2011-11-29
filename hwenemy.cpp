/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwenemy.h): enemy source file
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
#include "hwentity.h"
#include "hwdsound.h"
#include "hwworld.h"
#include "hwcollision.h"
#include "hwenemy.h"
#include "hwengine.h"
#include "hwpickup.h"
#include "hwlevel.h"

ENEMYNFO        *nfoEnemy;
int         g_NumEnemies;

void ReadEnemyNFO(void)
{
    ifstream file;
    char buf[256];
    int tmp;
    int i = 0;
    
    file.open(".//Data//enemies.hwd", ios::in);
    file >> g_NumEnemies;
    
    nfoEnemy = (ENEMYNFO *)malloc(sizeof(ENEMYNFO) * g_NumEnemies);
    
    file >> buf;
    
    while (file) {
        file >> buf;
        sprintf(nfoEnemy[i].picfn, ".//Gfx//%s", buf);
        file >> nfoEnemy[i].pics;
        file >> nfoEnemy[i].width;
        file >> nfoEnemy[i].height;
        file >> nfoEnemy[i].walkfs;
        file >> nfoEnemy[i].walkfe;
        file >> nfoEnemy[i].anmdel;
        file >> nfoEnemy[i].velx;
        file >> nfoEnemy[i].vely;
        file >> nfoEnemy[i].hitpts;
        file >> nfoEnemy[i].killpts;
        file >> tmp;
        if (tmp == 0) {
            nfoEnemy[i].passable = FALSE;
        } else {
            nfoEnemy[i].passable = TRUE;
        }
        file >> buf;
        i++;
        
        file >> buf;
    }
    
    file.close();
}

void DeleteEnemyNFO(void)
{
    free(nfoEnemy);
}

void BuildEnemyList(void)
{
    int i;
    ENEMY *curEnemy;
    
    curEnemy = level.enemyLst;
    
    for (i = 0; i < level.num_enemies; i++) {
        if (level.enemies[i].state == ES_UNACTIVATED) {
            if (Collision(level.enemies[i].relx, level.enemies[i].rely, level.enemies[i].width, level.enemies[i].height, g_VOX, g_VOY, 645, 453)) {
                if (curEnemy == NULL) {
                    level.enemyLst = (ENEMY *)&level.enemies[i];
                    curEnemy = level.enemyLst;
                    curEnemy->state = ES_ACTIVATED;
                } else {
                    while (curEnemy->next != NULL) {
                        curEnemy = curEnemy->next;
                    }
                    curEnemy->next = (ENEMY *)&level.enemies[i];
                    curEnemy->next->prev = curEnemy;
                    curEnemy = curEnemy->next;
                    curEnemy->state = ES_ACTIVATED;
                }
            }
        }
    }
}

void EnemyDie(ENEMY *enemy)
{
    if (enemy->prev && enemy->next) {
        enemy->prev->next = enemy->next;
        enemy->next->prev = enemy->prev;
    } else if (enemy->prev && !enemy->next) {
        enemy->prev->next = NULL;
    } else if (!enemy->prev && enemy->next) {
        enemy->next->prev = NULL;
        level.enemyLst = enemy->next;
    } else {
        level.enemyLst = NULL;
    }
    
    enemy->next = NULL;
    enemy->prev = NULL;
    enemy->state = ES_DEAD;
}

void EnemyCheckLeft(ENEMY *enemy)
{
    ENEMY *curEnemy;
    POLY *curPoly[4];
    int num_sectors;
    int i, right, left, top, bottom, collision;
    
    num_sectors = GetSectorList(curPoly, enemy->relx, enemy->rely, enemy->width, enemy->height);
    
    for (i = 0; i < num_sectors; i++) {
        if (curPoly[i]) {
            while (1) {
                if (!curPoly[i]->canmovethrough) {
                    collision = Collision(enemy->relx, enemy->rely, enemy->width, enemy->height, curPoly[i], &right, &left, &top, &bottom);
                    
                    if (left && !right && collision && (enemy->velx < 0)) {
                        enemy->relx += left;
                        enemy->direction = D_RIGHT;
                        enemy->velx = -enemy->velx;
                        i = num_sectors;
                        break;
                    }
                }
                if (curPoly[i]->next == NULL) {
                    break;
                }
                curPoly[i] = curPoly[i]->next;
            }
        }
    }
    
    if (enemy->data1 != -1) {
        if (enemy->relx < enemy->data1) {
            enemy->direction = D_RIGHT;
            enemy->velx = -enemy->velx;
        }
    }
    
    curEnemy = level.enemyLst;
    
    while (1) {
        if (curEnemy == enemy) {
            curEnemy = curEnemy->next;
            if (curEnemy == NULL) {
                break;
            }
        }
        if ((curEnemy->state != ES_DEAD) || (curEnemy->passable != TRUE)) {
            collision = Collision(enemy->relx, enemy->rely, enemy->width, enemy->height, curEnemy->relx, curEnemy->rely, curEnemy->width, curEnemy->height, &right, &left, &top, &bottom);
            
            if (collision && left && !right && (enemy->velx < 0)) {
                enemy->direction = D_RIGHT;
                enemy->velx = -enemy->velx;
                break;
            }
            
            curEnemy = curEnemy->next;
            if (curEnemy == NULL) {
                break;
            }
        }
    }
}

void EnemyCheckRight(ENEMY *enemy)
{
    ENEMY *curEnemy;
    POLY *curPoly[4];
    int num_sectors;
    int i, right, left, top, bottom, collision;
    
    num_sectors = GetSectorList(curPoly, enemy->relx, enemy->rely, enemy->width, enemy->height);
    
    for (i = 0; i < num_sectors; i++) {
        if (curPoly[i]) {
            while (1) {
                if (!curPoly[i]->canmovethrough) {
                    collision = Collision(enemy->relx, enemy->rely, enemy->width, enemy->height, curPoly[i], &right, &left, &top, &bottom);
                    
                    if (right && !left && collision && (enemy->velx > 0)) {
                        enemy->relx -= right;
                        enemy->direction = D_LEFT;
                        enemy->velx = -enemy->velx;
                        i = num_sectors;
                        break;
                    }
                }
                if (curPoly[i]->next == NULL) {
                    break;
                }
                curPoly[i] = curPoly[i]->next;
            }
        }
    }
    
    if (enemy->data1 != -1) {
        if (enemy->relx > enemy->data2) {
            enemy->direction = D_LEFT;
            enemy->velx = -enemy->velx;
        }
    }
    
    curEnemy = level.enemyLst;
    
    while (1) {
        if (curEnemy == enemy) {
            curEnemy = curEnemy->next;
            if (curEnemy == NULL) {
                break;
            }
        }
        if ((curEnemy->state != ES_DEAD) || (curEnemy->passable != TRUE)) {
            collision = Collision(enemy->relx, enemy->rely, enemy->width, enemy->height, curEnemy->relx, curEnemy->rely, curEnemy->width, curEnemy->height, &right, &left, &top, &bottom);
            
            if (collision && !left && right && (enemy->velx > 0)) {
                enemy->direction = D_LEFT;
                enemy->velx = -enemy->velx;
                break;
            }
            
            curEnemy = curEnemy->next;
            if (curEnemy == NULL) {
                break;
            }
        }
    }
}

void ProcessEnemyList(void)
{
    ENEMY *curEnemy, *savEnemy;
    
    curEnemy = level.enemyLst;
    
    if (curEnemy != NULL) {
        while (1) {
            switch (curEnemy->state) {
                case ES_ACTIVATED:
                    curEnemy->state = ES_PATROL;
                    if (curEnemy->direction == D_LEFT) {
                        curEnemy->velx = -nfoEnemy[curEnemy->id].velx;
                    } else {
                        curEnemy->velx = nfoEnemy[curEnemy->id].velx;
                    }
                    curEnemy->vely = 0;
                    curEnemy->absx = curEnemy->relx - g_VOX;
                    curEnemy->absy = curEnemy->rely - g_VOY + 32;
                    curEnemy->delay++;
                    if (curEnemy->delay > nfoEnemy[curEnemy->id].anmdel) {
                        curEnemy->delay = 0;
                        curEnemy->offset++;
                        if (curEnemy->offset > nfoEnemy[curEnemy->id].walkfe) {
                            curEnemy->offset = nfoEnemy[curEnemy->id].walkfs;
                        }
                    }
                    curEnemy->frame = curEnemy->offset;
                    if (curEnemy->direction == D_LEFT) {
                        curEnemy->frame += nfoEnemy[curEnemy->id].pics / 2;
                    }
                    break;
                case ES_PATROL:
                    if (curEnemy->direction == D_LEFT) {
                        EnemyCheckLeft(curEnemy);
                    } else {
                        EnemyCheckRight(curEnemy);
                    }
                    curEnemy->relx += curEnemy->velx;
                    curEnemy->absx = curEnemy->relx - g_VOX;
                    curEnemy->absy = curEnemy->rely - g_VOY + 32;
                    curEnemy->delay++;
                    if (curEnemy->delay > nfoEnemy[curEnemy->id].anmdel) {
                        curEnemy->delay = 0;
                        curEnemy->offset++;
                        if (curEnemy->offset > nfoEnemy[curEnemy->id].walkfe) {
                            curEnemy->offset = nfoEnemy[curEnemy->id].walkfs;
                        }
                    }
                    curEnemy->frame = curEnemy->offset;
                    if (curEnemy->direction == D_LEFT) {
                        curEnemy->frame += nfoEnemy[curEnemy->id].pics / 2;
                    }
                    break;
                case ES_DYING:
                    curEnemy->absx = curEnemy->relx - g_VOX;
                    curEnemy->absy = curEnemy->rely - g_VOY + 32;
                    curEnemy->delay++;
                    if (curEnemy->delay > nfoEnemy[curEnemy->id].anmdel) {
                        curEnemy->delay = 0;
                        curEnemy->offset++;
                        if (curEnemy->offset > nfoEnemy[curEnemy->id].walkfe + 3) {
                            savEnemy = curEnemy->next;
                            EnemyDie(curEnemy);
                            curEnemy = savEnemy;
                            if (curEnemy == NULL) {
                                break;
                            }
                        }
                    }
                    curEnemy->frame = curEnemy->offset;
                    if (curEnemy->direction == D_LEFT) {
                        curEnemy->frame += nfoEnemy[curEnemy->id].pics / 2;
                    }
                    break;
                case ES_DEAD:
                    curEnemy->absx = curEnemy->relx - g_VOX;
                    curEnemy->absy = curEnemy->rely - g_VOY + 32;
                    break;
            }
            
            if (curEnemy != NULL) {
                curEnemy = curEnemy->next;
            }
            if (curEnemy == NULL) {
                break;
            }
        }
    }
}