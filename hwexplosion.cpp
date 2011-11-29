/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwexplosion.cpp): particle explosion code
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <digifx.h>
#include "hwexplosion.h"
#include "hwutil.h"

EXPLOSION *explosions = NULL;

void AddExplosion(int x, int y, int r1, int g1, int b1, int r2, int g2, int b2)
{
    EXPLOSION *curExplosion;
    int i, ang;
    int count;
    double adder[5] = { 1.45, 1.85, 2.25, 2.65, 3.05 };
    
    curExplosion = explosions;
    
    if (curExplosion == NULL) {
        explosions = (EXPLOSION *)malloc(sizeof(EXPLOSION));
        curExplosion = explosions;
        curExplosion->next = NULL;
        curExplosion->prev = NULL;
    } else {
        while (curExplosion->next != NULL) {
            curExplosion = curExplosion->next;
        }
        curExplosion->next = (EXPLOSION *)malloc(sizeof(EXPLOSION));
        curExplosion->next->prev = curExplosion;
        curExplosion = curExplosion->next;
        curExplosion->next = NULL;
    }
    
    curExplosion->x = x;
    curExplosion->y = y;
    curExplosion->lifetime = EXPLOSION_LIFE;
    
    for (i = 0; i < NUM_PARTICLES; i++) {
        curExplosion->particles[i].x = EXPLOSION_RADIUS;
        curExplosion->particles[i].y = EXPLOSION_RADIUS;
        curExplosion->particles[i].counter = 0;
        curExplosion->particles[i].threshold = 5;
        curExplosion->particles[i].trans = 100;
    }
    
    ang = 0;
    for (i = 0; i < NUM_PARTICLES_LONG; i++) {
        curExplosion->particles[i].r = r1;
        curExplosion->particles[i].g = g1;
        curExplosion->particles[i].b = b1;
        ang += (360 / NUM_PARTICLES_LONG) + rand() % 24;
        if (ang >= 360) {
            ang -= 360;
        }
        curExplosion->particles[i].vx = (4 + (rand() % 5)) * g_CosTab[ang];
        curExplosion->particles[i].vy = (4 + (rand() % 5)) * g_SinTab[ang];
    }
    
    ang = 0;
    count = rand() % 5;
    for (i = NUM_PARTICLES_LONG; i < NUM_PARTICLES; i++) {
        curExplosion->particles[i].r = r2;
        curExplosion->particles[i].g = g2;
        curExplosion->particles[i].b = b2;
        ang += 6 + rand() % 1;
        if (ang >= 360) {
            ang -= 360;
        }
        curExplosion->particles[i].vx = adder[count] * g_CosTab[ang];
        curExplosion->particles[i].vy = adder[count] * g_SinTab[ang];
        count++;
        if (count > 4) {
            count = rand() % 5;
        }
    }
}

void RemoveExplosion(EXPLOSION *explosion)
{
    EXPLOSION *curExplosion;
    
    curExplosion = explosion;
    
    if (explosion->prev && explosion->next) {
        explosion->prev->next = explosion->next;
        explosion->next->prev = explosion->prev;
    } else if (explosion->prev && !explosion->next) {
        explosion->prev->next = NULL;
    } else if (!explosion->prev && explosion->next) {
        explosion->next->prev = NULL;
        explosions = explosion->next;
    } else {
        explosions = NULL;
    }
    
    explosion->next = NULL;
    explosion->prev = NULL;
    
    free(curExplosion);
}

void AnimateExplosions(void)
{
    EXPLOSION *curExplosion, *savExplosion;
    int i;
    
    curExplosion = explosions;
    
    if (curExplosion != NULL) {
        while (1) {
            curExplosion->lifetime--;
            
            if (curExplosion->lifetime < 0) {
                savExplosion = curExplosion->next;
                RemoveExplosion(curExplosion);
                curExplosion = savExplosion;
            } else {
                for (i = 0; i < NUM_PARTICLES; i++) {
                    curExplosion->particles[i].x += curExplosion->particles[i].vx;
                    curExplosion->particles[i].y += curExplosion->particles[i].vy;
                    curExplosion->particles[i].trans -= PARTICLE_DTRANS;
                    
                    curExplosion->particles[i].counter++;
                    if ((curExplosion->particles[i].counter >= curExplosion->particles[i].threshold) && (i >= NUM_PARTICLES_LONG)) {
                        if (curExplosion->particles[i].vx < 0) {
                            curExplosion->particles[i].vx += (double)PARTICLE_DECEL_X;
                            if (curExplosion->particles[i].vx > 0) {
                                curExplosion->particles[i].vx = 0;
                            }
                        } else {
                            curExplosion->particles[i].vx -= (double)PARTICLE_DECEL_X;
                            if (curExplosion->particles[i].vx < 0) {
                                curExplosion->particles[i].vx = 0;
                            }
                        }
                        if (curExplosion->particles[i].vy < 0) {
                            curExplosion->particles[i].vy += (double)PARTICLE_DECEL_Y;
                            if (curExplosion->particles[i].vy > 0) {
                                curExplosion->particles[i].vy = 0;
                            }
                        } else {
                            curExplosion->particles[i].vy -= (double)PARTICLE_DECEL_Y;
                            if (curExplosion->particles[i].vy < 0) {
                                curExplosion->particles[i].vy = 0;
                            }
                        }
                        
                        curExplosion->particles[i].counter = 0;
                    }
                }
            }
            
            if (curExplosion != NULL) {
                curExplosion = curExplosion->next;
            }
            if (curExplosion == NULL) {
                break;
            }
        }
    }
}
