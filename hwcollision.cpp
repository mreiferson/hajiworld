/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwcollision.cpp): Collision Detection Functions
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "hwworld.h"
#include "hwplayer.h"
#include "hwcollision.h"

int Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    if ((((x1 >= x2) && (x1 <= x2 + w2)) ||
            ((x1 + w1 >= x2) && (x1 + w1 <= x2 + w2)) ||
            ((x2 >= x1) && (x2 <= x1 + w1)) ||
            ((x2 + w2 >= x1) && (x2 + w2 <= x1 + w1)))
            &&
            (((y1 >= y2) && (y1 <= y2 + h2)) ||
             ((y1 + h1 >= y2) && (y1 + h1 <= y2 + h2)) ||
             ((y2 >= y1) && (y2 <= y1 + h2)) ||
             ((y2 + h2 >= y1) && (y2 + h2 <= y1 + h1)))) {
        return 1;
    } else {
        return 0;
    }
}

int Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, int *right, int *left, int *top, int *bottom)
{
    *right = *left = *top = *bottom = 0;
    
    if ((x1 >= x2) && ((x2 + w2) >= x1)) {
        *left = ((x2 + w2) - x1) + 1;
    }
    if (((x1 + w1) >= x2) && (x2 >= x1)) {
        *right = ((x1 + w1) - x2) + 1;
    }
    
    if ((y1 >= y2) && (y1 <= (y2 + h2))) {
        *top = ((y2 + h2) - y1) + 1;
    }
    if (((y1 + h1) >= y2) && (y2 >= y1)) {
        *bottom = ((y1 + h1) - y2) + 1;
    }
    
    if ((*left || *right) && (*top || *bottom)) {
        if (((x1 >= x2) && (x1 + w1 <= x2 + w2)) || ((x2 >= x1) && (x2 + w2 <= x1 + w1))) {
            *right = 0;
            *left = 0;
        }
        if (((y1 >= y2) && (y1 + h1 <= y2 + h2)) || ((y2 >= y1) && (y2 + h2 <= y1 + h1))) {
            *top = 0;
            *bottom = 0;
        }
        return 1;
    } else {
        return 0;
    }
}

int Collision(int x2, int y2, int w2, int h2, int *right, int *left, int *top, int *bottom)
{
    int x1, y1, w1, h1;
    
    if (player.state == ES_DUCKING) {
        x1 = COLLISION_DX;
        y1 = COLLISION_DY;
        w1 = COLLISION_DW;
        h1 = COLLISION_DH;
    } else if (player.state == ES_JUMPINGUP || player.state == ES_JUMPINGDOWN || player.state == ES_JUMPATTACK) {
        x1 = COLLISION_JX;
        y1 = COLLISION_JY;
        w1 = COLLISION_JW;
        h1 = COLLISION_JH;
    } else {
        x1 = COLLISION_X;
        y1 = COLLISION_Y;
        w1 = COLLISION_W;
        h1 = COLLISION_H;
    }
    
    *right = *left = *top = *bottom = 0;
    
    if ((x1 >= x2) && ((x2 + w2) >= x1)) {
        *left = ((x2 + w2) - x1) + 1;
    }
    if (((x1 + w1) >= x2) && (x2 >= x1)) {
        *right = ((x1 + w1) - x2) + 1;
    }
    
    if ((y1 >= y2) && (y1 <= (y2 + h2))) {
        *top = ((y2 + h2) - y1) + 1;
    }
    if (((y1 + h1) >= y2) && (y2 >= y1)) {
        *bottom = ((y1 + h1) - y2) + 1;
    }
    
    if ((*left || *right) && (*top || *bottom)) {
        if (((x1 >= x2) && (x1 + w1 <= x2 + w2)) || ((x2 >= x1) && (x2 + w2 <= x1 + w1))) {
            *right = 0;
            *left = 0;
        }
        if (((y1 >= y2) && (y1 + h1 <= y2 + h2)) || ((y2 >= y1) && (y2 + h2 <= y1 + h1))) {
            *top = 0;
            *bottom = 0;
        }
        return 1;
    } else {
        return 0;
    }
}

int Collision(int x1, int y1, int w1, int h1, POLY *poly2)
{
    int x2, y2, w2, h2;
    
    x2 = poly2->verts[0].x;
    y2 = poly2->verts[0].y;
    
    w2 = poly2->verts[1].x - x2;
    h2 = poly2->verts[3].y - y2;
    
    if ((((x1 >= x2) && (x1 <= x2 + w2)) ||
            ((x1 + w1 >= x2) && (x1 + w1 <= x2 + w2)) ||
            ((x2 >= x1) && (x2 <= x1 + w1)) ||
            ((x2 + w2 >= x1) && (x2 + w2 <= x1 + w1)))
            &&
            (((y1 >= y2) && (y1 <= y2 + h2)) ||
             ((y1 + h1 >= y2) && (y1 + h1 <= y2 + h2)) ||
             ((y2 >= y1) && (y2 <= y1 + h1)) ||
             ((y2 + h2 >= y1) && (y2 + h2 <= y1 + h1)))) {
        return 1;
    } else {
        return 0;
    }
}

int Collision(int x1, int y1, int w1, int h1, POLY *poly2, int *right, int *left, int *top, int *bottom)
{
    int x2, y2, w2, h2;
    
    *right = *left = *top = *bottom = 0;
    
    x2 = poly2->verts[0].x;
    y2 = poly2->verts[0].y;
    
    w2 = poly2->verts[1].x - x2;
    h2 = poly2->verts[3].y - y2;
    
    if ((x1 >= x2) && ((x2 + w2) >= x1)) {
        *left = ((x2 + w2) - x1) + 1;
    }
    if (((x1 + w1) >= x2) && (x2 >= x1)) {
        *right = ((x1 + w1) - x2) + 1;
    }
    
    if ((y1 >= y2) && (y1 <= (y2 + h2))) {
        *top = ((y2 + h2) - y1) + 1;
    }
    if (((y1 + h1) >= y2) && (y2 >= y1)) {
        *bottom = ((y1 + h1) - y2) + 1;
    }
    
    if ((*left || *right) && (*top || *bottom)) {
        if (((x1 >= x2) && (x1 + w1 <= x2 + w2)) || ((x2 >= x1) && (x2 + w2 <= x1 + w1))) {
            *right = 0;
            *left = 0;
        }
        if (((y1 >= y2) && (y1 + h1 <= y2 + h2)) || ((y2 >= y1) && (y2 + h2 <= y1 + h1))) {
            *top = 0;
            *bottom = 0;
        }
        return 1;
    } else {
        return 0;
    }
}

int Collision(POLY *poly2, int *right, int *left, int *top, int *bottom)
{
    int x1, y1, w1, h1;
    int x2, y2, w2, h2;
    
    *right = *left = *top = *bottom = 0;
    
    if (player.state == ES_DUCKING) {
        x1 = COLLISION_DX;
        y1 = COLLISION_DY;
        w1 = COLLISION_DW;
        h1 = COLLISION_DH;
    } else if (player.state == ES_JUMPINGUP || player.state == ES_JUMPINGDOWN || player.state == ES_JUMPATTACK) {
        x1 = COLLISION_JX;
        y1 = COLLISION_JY;
        w1 = COLLISION_JW;
        h1 = COLLISION_JH;
    } else {
        x1 = COLLISION_X;
        y1 = COLLISION_Y;
        w1 = COLLISION_W;
        h1 = COLLISION_H - 1;
    }
    
    x2 = poly2->verts[0].x;
    y2 = poly2->verts[0].y;
    
    w2 = poly2->verts[1].x - x2;
    h2 = poly2->verts[3].y - y2;
    
    if ((x1 >= x2) && ((x2 + w2) >= x1)) {
        *left = ((x2 + w2) - x1) + 1;
    }
    if (((x1 + w1) >= x2) && (x2 >= x1)) {
        *right = ((x1 + w1) - x2) + 1;
    }
    
    if ((y1 >= y2) && (y1 <= (y2 + h2))) {
        *top = ((y2 + h2) - y1) + 1;
    }
    if (((y1 + h1) >= y2) && (y2 >= y1)) {
        *bottom = ((y1 + h1) - y2) + 1;
    }
    
    if ((*left || *right) && (*top || *bottom)) {
        if (((x1 >= x2) && (x1 + w1 <= x2 + w2)) || ((x2 >= x1) && (x2 + w2 <= x1 + w1))) {
            *right = 0;
            *left = 0;
        }
        if (((y1 >= y2) && (y1 + h1 <= y2 + h2)) || ((y2 >= y1) && (y2 + h2 <= y1 + h1))) {
            *top = 0;
            *bottom = 0;
        }
        return 1;
    } else {
        return 0;
    }
}