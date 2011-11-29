/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwplayer.cpp): player functions, haji stuff
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <digifx.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>
#include "hajiworld.h"
#include "hwdsound.h"
#include "hwddraw.h"
#include "hwworld.h"
#include "hwplayer.h"
#include "hwgfx.h"
#include "hwcollision.h"
#include "hwentity.h"
#include "hwobjectile.h"
#include "hwenemy.h"
#include "hwengine.h"
#include "hwinput.h"
#include "hwpickup.h"
#include "hwsoundfx.h"
#include "hwlevel.h"
#include "hwexplosion.h"
#include "hwutil.h"

PLAYERNFO *nfoPlayer;
int g_NumPlayers;
PLAYER player;
PLAYER_WHOIS player_whois;

void ReadPlayerNFO(void)
{
    ifstream file;
    char buf[256];
    int i = 0;
    
    file.open(".//Data//players.hwd", ios::in);
    file >> g_NumPlayers;
    
    nfoPlayer = (PLAYERNFO *)malloc(sizeof(PLAYERNFO) * g_NumPlayers);
    
    file >> buf;
    
    while (file) {
        file >> buf;
        sprintf(nfoPlayer[i].picfn, ".//Gfx//%s", buf);
        file >> nfoPlayer[i].pics;
        file >> nfoPlayer[i].width;
        file >> nfoPlayer[i].height;
        file >> nfoPlayer[i].aulen;
        file >> nfoPlayer[i].iulen;
        file >> nfoPlayer[i].uplen;
        file >> nfoPlayer[i].wlen;
        file >> nfoPlayer[i].invlen;
        file >> nfoPlayer[i].idlelen;
        file >> nfoPlayer[i].mvwx;
        file >> nfoPlayer[i].mvjx;
        file >> nfoPlayer[i].mvrx;
        file >> nfoPlayer[i].mvy;
        file >> nfoPlayer[i].anmdel;
        file >> nfoPlayer[i].skidwait;
        file >> nfoPlayer[i].i_stand;
        file >> nfoPlayer[i].i_walk_s;
        file >> nfoPlayer[i].i_walk_e;
        file >> nfoPlayer[i].i_skid_s;
        file >> nfoPlayer[i].i_skid_e;
        file >> nfoPlayer[i].i_duck_s;
        file >> nfoPlayer[i].i_duck_e;
        file >> nfoPlayer[i].i_run_s;
        file >> nfoPlayer[i].i_run_e;
        file >> nfoPlayer[i].i_jump1_s;
        file >> nfoPlayer[i].i_jump1_e;
        file >> nfoPlayer[i].i_jump2_s;
        file >> nfoPlayer[i].i_jump2_e;
        file >> nfoPlayer[i].i_idle1_s;
        file >> nfoPlayer[i].i_idle1_e;
        file >> nfoPlayer[i].i_idle2_s;
        file >> nfoPlayer[i].i_idle2_e;
        file >> nfoPlayer[i].i_idle3_s;
        file >> nfoPlayer[i].i_idle3_e;
        file >> nfoPlayer[i].i_die_s;
        file >> nfoPlayer[i].i_die_e;
        file >> nfoPlayer[i].i_t_stand_s;
        file >> nfoPlayer[i].i_t_stand_e;
        file >> nfoPlayer[i].i_t_walk_s;
        file >> nfoPlayer[i].i_t_walk_e;
        file >> nfoPlayer[i].i_t_run_s;
        file >> nfoPlayer[i].i_t_run_e;
        file >> nfoPlayer[i].i_t_duck_s;
        file >> nfoPlayer[i].i_t_duck_e;
        file >> nfoPlayer[i].i_t_jump_s;
        file >> nfoPlayer[i].i_t_jump_e;
        file >> nfoPlayer[i].ts_stand;
        file >> nfoPlayer[i].ts_walk;
        file >> nfoPlayer[i].ts_run;
        file >> nfoPlayer[i].ts_duck;
        file >> nfoPlayer[i].ts_jump;
        file >> buf;
        i++;
        
        file >> buf;
    }
    
    file.close();
}

void DeletePlayerNFO(void)
{
    free(nfoPlayer);
}

void InitPlayer(void)
{
    player.level = 1;
    player.lives = 3;
    player.power = 100;
    player.score = 0;
    player.objectiles = 5;
    player.state = ES_STANDING;
    player.sub_state = ES_NOTHING;
    player.save_state = ES_NOTHING;
    player.absx = player.relx - g_VOX;
    player.absy = player.rely - g_VOY + 32;
    player.velx = 0;
    player.vely = 0;
    player.direction = D_RIGHT;
    player.animindex = 0;
    player.invulnerable = FALSE;
    player.invcount = 100;
    player.mpptr = NULL;
    player.pptr = NULL;
}

void RestartPlayer(void)
{
    ENEMY *curEnemy;
    
    g_VOX = g_VOX_origin;
    g_VOY = g_VOY_origin;
    player.power = 100;
    player.objectiles = 5;
    player.state = ES_STANDING;
    player.sub_state = ES_NOTHING;
    player.save_state = ES_NOTHING;
    player.relx = player.ognx;
    player.rely = player.ogny;
    player.absx = player.relx - g_VOX;
    player.absy = player.rely - g_VOY + 32;
    player.velx = 0;
    player.vely = 0;
    player.direction = D_RIGHT;
    player.animindex = 0;
    player.invulnerable = FALSE;
    player.invcount = 100;
    player.mpptr = NULL;
    player.pptr = NULL;
    
    curEnemy = level.enemyLst;
    
    if (curEnemy) {
        while (1) {
            if (curEnemy->state != ES_DEAD) {
                curEnemy->direction = curEnemy->ogndir;
                curEnemy->relx = curEnemy->ognx;
                curEnemy->rely = curEnemy->ogny;
                curEnemy->velx = 0;
                curEnemy->vely = 0;
                curEnemy->absx = -1;
                curEnemy->absy = -1;
                curEnemy->state = ES_UNACTIVATED;
            }
            curEnemy = curEnemy->next;
            if (curEnemy == NULL) {
                break;
            }
        }
    }
    
    curEnemy = level.enemyLst;
    
    if (curEnemy) {
        while (curEnemy->next != NULL) {
            curEnemy = curEnemy->next;
        }
        
        while (1) {
            curEnemy = curEnemy->prev;
            if (curEnemy) {
                curEnemy->next->prev = NULL;
                curEnemy->next = NULL;
            } else {
                break;
            }
        }
    }
    
    level.enemyLst = NULL;
    
    for (int i = 0; i < level.num_movepoly; i++) {
        level.movepoly[i].state = MPS_UNACTIVATED;
        level.movepoly[i].x = level.movepoly[i].ognx;
        level.movepoly[i].y = level.movepoly[i].ogny;
        level.movepoly[i].ascObject->x = level.movepoly[i].ognx;
        level.movepoly[i].ascObject->y = level.movepoly[i].ogny;
        level.movepoly[i].next = NULL;
        level.movepoly[i].prev = NULL;
    }
    
    level.movepolyLst = NULL;
    
    FadeToBlack();
}

void KillPlayer(void)
{
    ZeroMemory(&player, sizeof(player));
}

void ProcessHaji(void)
{
    static jumpdep = 0; // Jump button was depressed
    static attflag = 0; // the attack flag
    static upcount = 0; // the jump up count
    static uplen = 0; // the max jump height for the current jump
    static wcount = 0; // the wait count after landing
    static wlen = 0;    // the wait length
    static icount = 0;  // idle count
    static itype = 0; // the idle type
    static offset = 1;  // the animation offset
    static delay = 0; // the delay counter
    static swait = 0; // the skid wait count
    static jumptype = 0; // the type of jump, standing, inrun
    static idledata = 0; // temp idle anim data
    static idcount = 0; // idle data count
    int i, num_sectors, collision, right, left, top, bottom; // for collision detection
    POLY *curPoly[4]; // the poly's in current sectors (max 4)
    PICKUP *curPickup;
    ENEMY *curEnemy;
    MOVEPOLY *curMovePoly;
    
    if (player.state == ES_DYING) {
        goto deadman;
    }
    
    if (butRight && !butLeft) {
        if (player.state == ES_IDLE) {
            player.state = ES_STANDING;
        }
        
        player.direction = D_RIGHT;
        
        if ((player.state == ES_JUMPINGUP) || (player.state == ES_JUMPINGDOWN) || (player.state == ES_JUMPATTACK)) {
            // if we're jumping, x movement should be restricted
            if (player.velx < nfoPlayer[player_whois].mvjx) {
                player.velx++;
            }
        } else if (player.state == ES_STANDING) {
            // if we're standing, x movement is normal
            if (player.velx < nfoPlayer[player_whois].mvwx) {
                player.velx++;
            }
        } else if (player.state == ES_WALKING) {
            // if we're walking, we need to check to see if we need to start running
            if (player.velx < nfoPlayer[player_whois].mvwx) {
                player.velx++;
            }
        } else if (player.state == ES_RUNNING) {
            if (player.velx < nfoPlayer[player_whois].mvrx) {
                player.velx++;
            }
        } else if (player.state == ES_SKIDDING) {
            player.state = ES_WALKING;
        }
    } else if ((player.state == ES_STANDING) || (player.state == ES_SKIDDING) || (player.state == ES_STOPPING) || (player.state == ES_JUMPINGUP) || (player.state == ES_JUMPINGDOWN) || (player.state == ES_JUMPATTACK)) { // player isn't hitting right button, see if we need to slow haji down
        if (player.velx > 0) {
            player.velx--;    // haji's got velocity, make him stop
        }
        
        if ((player.velx == 0) && (player.state == ES_SKIDDING)) {
            player.state = ES_STANDING;
        }
    }
    
    // player is hitting the left directional button
    if (butLeft && !butRight) {
        if (player.state == ES_IDLE) {
            player.state = ES_STANDING;
        }
        
        player.direction = D_LEFT;
        
        if ((player.state == ES_JUMPINGUP) || (player.state == ES_JUMPINGDOWN) || (player.state == ES_JUMPATTACK)) {
            // if we're jumping, x movement should be restricted
            if (player.velx > -nfoPlayer[player_whois].mvjx) {
                player.velx--;
            }
        } else if (player.state == ES_STANDING) {
            // if we're standing, x movement is normal
            if (player.velx > -nfoPlayer[player_whois].mvwx) {
                player.velx--;
            }
        } else if (player.state == ES_WALKING) {
            // if we're walking, we need to check to see if we need to start running
            if (player.velx > -nfoPlayer[player_whois].mvwx) {
                player.velx--;
            }
        } else if (player.state == ES_RUNNING) {
            if (player.velx > -nfoPlayer[player_whois].mvrx) {
                player.velx--;
            }
        } else if (player.state == ES_SKIDDING) {
            player.state = ES_WALKING;
        }
    } else if ((player.state == ES_STANDING) || (player.state == ES_SKIDDING) || (player.state == ES_STOPPING) || (player.state == ES_JUMPINGUP) || (player.state == ES_JUMPINGDOWN) || (player.state == ES_JUMPATTACK)) { // player isn't hitting the left button, see if we need to slow haji down
        if (player.velx < 0) {
            player.velx++;    // haji's got velocity, make him stop
        }
        
        if ((player.velx == 0) && (player.state == ES_SKIDDING)) {
            player.state = ES_STANDING;
        }
    }
    
    if (butLeft && butRight) {
        if (player.velx < 0) {
            player.velx++;
        }
        if (player.velx > 0) {
            player.velx--;
        }
    }
    
    if (butDuck) {
        if ((player.state == ES_STANDING) || (player.state == ES_WALKING) || (player.state == ES_RUNNING)) {
            player.state = ES_DUCKING;
        }
    }
    
    if (butThrow) {
        if ((player.sub_state != ES_THROWING) && (player.state != ES_DYING) && (player.state != ES_SKIDDING) && (player.state != ES_JUMPATTACK)) {
            if (player.objectiles != 0) {
                switch (player.state) {
                    case ES_STANDING:
                        offset = nfoPlayer[player_whois].i_t_stand_s;
                        break;
                    case ES_WALKING:
                        offset = nfoPlayer[player_whois].i_t_walk_s;
                        break;
                    case ES_RUNNING:
                        offset = nfoPlayer[player_whois].i_t_run_s;
                        break;
                    case ES_JUMPINGUP:
                    case ES_JUMPINGDOWN:
                        offset = nfoPlayer[player_whois].i_t_jump_s;
                        break;
                    case ES_DUCKING:
                        offset = nfoPlayer[player_whois].i_t_duck_s;
                        break;
                }
                player.save_state = player.state;
                player.sub_state = ES_THROWING;
            }
        }
    }
    
    // set haji's state
    if ((player.state != ES_IDLE) && !((player.state == ES_DUCKING) && butDuck) && (player.state != ES_SKIDDING) && (player.state != ES_JUMPINGUP) && (player.state != ES_JUMPINGDOWN) && (player.state != ES_JUMPATTACK)) {
        // haji isn't jumping, is he standing or walking?
        if (butRight && butLeft) {
            if (player.velx == 0) {
                player.state = ES_STANDING; // he's standing
            }
        } else if (!butLeft && !butRight) {
            if (player.velx == 0) {
                player.state = ES_STANDING; // he's standing
            } else if (player.state == ES_WALKING) {
                player.state = ES_STOPPING;
            } else if (player.state == ES_RUNNING) {
                swait++;
                if (swait >= nfoPlayer[player_whois].skidwait) {
                    player.state = ES_SKIDDING;
                    swait = 0;
                }
            }
        } else if (!butRun) {
            swait = 0;
            player.state = ES_WALKING; // he's walking
            if (abs(player.velx) > nfoPlayer[player_whois].mvwx) {
                switch (player.direction) {
                    case D_LEFT:
                        player.velx = -nfoPlayer[player_whois].mvwx;
                        break;
                    case D_RIGHT:
                        player.velx = nfoPlayer[player_whois].mvwx;
                        break;
                }
            }
        } else {
            player.state = ES_RUNNING;
            if (offset < (nfoPlayer[player_whois].i_run_s - 1)) {
                offset = nfoPlayer[player_whois].i_run_s - 1;
            }
        }
    }
    
    if (player.state == ES_DUCKING) {
        if (player.velx > 0) {
            player.velx--;
        }
        if (player.velx < 0) {
            player.velx++;
        }
    }
    
    if (player.state == ES_SKIDDING) {
        PlaySound(FX_SKID, 0);
    }
    
    // test if player hit the jump button
    if (butJump) {
        if (!wlen) {
            if (jumpdep) {
                if (player.state == ES_JUMPINGDOWN) {
                    player.state = ES_JUMPATTACK;
                } else if (player.state == ES_JUMPINGUP) {
                    attflag = 1;
                    upcount = uplen - 1;
                }
                jumpdep = 0;
            }
            
            // make haji jump!!!
            if ((player.state != ES_JUMPINGUP) && (player.state != ES_JUMPINGDOWN) && (player.state != ES_JUMPATTACK) && (player.state != ES_DUCKING)) {
                // we're not jumping, so start a jump
                PlaySound(FX_HAJIJUMP, 0);
                player.vely = -nfoPlayer[player_whois].mvy;
                player.mpptr = NULL;
                player.pptr = NULL;
                player.state = ES_JUMPINGUP;
                if (player.velx == 0) {
                    offset = nfoPlayer[player_whois].i_jump1_s - 1;
                    jumptype = 1;
                } else {
                    offset = nfoPlayer[player_whois].i_jump2_s - 1;
                    jumptype = 2;
                }
                uplen = nfoPlayer[player_whois].iulen;
            } else if ((player.state == ES_JUMPINGUP) && (uplen != upcount)) {
                // we're already jumping/button is still pressed/we haven't hit max height so make haji jump higher
                if (uplen < nfoPlayer[player_whois].uplen) { // see if we're at the max of max height
                    uplen++;    // increase the max height
                }
            } else if (player.state == ES_DUCKING) {
                if (player.pptr != NULL) {
                    if (player.pptr->canjumpdown) {
                        player.vely++;
                        player.sub_state = ES_JUMPINGDOWN;
                        player.state = ES_JUMPINGDOWN;
                        offset = nfoPlayer[player_whois].i_jump2_s + 4;
                        jumptype = 2;
                    }
                }
            }
        }
    } else if ((player.state == ES_JUMPINGUP) || (player.state == ES_JUMPINGDOWN)) {
        jumpdep = 1;
    }
    
    // check if the wait length variable is set
    if (wlen) {
        // we need to wait a bit
        wcount++;
        if ((wcount == wlen) || !butJump) {
            // we've waited long enough, zero it out
            wcount = 0;
            wlen = 0;
        }
    }
    
    if ((player.state == ES_STANDING) && (player.sub_state != ES_THROWING)) {
        icount++;
        if (icount > nfoPlayer[player_whois].idlelen) {
            player.state = ES_IDLE;
            icount = 0;
            itype = rand() % 3;
            delay = 0;
            switch (itype) {
                case 0:
                    offset = nfoPlayer[player_whois].i_idle1_s;
                    break;
                case 1:
                    offset = nfoPlayer[player_whois].i_idle2_s;
                    break;
                case 2:
                    idledata = 3 + rand() % 5;
                    idcount = 0;
                    offset = nfoPlayer[player_whois].i_idle3_s;
                    break;
            }
        }
    } else {
        icount = 0;
    }
    
    // are we jumping up?
    if (player.state == ES_JUMPINGUP) {
        upcount++; // increment the up jump count
        if ((player.vely < nfoPlayer[player_whois].mvy) && (upcount == uplen)) {
            // did we hit the vertex of our jump
            upcount = 0; // zero the up jump count
            player.vely++; // slow our velocity
            if (player.vely > 0) { // if we're headed down now, set the jump state
                if (attflag) {
                    attflag = 0;
                    player.state = ES_JUMPATTACK;
                } else {
                    player.state = ES_JUMPINGDOWN;
                }
            }
        }
    }
    
    // are we falling?
    if ((player.state == ES_JUMPINGDOWN) || (player.state == ES_JUMPATTACK)) {
        // just increase the downward velocity if it's below the max
        if (player.vely < nfoPlayer[player_whois].mvy) {
            player.vely += 2;
        }
    }
    
    // adjust Haji's velocity due to being on a movable platform
    if (player.mpptr != NULL) {
        player.velx += player.mpptr->velx;
        player.vely += player.mpptr->vely;
    }
    
    if (player.velx > 0) {
        if (player.absx < PLAYER_MIDX) {
            player.absx += player.velx;
            player.relx += player.velx;
            
            if (player.absx > PLAYER_MIDX) {
                int dif = player.absx - PLAYER_MIDX;
                g_VOX += dif;
                player.absx = PLAYER_MIDX;
            }
        } else if (player.absx == PLAYER_MIDX) {
            g_VOX += player.velx;
            player.relx += player.velx;
            
            if (g_VOX > (level.width - SECTOR_W)) {
                int dif = g_VOX - (level.width - SECTOR_W);
                g_VOX = (level.width - SECTOR_W);
                player.absx += dif;
            }
        } else if (player.absx > PLAYER_MIDX) {
            player.relx += player.velx;
            
            if (player.relx > (level.width - (nfoPlayer[player_whois].width + 1))) {
                player.absx = 640 - (nfoPlayer[player_whois].width + 1);
                player.relx = (level.width - (nfoPlayer[player_whois].width + 1));
            } else {
                player.absx += player.velx;
            }
        }
    }
    
    if (player.velx < 0) {
        if (player.absx < PLAYER_MIDX) {
            player.relx += player.velx;
            if (player.relx < 0) {
                player.absx = 0;
                player.relx = 0;
            } else {
                player.absx += player.velx;
            }
        } else if (player.absx == PLAYER_MIDX) {
            g_VOX += player.velx;
            player.relx += player.velx;
            if (g_VOX < 0) {
                int dif = 0 - g_VOX;
                g_VOX = 0;
                player.absx -= dif;
            }
        } else if (player.absx > PLAYER_MIDX) {
            player.absx += player.velx;
            player.relx += player.velx;
            if (player.absx < PLAYER_MIDX) {
                int dif = PLAYER_MIDX - player.absx;
                g_VOX -= dif;
                player.absx = PLAYER_MIDX;
            }
        }
    }
    
    if (player.vely > 0) {
        if (player.absy < PLAYER_MIDY) {
            player.absy += player.vely;
            player.rely += player.vely;
            if (player.absy > PLAYER_MIDY) {
                int dif = player.absy - PLAYER_MIDY;
                g_VOY += dif;
                player.absy = PLAYER_MIDY;
            }
        } else if (player.absy == PLAYER_MIDY) {
            g_VOY += player.vely;
            player.rely += player.vely;
            if (g_VOY > (level.height - SECTOR_H)) {
                int dif = g_VOY - (level.height - SECTOR_H);
                g_VOY = (level.height - SECTOR_H);
                player.absy += dif;
            }
        } else if (player.absy > PLAYER_MIDY) {
            player.rely += player.vely;
            if (player.rely > (level.height - nfoPlayer[player_whois].height)) {
                // DIED player has died by falling
            } else {
                player.absy += player.vely;
            }
        }
    }
    
    if (player.vely < 0) {
        if (player.absy < PLAYER_MIDY) {
            player.rely += player.vely;
            player.absy += player.vely;
        } else if (player.absy == PLAYER_MIDY) {
            g_VOY += player.vely;
            player.rely += player.vely;
            if (g_VOY < 0) {
                int dif = 0 - g_VOY;
                g_VOY = 0;
                player.absy -= dif;
            }
        } else if (player.absy > PLAYER_MIDY) {
            player.absy += player.vely;
            player.rely += player.vely;
            if (player.absy < PLAYER_MIDY) {
                int dif = PLAYER_MIDY - player.absy;
                g_VOY -= dif;
                player.absy = PLAYER_MIDY;
            }
        }
    }
    
    if (player.mpptr != NULL) {
        player.velx -= player.mpptr->velx;
        player.vely -= player.mpptr->vely;
    }
    
    if (player.invulnerable) {
        player.invcount++;
        if (player.invcount == 100) {
            player.invulnerable = FALSE;
        }
    }
    
    if ((player.state != ES_JUMPINGUP) && (player.state != ES_JUMPINGDOWN) && (player.state != ES_JUMPATTACK)) {
        num_sectors = GetSectorList(curPoly, player.relx, player.rely, nfoPlayer[player_whois].width, nfoPlayer[player_whois].height);
        
        collision = 0;
        
        for (i = 0; i < num_sectors; i++) {
            if (curPoly[i]) {
                while (1) {
                    if (curPoly[i]->canwalkon) {
                        collision = Collision(COLLISION_X, COLLISION_Y, COLLISION_W, COLLISION_H + 1, curPoly[i], &right, &left, &top, &bottom);
                        
                        if (collision && bottom && !top) {
                            player.pptr = curPoly[i];
                            collision = 1;
                            i = num_sectors;
                            break;
                        } else {
                            collision = 0;
                        }
                    }
                    if (curPoly[i]->next == NULL) {
                        break;
                    }
                    curPoly[i] = curPoly[i]->next;
                }
            }
        }
        
        curMovePoly = level.movepolyLst;
        
        if (curMovePoly && !collision) {
            while (1) {
                if (curMovePoly->canwalkon) {
                    collision = Collision(COLLISION_X, COLLISION_Y, COLLISION_W, COLLISION_H + 1, curMovePoly->x, curMovePoly->y, curMovePoly->w, curMovePoly->h, &right, &left, &top, &bottom);
                    
                    if (collision && bottom && !top && (curMovePoly->state != MPS_FALLING)) {
                        player.mpptr = curMovePoly;
                        collision = 1;
                        break;
                    } else {
                        collision = 0;
                    }
                }
                if (curMovePoly->next == NULL) {
                    break;
                }
                curMovePoly = curMovePoly->next;
            }
        }
        
        if (collision == 0) {
            player.vely++;
            player.state = ES_JUMPINGDOWN;
            offset = nfoPlayer[player_whois].i_jump2_s + 4;
            jumptype = 2;
            player.mpptr = NULL;
            player.pptr = NULL;
        }
    }
    
    if (player.state == ES_JUMPATTACK) {
        curEnemy = level.enemyLst;
        if (curEnemy != NULL) {
            while (1) {
                if ((curEnemy->state != ES_DEAD) && (curEnemy->state != ES_DYING)) {
                    collision = Collision(COLLISION_X, COLLISION_Y, COLLISION_W, COLLISION_H - 45, curEnemy->relx, curEnemy->rely, curEnemy->width, curEnemy->height, &right, &left, &top, &bottom);
                    
                    if (collision && bottom) {
                        player.rely -= bottom - 1;
                        if (player.absy != PLAYER_MIDY) {
                            player.absy -= bottom - 1;
                        } else {
                            g_VOY -= bottom - 1;
                        }
                        player.vely = -nfoPlayer[player_whois].mvy;
                        player.state = ES_JUMPINGUP;
                        uplen = nfoPlayer[player_whois].aulen;
                        attflag = 1;
                        player.score += curEnemy->killpts;
                        curEnemy->state = ES_DYING;
                        curEnemy->offset = nfoEnemy[curEnemy->id].walkfe;
                        PlaySound(FX_ENEMYDIE, 0);
                        curEnemy = curEnemy->next;
                        if (curEnemy == NULL) {
                            break;
                        }
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
    }
    
    if (player.vely) {
        num_sectors = GetSectorList(curPoly, player.relx, player.rely, nfoPlayer[player_whois].width, nfoPlayer[player_whois].height);
        
        for (i = 0; i < num_sectors; i++) {
            if (curPoly[i]) {
                while (1) {
                    collision = Collision(COLLISION_X, COLLISION_Y, COLLISION_W, COLLISION_H, curPoly[i], &right, &left, &top, &bottom);
                    
                    if (collision && bottom && !top && (player.vely > 0) && curPoly[i]->canwalkon) {
                        if (player.pptr != NULL)
                            if ((player.sub_state == ES_JUMPINGDOWN) && (player.pptr->id == curPoly[i]->id)) {
                                goto jumpdown;
                            }
                        wlen = nfoPlayer[player_whois].wlen;
                        player.rely -= bottom - 1;
                        player.vely = 0;
                        player.state = ES_STANDING;
                        player.pptr = curPoly[i];
                        if (player.absy != PLAYER_MIDY) {
                            player.absy -= bottom - 1;
                        } else {
                            g_VOY -= bottom - 1;
                        }
                        i = num_sectors;
                        break;
                    } else if (collision && top && !bottom && (player.vely < 0) && !curPoly[i]->canmovethrough) {
                        upcount = 0;
                        player.vely = 0;
                        if (attflag) {
                            player.state = ES_JUMPATTACK;
                        } else {
                            player.state = ES_JUMPINGDOWN;
                        }
                        i = num_sectors;
                        break;
                    }
jumpdown:
                    if (curPoly[i]->next == NULL) {
                        break;
                    }
                    curPoly[i] = curPoly[i]->next;
                }
            }
        }
        
        curMovePoly = level.movepolyLst;
        
        if ((curMovePoly != NULL) && (player.state != ES_STANDING)) {
            while (1) {
                collision = Collision(COLLISION_X, COLLISION_Y, COLLISION_W, COLLISION_H, curMovePoly->x, curMovePoly->y, curMovePoly->w, curMovePoly->h, &right, &left, &top, &bottom);
                
                if (collision && bottom && !top && (player.vely > 0) && curMovePoly->canwalkon && (curMovePoly->state != MPS_FALLING)) {
                    wlen = nfoPlayer[player_whois].wlen;
                    player.rely -= bottom - 1;
                    player.vely = 0;
                    player.state = ES_STANDING;
                    player.mpptr = curMovePoly;
                    if (player.absy != PLAYER_MIDY) {
                        player.absy -= bottom - 1;
                    } else {
                        g_VOY -= bottom - 1;
                    }
                    break;
                } else if (collision && top && !bottom && (player.vely < 0) && !curMovePoly->canmovethrough) {
                    upcount = 0;
                    player.vely = 0;
                    if (attflag) {
                        player.state = ES_JUMPATTACK;
                    } else {
                        player.state = ES_JUMPINGDOWN;
                    }
                    break;
                }
                
                curMovePoly = curMovePoly->next;
                if (curMovePoly == NULL) {
                    break;
                }
            }
        }
    } else {
        curMovePoly = level.movepolyLst;
        
        if (curMovePoly != NULL) {
            while (1) {
                collision = Collision(curMovePoly->x, curMovePoly->y, curMovePoly->w, curMovePoly->h, &right, &left, &top, &bottom);
                
                if (collision && bottom && !top && curMovePoly->canwalkon && (curMovePoly->state != MPS_FALLING)) {
                    player.rely -= bottom - 1;
                    if (player.absy != PLAYER_MIDY) {
                        player.absy -= bottom - 1;
                    } else {
                        g_VOY -= bottom - 1;
                    }
                    player.mpptr = curMovePoly;
                    break;
                } else if (collision && top && !bottom && !curMovePoly->canmovethrough && (curMovePoly->state != MPS_FALLING)) {
                    // DIED player has died by getting crushed
                    break;
                }
                
                curMovePoly = curMovePoly->next;
                if (curMovePoly == NULL) {
                    break;
                }
            }
        }
    }
    
    if (player.velx) {
        num_sectors = GetSectorList(curPoly, player.relx, player.rely, nfoPlayer[player_whois].width, nfoPlayer[player_whois].height);
        
        for (i = 0; i < num_sectors; i++) {
            if (curPoly[i]) {
                while (1) {
                    if (!curPoly[i]->canmovethrough) {
                        collision = Collision(curPoly[i], &right, &left, &top, &bottom);
                        
                        if (left && collision && (player.velx < 0)) {
                            player.velx = 0;
                            player.relx += left - 1;
                            if (player.absx != PLAYER_MIDX) {
                                player.absx += left - 1;
                            } else {
                                g_VOX += left - 1;
                            }
                            i = num_sectors;
                            break;
                        }
                        
                        if (right && collision && (player.velx > 0)) {
                            player.velx = 0;
                            player.relx -= right - 1;
                            if (player.absx != PLAYER_MIDX) {
                                player.absx -= right - 1;
                            } else {
                                g_VOX -= right - 1;
                            }
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
        
        curMovePoly = level.movepolyLst;
        
        if ((curMovePoly != NULL) && (player.velx != 0)) {
            while (1) {
                if (!curMovePoly->canmovethrough) {
                    collision = Collision(curMovePoly->x, curMovePoly->y, curMovePoly->w, curMovePoly->h, &right, &left, &top, &bottom);
                    
                    if (left && collision && (player.velx < 0) && (curMovePoly->state != MPS_FALLING)) {
                        player.velx = 0;
                        player.relx += left - 1;
                        if (player.absx != PLAYER_MIDX) {
                            player.absx += left - 1;
                        } else {
                            g_VOX += left - 1;
                        }
                        break;
                    }
                    
                    if (right && collision && (player.velx > 0) && (curMovePoly->state != MPS_FALLING)) {
                        player.velx = 0;
                        player.relx -= right - 1;
                        if (player.absx != PLAYER_MIDX) {
                            player.absx -= right - 1;
                        } else {
                            g_VOX -= right - 1;
                        }
                        break;
                    }
                }
                
                curMovePoly = curMovePoly->next;
                if (curMovePoly == NULL) {
                    break;
                }
            }
        }
    } else {
        curMovePoly = level.movepolyLst;
        
        if (curMovePoly != NULL) {
            while (1) {
                if (!curMovePoly->canmovethrough) {
                    collision = Collision(curMovePoly->x, curMovePoly->y, curMovePoly->w, curMovePoly->h, &right, &left, &top, &bottom);
                    
                    if (left && !right && collision && (curMovePoly->state != MPS_FALLING)) {
                        player.velx = 0;
                        player.relx += left - 1;
                        if (player.absx != PLAYER_MIDX) {
                            player.absx += left - 1;
                        } else {
                            g_VOX += left - 1;
                        }
                        break;
                    }
                    
                    if (right && !left && collision && (curMovePoly->state != MPS_FALLING)) {
                        player.velx = 0;
                        player.relx -= right - 1;
                        if (player.absx != PLAYER_MIDX) {
                            player.absx -= right - 1;
                        } else {
                            g_VOX -= right - 1;
                        }
                        break;
                    }
                }
                
                curMovePoly = curMovePoly->next;
                if (curMovePoly == NULL) {
                    break;
                }
            }
        }
    }
    
    collision = Collision(level.gate.verts[0].x, level.gate.verts[0].y, level.gate.verts[3].x - level.gate.verts[0].x, level.gate.verts[3].y - level.gate.verts[0].y, &right, &left, &top, &bottom);
    
    if (collision) {
        // level won
        player.level++;
        Game_State = ENDLEVEL;
    }
    
    curEnemy = level.enemyLst;
    if (curEnemy && (!player.invulnerable) && (player.state != ES_JUMPATTACK)) {
        while (1) {
            if ((curEnemy->state != ES_DEAD) && (curEnemy->state != ES_DYING)) {
                collision = Collision(curEnemy->relx, curEnemy->rely, curEnemy->width, curEnemy->height, &right, &left, &top, &bottom);
                
                if (collision) {
                    PlaySound(FX_OUCH, 0);
                    player.power -= curEnemy->hitpts;
                    player.invulnerable = TRUE;
                    player.invcount = 100 - nfoPlayer[player_whois].invlen;
                    break;
                }
            }
            curEnemy = curEnemy->next;
            if (curEnemy == NULL) {
                break;
            }
        }
    }
    
    curPickup = level.pickupLst;
    if (curPickup) {
        while (1) {
            if ((curPickup->state != ES_DEAD) && (curPickup->state != ES_DYING)) {
                collision = Collision(curPickup->relx, curPickup->rely, curPickup->width, curPickup->height, &right, &left, &top, &bottom);
                
                if (collision) {
                    AddExplosion(curPickup->relx - 9, curPickup->rely - 9, PARTICLEL_COLOR_R, PARTICLEL_COLOR_G, PARTICLEL_COLOR_B, PARTICLE_COLOR_R, PARTICLE_COLOR_G, PARTICLE_COLOR_B);
                    PerformPickup(curPickup);
                    curPickup->state = ES_DYING;
                    curPickup->trans = 100;
                    if (curPickup == NULL) {
                        break;
                    }
                } else {
                    curPickup = curPickup->next;
                    if (curPickup == NULL) {
                        break;
                    }
                }
            } else {
                curPickup = curPickup->next;
                if (curPickup == NULL) {
                    break;
                }
            }
        }
    }
    
    if (player.power <= 0) {
        if (--player.lives < 0) {
            // GAME OVER
        } else {
            // DIED player has died because of no power
            player.state = ES_DYING;
            offset = nfoPlayer[player_whois].i_die_s - 1;
        }
    }
    
deadman:
    if (player.sub_state == ES_THROWING) {
        if (player.save_state == ES_JUMPINGUP) {
            if ((player.state != ES_JUMPINGUP) && (player.state != ES_JUMPINGDOWN)) {
                player.sub_state = ES_NOTHING;
            }
        } else {
            if (player.save_state != player.state) {
                player.sub_state = ES_NOTHING;
            }
        }
    }
    
    if (player.sub_state == ES_THROWING) {
        switch (player.state) {
            case ES_STANDING:
                delay++;
                if (delay > nfoPlayer[player_whois].anmdel) {
                    delay = 0;
                    offset++;
                    if (offset == (nfoPlayer[player_whois].i_t_stand_s + nfoPlayer[player_whois].ts_stand)) {
                        player.objectiles--;
                        PlaySound(FX_HAJITHROW, 0);
                        AddObjectile(E_PLAYER, OT_FALLING, player.direction, (player.direction == D_LEFT) ? (player.relx + 32) : (player.relx + nfoPlayer[player_whois].width - 32), player.rely + 17);
                    }
                    if (offset > nfoPlayer[player_whois].i_t_stand_e) {
                        offset--;
                        player.sub_state = ES_NOTHING;
                    }
                }
                player.animindex = offset;
                break;
            case ES_WALKING:
                delay++;
                if (delay > nfoPlayer[player_whois].anmdel) {
                    delay = 0;
                    offset++;
                    if (offset == (nfoPlayer[player_whois].i_t_walk_s + nfoPlayer[player_whois].ts_walk)) {
                        player.objectiles--;
                        PlaySound(FX_HAJITHROW, 0);
                        AddObjectile(E_PLAYER, OT_FALLING, player.direction, (player.direction == D_LEFT) ? (player.relx + 32) : (player.relx + nfoPlayer[player_whois].width - 32), player.rely + 17);
                    }
                    if (offset > nfoPlayer[player_whois].i_t_walk_e) {
                        offset--;
                        player.sub_state = ES_NOTHING;
                    }
                }
                player.animindex = offset;
                break;
            case ES_RUNNING:
                delay++;
                if (delay > nfoPlayer[player_whois].anmdel) {
                    delay = 0;
                    offset++;
                    if (offset == (nfoPlayer[player_whois].i_t_run_s + nfoPlayer[player_whois].ts_run)) {
                        player.objectiles--;
                        PlaySound(FX_HAJITHROW, 0);
                        AddObjectile(E_PLAYER, OT_FALLING, player.direction, (player.direction == D_LEFT) ? (player.relx + 32) : (player.relx + nfoPlayer[player_whois].width - 32), player.rely + 17);
                    }
                    if (offset > nfoPlayer[player_whois].i_t_run_e) {
                        offset--;
                        player.sub_state = ES_NOTHING;
                    }
                }
                player.animindex = offset;
                break;
            case ES_JUMPINGUP:
            case ES_JUMPINGDOWN:
            case ES_JUMPATTACK:
                delay++;
                if (delay > nfoPlayer[player_whois].anmdel) {
                    delay = 0;
                    offset++;
                    if (offset == (nfoPlayer[player_whois].i_t_jump_s + nfoPlayer[player_whois].ts_jump)) {
                        player.objectiles--;
                        PlaySound(FX_HAJITHROW, 0);
                        AddObjectile(E_PLAYER, OT_FALLING, player.direction, (player.direction == D_LEFT) ? (player.relx + 32) : (player.relx + nfoPlayer[player_whois].width - 32), player.rely + 17);
                    }
                    if (offset > nfoPlayer[player_whois].i_t_jump_e) {
                        offset--;
                        player.sub_state = ES_NOTHING;
                    }
                }
                player.animindex = offset;
                break;
            case ES_DUCKING:
                delay++;
                if (delay > nfoPlayer[player_whois].anmdel) {
                    delay = 0;
                    offset++;
                    if (offset == (nfoPlayer[player_whois].i_t_duck_s + nfoPlayer[player_whois].ts_duck)) {
                        player.objectiles--;
                        PlaySound(FX_HAJITHROW, 0);
                        AddObjectile(E_PLAYER, OT_FALLING, player.direction, (player.direction == D_LEFT) ? (player.relx + 32) : (player.relx + nfoPlayer[player_whois].width - 32), player.rely + nfoPlayer[player_whois].height / 2);
                    }
                    if (offset > nfoPlayer[player_whois].i_t_duck_e) {
                        offset--;
                        player.sub_state = ES_NOTHING;
                    }
                }
                player.animindex = offset;
                break;
        }
        if (player.direction == D_LEFT) {
            player.animindex += nfoPlayer[player_whois].pics / 2;
        }
    } else {
        switch (player.state) {
            case ES_STANDING:
                player.animindex = nfoPlayer[player_whois].i_stand;
                if (StatusSound(FX_WALK) & DSBSTATUS_LOOPING) {
                    StopSound(FX_WALK);
                } else if (StatusSound(FX_RUN) & DSBSTATUS_LOOPING) {
                    StopSound(FX_RUN);
                }
                break;
            case ES_STOPPING:
            case ES_SKIDDING:
                if (player.direction == D_LEFT) {
                    player.animindex = player.animindex - (nfoPlayer[player_whois].pics / 2);
                }
                if ((player.animindex != nfoPlayer[player_whois].i_skid_s) && (player.animindex != nfoPlayer[player_whois].i_skid_s + 1) && (player.animindex != nfoPlayer[player_whois].i_skid_s + 2) && (player.state != ES_STOPPING)) {
                    player.animindex = nfoPlayer[player_whois].i_skid_s;
                } else if (player.animindex == nfoPlayer[player_whois].i_skid_s) {
                    player.animindex = nfoPlayer[player_whois].i_skid_s + 1;
                } else if (player.animindex == nfoPlayer[player_whois].i_skid_s + 1) {
                    player.animindex = nfoPlayer[player_whois].i_skid_s + 2;
                } else if (player.state == ES_SKIDDING || (player.animindex == nfoPlayer[player_whois].i_skid_s + 2)) {
                    player.animindex = nfoPlayer[player_whois].i_skid_s + 2;
                } else {
                    player.animindex = nfoPlayer[player_whois].i_stand;
                }
                break;
            case ES_IDLE:
                switch (itype) {
                    case 0:
                        delay++;
                        if (delay > (nfoPlayer[player_whois].anmdel)) {
                            delay = 0;
                            offset++;
                            if (offset > nfoPlayer[player_whois].i_idle1_e) {
                                offset--;
                                player.state = ES_STANDING;
                            }
                        }
                        player.animindex = offset;
                        break;
                    case 1:
                        delay++;
                        if (delay > (nfoPlayer[player_whois].anmdel)) {
                            delay = 0;
                            offset++;
                            if (offset > nfoPlayer[player_whois].i_idle2_e) {
                                offset--;
                                player.state = ES_STANDING;
                            }
                        }
                        player.animindex = offset;
                        break;
                    case 2:
                        delay++;
                        if (delay > (nfoPlayer[player_whois].anmdel)) {
                            delay = 0;
                            if (offset > (nfoPlayer[player_whois].i_idle3_s + 3)) {
                                idcount++;
                            }
                            if (idcount > idledata) {
                                offset++;
                                if (offset > nfoPlayer[player_whois].i_idle3_e) {
                                    offset--;
                                    player.state = ES_STANDING;
                                }
                            } else {
                                offset++;
                                if (offset > (nfoPlayer[player_whois].i_idle3_e - 3)) {
                                    offset = nfoPlayer[player_whois].i_idle3_s + 3;
                                }
                            }
                        }
                        player.animindex = offset;
                        break;
                }
                break;
            case ES_RUNNING:
                if (StatusSound(FX_WALK) & DSBSTATUS_LOOPING) {
                    StopSound(FX_WALK);
                }
                if (!(StatusSound(FX_RUN) & DSBSTATUS_LOOPING)) {
                    PlaySound(FX_RUN, 1);
                }
                delay++;
                if (delay > nfoPlayer[player_whois].anmdel) {
                    delay = 0;
                    offset++;
                    if (offset > nfoPlayer[player_whois].i_run_e) {
                        offset = nfoPlayer[player_whois].i_run_s;
                    }
                }
                player.animindex = offset;
                break;
            case ES_WALKING:
                if (StatusSound(FX_RUN) & DSBSTATUS_LOOPING) {
                    StopSound(FX_RUN);
                }
                if (!(StatusSound(FX_WALK) & DSBSTATUS_LOOPING)) {
                    PlaySound(FX_WALK, 1);
                }
                if (player.animindex == nfoPlayer[player_whois].i_stand) {
                    player.animindex = offset = nfoPlayer[player_whois].i_walk_s;
                } else {
                    delay++;
                    if (delay > nfoPlayer[player_whois].anmdel) {
                        delay = 0;
                        offset++;
                        if (offset > nfoPlayer[player_whois].i_walk_e) {
                            offset = nfoPlayer[player_whois].i_walk_s;
                        }
                    }
                    player.animindex = offset;
                }
                
                break;
            case ES_JUMPINGUP:
            case ES_JUMPINGDOWN:
                if (StatusSound(FX_WALK) & DSBSTATUS_LOOPING) {
                    StopSound(FX_WALK);
                } else if (StatusSound(FX_RUN) & DSBSTATUS_LOOPING) {
                    StopSound(FX_RUN);
                }
                if (jumptype == 1) {
                    if (attflag && (offset == nfoPlayer[player_whois].i_jump1_s + 4)) {
                        player.animindex = nfoPlayer[player_whois].i_jump1_s + 4;
                    } else {
                        delay++;
                        if (delay > nfoPlayer[player_whois].anmdel) {
                            delay = 0;
                            offset++;
                            if (offset > nfoPlayer[player_whois].i_jump1_e) {
                                offset--;
                            }
                        }
                        player.animindex = offset;
                    }
                } else if (jumptype == 2) {
                    if (attflag && (offset == nfoPlayer[player_whois].i_jump2_s + 4)) {
                        player.animindex = nfoPlayer[player_whois].i_jump2_s + 4;
                    } else {
                        delay++;
                        if (delay > nfoPlayer[player_whois].anmdel) {
                            delay = 0;
                            offset++;
                            if (offset > nfoPlayer[player_whois].i_jump2_e) {
                                offset--;
                            }
                        }
                        player.animindex = offset;
                    }
                }
                break;
            case ES_JUMPATTACK:
                player.animindex = nfoPlayer[player_whois].i_jump1_s + 4;
                break;
            case ES_DUCKING:
                if (StatusSound(FX_WALK) & DSBSTATUS_LOOPING) {
                    StopSound(FX_WALK);
                } else if (StatusSound(FX_RUN) & DSBSTATUS_LOOPING) {
                    StopSound(FX_RUN);
                }
                if (player.direction == D_LEFT) {
                    player.animindex = player.animindex - (nfoPlayer[player_whois].pics / 2);
                }
                if ((player.animindex != nfoPlayer[player_whois].i_duck_s) && (player.animindex != nfoPlayer[player_whois].i_duck_s + 1)) {
                    player.animindex = nfoPlayer[player_whois].i_duck_s;
                } else if (player.animindex == nfoPlayer[player_whois].i_duck_s) {
                    player.animindex = nfoPlayer[player_whois].i_duck_s + 1;
                }
                break;
            case ES_DYING:
                delay++;
                if (delay > nfoPlayer[player_whois].anmdel) {
                    delay = 0;
                    offset++;
                    if (offset > nfoPlayer[player_whois].i_die_e) {
                        offset--;
                        RestartPlayer();
                    }
                }
                player.animindex = offset;
                break;
        }
        if (player.direction == D_LEFT) {
            player.animindex += nfoPlayer[player_whois].pics / 2;
        }
    }
}