/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwengine.cpp): Graphical Engine, Main Game Loop
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>
#include <dinput.h>
#include <digifx.h>
#include "defines.h"
#include "hajiworld.h"
#include "hwddraw.h"
#include "hwdsound.h"
#include "hwdinput.h"
#include "hwdigifx.h"
#include "hwengine.h"
#include "hwentity.h"
#include "hwworld.h"
#include "hwenemy.h"
#include "hwobjectile.h"
#include "hwplayer.h"
#include "hwgfx.h"
#include "hwinput.h"
#include "hwmidi.h"
#include "hwutil.h"
#include "hwpickup.h"
#include "hwsoundfx.h"
#include "hwlevel.h"
#include "hwexplosion.h"

void UpdateXGIntro(BOOL *ttl);
void UpdatePPIntro(BOOL *ttl);
void UpdateMRIntro(BOOL *ttl);
void UpdateHWIntro(BOOL *ttl);
void UpdateCharsel(BOOL *ttl);
void UpdateGame(void);
void UpdateEndLevel(void);
void RenderForeground(void);
void RenderTop(void);
void RenderHaji(void);
void RenderEnemies(void);
void RenderPickups(void);
void RenderObjectiles(void);
void RenderPowerBar(void);
void RenderLives(void);
void RenderScore(void);
void RenderCurryCount(void);
void RenderBackground(void);
void RenderExplosions(void);

LARGE_INTEGER   g_liStartTicks, g_liEndTicks, g_liFreq;
double          g_SecsPerBlt, g_Secs = 0;
long                g_Blts = 0;

void GameMain(void)
{
    BOOL ttl = TRUE; // time to live?
    
    UpdateInput(); // Update all input related variables for the game
    
    QueryPerformanceFrequency(&g_liFreq);
    
    // What game state are we in?
    switch (Game_State) {
        case NOTHING:
            break;
            // Load stuff FOOL!
        case INIT:
#ifdef NOINTRO
            DDFillSurface(lpDDSPrimary, 0);
            DDFillSurface(lpDDSBack, 0);
            MidiPlay(".\\Music\\Havangla.mid", 1);
            Game_State = CHARSEL;
#else
            DDFillSurface(lpDDSPrimary, 0);
            DDFillSurface(lpDDSBack, 0);
            LoadIntroGFX();
            Game_State = XGINTRO;
#endif
            break;
            // We are currently in the XGINTRO phase
        case XGINTRO:
            // Update the XG screen
            UpdateXGIntro(&ttl);
            // if the player hits enter go on to next phase
            if (keyboard_state[DIK_RETURN] || !ttl) {
                Game_State = PPINTRO; // set Game_State to next phase
                MidiStop();
                MidiPlay(".\\Music\\Willtell.mid", 1);
                AlphaTransition(lpDDSXG, lpDDSPP); // do an alpha transition into next screen
            }
            break;
            // we are currently in the PPINTRO phase
        case PPINTRO:
            // Update the PP screen
            UpdatePPIntro(&ttl);
            // if the player hits enter go on to next phase
            if (keyboard_state[DIK_RETURN] || !ttl) {
                Game_State = MRINTRO; // set Game_State to next phase
                AlphaTransition(lpDDSPP, lpDDSMR); // do an alpha transition into next screen
            }
            break;
            // We are currently in the MRINTRO phase
        case MRINTRO:
            // Update the MR screen
            UpdateMRIntro(&ttl);
            if (keyboard_state[DIK_RETURN] || !ttl) {
                Game_State = HWINTRO; // set Game_State to next phase
                MidiStop();
                MidiPlay(".\\Music\\Funmario.mid", 1);
                AlphaTransition(lpDDSMR, lpDDSHW); // do an alpha transition into next screen
            }
            break;
            // We are currently in the HWINTRO phase
        case HWINTRO:
            // Update the hw screen
            UpdateHWIntro(&ttl);
            // if the player hits enter go on to next phase
            if (keyboard_state[DIK_RETURN] || !ttl) {
                Game_State = CHARSEL; // set the Game_State to the next phase
                FadeToBlack(); // do another alphatransition to the next screen
                KillIntroGFX();
                DDFillSurface(lpDDSPrimary, 0);
                DDFillSurface(lpDDSBack, 0);
                MidiStop();
                MidiPlay(".\\Music\\Havangla.mid", 1);
            }
            break;
        case CHARSEL:
            UpdateCharsel(&ttl);
            if (!ttl) {
                Game_State = PLAYING;
                
                FadeToBlack();
                
                // load the level and game resources
                LoadGameGFX();
                LoadSoundFX();
                InitLevel(1);
                InitPlayer();
            }
            break;
            // We are playing the game!
        case PLAYING:
            UpdateGame();
            if (Game_State == ENDLEVEL) {
                StopAllSounds();
                FadeToBlack();
                memset(Bitplane.bitsPtr, 0, 640 * 480 * 2);
                g_VOX = g_VOY = 0;
            }
            if (keyboard_state[DIK_ESCAPE]) {
                FadeToBlack();
                
                // Kill the game level and resources
                KillPlayer();
                KillLevel();
                ExitGame();
            }
            break;
        case ENDLEVEL:
            UpdateEndLevel();
            if (keyboard_state[DIK_ESCAPE]) {
                FadeToBlack();
                
                // Kill the game level and resources
                KillPlayer();
                KillLevel();
                ExitGame();
            }
            break;
    }
}

void ExitGame(void)
{
    KillGameGFX();
    KillSoundFX();
    
    PostMessage(g_hWnd, WM_DESTROY, 0, 0); // Post the destroy window message
    Game_State = NOTHING; // set the game state to nothing
}

void InitStuff(void)
{
    DFXSetupBitplane();
    ReadPlayerNFO();
    ReadEnemyNFO();
    ReadObjectNFO();
    ReadPickupNFO();
    ReadObjectileNFO();
    LoadAppGFX();
}

void KillStuff(void)
{
    WriteError("\n	Frames Per Second: %lf", g_Blts / g_Secs);
    KillAppGFX();
    DeletePlayerNFO();
    DeleteEnemyNFO();
    DeleteObjectNFO();
    DeletePickupNFO();
    DeleteObjectileNFO();
    DFXKillBitplane();
}

void UpdateXGIntro(BOOL *ttl)
{
    RECT SrcRect; // Source And Destination Rectangles For Blit
    static int loaded = 0; // Variable To Determine If Function Is Being Run For The First Time
    static int count = 0;  // used for timing purposes
    
    // Initialize the clock for this iteration
    StartClock();
    
    // Determine If This Is The First Run
    if (!loaded) {
        // Set Source And Destinationg Rectangles The Size Of The Screen
        SetRect(&SrcRect, 0, 0, 640, 480);
        // Blit The XGames PIC
#ifdef WINDOWED
        lpDDSPrimary->Blt(&g_rcWindow, lpDDSXG, &SrcRect, DDBLT_WAIT, NULL);
#else
        lpDDSPrimary->Blt(&SrcRect, lpDDSXG, &SrcRect, DDBLT_WAIT, NULL);
#endif
        PlaySound(FX_XLOGO, 0);
        loaded = 1; // Stop This Code From Happening Again
    }
    
    // Wait for 1/10 of a second
    WaitClock(100);
    
    // see if it's time to kill this intro phase
    if (++count > 49) {
        *ttl = FALSE;
    } else {
        *ttl = TRUE;
    }
}

void UpdatePPIntro(BOOL *ttl)
{
    static int count = 0;    // used for timing purposes
    
    // Initialize the clock for this iteration
    StartClock();
    
    // Wait for 1/10 of a second
    WaitClock(100);
    
    // see if it's time to kill this intro phase
    if (++count > 49) {
        *ttl = FALSE;
    } else {
        *ttl = TRUE;
    }
}

void UpdateMRIntro(BOOL *ttl)
{
    static int count = 0;    // used for timing purposes
    
    // Initialize the clock for this iteration
    StartClock();
    
    // Wait for 1/10 of a second
    WaitClock(100);
    
    // see if it's time to kill this intro phase
    if (++count > 49) {
        *ttl = FALSE;
    } else {
        *ttl = TRUE;
    }
}

void UpdateHWIntro(BOOL *ttl)
{
    static int count = 0;  // used for timing purposes
    
    // Initialize the clock for this iteration
    StartClock();
    
    // Wait for 1/10 of a second
    WaitClock(100);
    
    // see if it's time to kill this intro phase
    if (++count > 49) {
        *ttl = FALSE;
    } else {
        *ttl = TRUE;
    }
}

void UpdateCharsel(BOOL *ttl)
{
    static trans = 30;
    static dire = 5;
    
    BlitFX.FXType = BLITFX_NONE;
    drawSprite(&sprCharsel, 0, 0);
    
    if (butLeft) {
        player_whois = WHOIS_HAJI;
    }
    if (butRight) {
        player_whois = WHOIS_SUMI;
    }
    
    if (player_whois == WHOIS_HAJI) {
        BlitFX.FXType = BLITFX_BLEND;
        trans += dire;
        if (trans >= 150) {
            dire = -dire;
        } else if (trans <= 30) {
            dire = -dire;
        }
        BlitFX.BlendSrcFactor = trans;
        BlitFX.BlendDstFactor = 100;
        drawSprite(&sprCharselHaji, 35, 78);
        BlitFX.BlendSrcFactor = 30;
        BlitFX.BlendDstFactor = 100;
        drawSprite(&sprCharselSumi, 372, 91);
    } else if (player_whois == WHOIS_SUMI) {
        BlitFX.FXType = BLITFX_BLEND;
        trans += dire;
        if (trans >= 150) {
            dire = -dire;
        } else if (trans <= 30) {
            dire = -dire;
        }
        BlitFX.BlendSrcFactor = trans;
        BlitFX.BlendDstFactor = 100;
        drawSprite(&sprCharselSumi, 372, 91);
        BlitFX.BlendSrcFactor = 30;
        BlitFX.BlendDstFactor = 100;
        drawSprite(&sprCharselHaji, 35, 78);
    }
    
    DDFlip();
    
    if (!butJump) {
        *ttl = TRUE;
    } else {
        *ttl = FALSE;
    }
}

void UpdateGame(void)
{
    QueryPerformanceCounter(&g_liStartTicks);
    
    // Build the active enemy list
    BuildEnemyList();
    
    // Build the active moving polygon list
    BuildMovePolyList();
    
    // Build the active pickup list
    BuildPickupList();
    
    // Process the active moving polygon list
    ProcessMovePolyList();
    
    // Update haji
    ProcessHaji();
    
    // Process the active enemy list
    ProcessEnemyList();
    
    // Process the active Pickup list
    ProcessPickupList();
    
    // Process the objectile list
    ProcessObjectileList();
    
    // Process the objects (animate)
    ProcessObjects();
    
    // Animate the explosions
    AnimateExplosions();
    
    // Render the background layer
    RenderBackground();
    
    // Render the foreground layer
    RenderForeground();
    
    // Render the pickups
    RenderPickups();
    
    // Render the explosions;
    RenderExplosions();
    
    // Render the Haji man....
    RenderHaji();
    
    // Render the enemies
    RenderEnemies();
    
    // Render the objectiles
    RenderObjectiles();
    
    // Render the top (most) layer
    RenderTop();
    
    // clear the info area
    BlitFX.FXType = BLITFX_NONE;
    BlitFX.Color.R = 0;
    BlitFX.Color.G = 0;
    BlitFX.Color.B = 0;
    RECT rcInfoRect = { 0, 0, 640, 32};
    digifxDrawRect(DFXHnd, &rcInfoRect, 0, 0, &BlitFX, &Bitplane);
    
    // Render the lives info
    RenderLives();
    
    // Render the power bar info
    RenderPowerBar();
    
    // Render the current score info
    RenderScore();
    
    // Render the number of objectiles the player has
    RenderCurryCount();
    
    // Show the screen
    DDFlip();
    
    g_SecsPerBlt = ((double)(g_liEndTicks.QuadPart - g_liStartTicks.QuadPart)) / ((double)(g_liFreq.QuadPart));
    while (g_SecsPerBlt < 0.03125) {
        QueryPerformanceCounter(&g_liEndTicks);
        g_SecsPerBlt = ((double)(g_liEndTicks.QuadPart - g_liStartTicks.QuadPart)) / ((double)(g_liFreq.QuadPart));
    }
    g_Secs += ((double)(g_liEndTicks.QuadPart - g_liStartTicks.QuadPart)) / ((double)(g_liFreq.QuadPart));
    g_Blts++;
}

void UpdateEndLevel(void)
{
    static expcount = 0;
    
    memset(Bitplane.bitsPtr, 0, 640 * 480 * 2);
    
    QueryPerformanceCounter(&g_liStartTicks);
    
    expcount++;
    if (expcount > 3) {
        expcount = 0;
        AddExplosion(rand() % 640, rand() % 480, 255, 180, 0, 255, 0, 0);
    }
    
    // Animate the explosions
    AnimateExplosions();
    
    // Render the explosions;
    RenderExplosions();
    
    // Show the screen
    DDFlip();
    
    g_SecsPerBlt = ((double)(g_liEndTicks.QuadPart - g_liStartTicks.QuadPart)) / ((double)(g_liFreq.QuadPart));
    while (g_SecsPerBlt < 0.03125) {
        QueryPerformanceCounter(&g_liEndTicks);
        g_SecsPerBlt = ((double)(g_liEndTicks.QuadPart - g_liStartTicks.QuadPart)) / ((double)(g_liFreq.QuadPart));
    }
    g_Secs += ((double)(g_liEndTicks.QuadPart - g_liStartTicks.QuadPart)) / ((double)(g_liFreq.QuadPart));
    g_Blts++;
}

void RenderForeground(void)
{
    OBJECT *curObject;
    
    curObject = level.foregroundLst;
    if (curObject) {
        while (1) {
            if (nfoObject[curObject->id].trans == 100) {
                BlitFX.FXType = BLITFX_NONE;
                drawSprite(&sprObjects[curObject->id][curObject->frame], curObject->x - g_VOX, curObject->y - g_VOY + 32);
                BlitFX.FXType = BLITFX_HENDS | BLITFX_SOFTEN;
                drawSprite(&sprObjects[curObject->id][curObject->frame], curObject->x - g_VOX, curObject->y - g_VOY + 32);
            } else {
                BlitFX.FXType = BLITFX_BLEND;
                BlitFX.BlendSrcFactor = nfoObject[curObject->id].trans;
                BlitFX.BlendDstFactor = 50;
                drawSprite(&sprObjects[curObject->id][curObject->frame], curObject->x - g_VOX, curObject->y - g_VOY + 32);
            }
            if (curObject->next == NULL) {
                break;
            }
            curObject = curObject->next;
        }
    }
}

void RenderTop(void)
{
    OBJECT *curObject;
    
    curObject = level.topLst;
    if (curObject) {
        while (1) {
            if (nfoObject[curObject->id].trans == 100) {
                BlitFX.FXType = BLITFX_NONE;
                drawSprite(&sprObjects[curObject->id][curObject->frame], curObject->x - g_VOX, curObject->y - g_VOY + 32);
                BlitFX.FXType = BLITFX_HENDS | BLITFX_SOFTEN;
                drawSprite(&sprObjects[curObject->id][curObject->frame], curObject->x - g_VOX, curObject->y - g_VOY + 32);
            } else {
                BlitFX.FXType = BLITFX_BLEND;
                BlitFX.BlendSrcFactor = nfoObject[curObject->id].trans;
                BlitFX.BlendDstFactor = 100;
                drawSprite(&sprObjects[curObject->id][curObject->frame], curObject->x - g_VOX, curObject->y - g_VOY + 32);
            }
            if (curObject->next == NULL) {
                break;
            }
            curObject = curObject->next;
        }
    }
}

void RenderHaji(void)
{
    int x, y, num, i, xoff = 0, yoff = 0;
    
    y = (341) + (level.height - (g_VOY + ((level.sectors_y - 1) * 448)));
    BlitFX.FXType = BLITFX_BLEND;
    BlitFX.BlendSrcFactor = 100;
    BlitFX.BlendDstFactor = 25;
    drawSprite(&sprPlayer[player.animindex + nfoPlayer[player_whois].pics], player.absx - 10, y);
    BlitFX.FXType = BLITFX_HENDS | BLITFX_SOFTEN;
    drawSprite(&sprPlayer[player.animindex + nfoPlayer[player_whois].pics], player.absx - 10, y);
    
    if (!player.invulnerable) {
        BlitFX.FXType = BLITFX_NONE;
        drawSprite(&sprPlayer[player.animindex], player.absx, player.absy);
        BlitFX.FXType = BLITFX_HENDS | BLITFX_SOFTEN;
        drawSprite(&sprPlayer[player.animindex], player.absx, player.absy);
    } else {
        BlitFX.FXType = BLITFX_BLEND;
        BlitFX.BlendSrcFactor = player.invcount;
        BlitFX.BlendDstFactor = 100 - player.invcount;
        drawSprite(&sprPlayer[player.animindex], player.absx, player.absy);
        BlitFX.FXType = BLITFX_HENDS | BLITFX_SOFTEN;
        drawSprite(&sprPlayer[player.animindex], player.absx, player.absy);
    }
    
    BlitFX.FXType = BLITFX_BLEND;
    BlitFX.BlendSrcFactor = 15;
    BlitFX.BlendDstFactor = 100;
    if (player.state == ES_SKIDDING) {
        num = rand() % 4 + 1;
        if (player.direction == D_RIGHT) {
            for (i = 0; i < num; i++) {
                x = player.absx + (nfoPlayer[player_whois].width - (nfoPlayer[player_whois].width / 4)) + rand() % 10 + xoff;
                xoff += x - (player.absx + (nfoPlayer[player_whois].width - (nfoPlayer[player_whois].width / 4)));
                y = player.absy + (nfoPlayer[player_whois].height - (nfoPlayer[player_whois].height / 6)) - rand() % 5 - yoff;
                yoff += (player.absy + (nfoPlayer[player_whois].height - (nfoPlayer[player_whois].height / 6))) - y;
                drawSprite(&sprSkidSmoke, x, y);
            }
        } else {
            for (i = 0; i < num; i++) {
                x = player.absx - rand() % 10 - xoff;
                xoff += player.absx - x;
                y = player.absy + (nfoPlayer[player_whois].height - (nfoPlayer[player_whois].height / 6)) - rand() % 5 - yoff;
                yoff += (player.absy + (nfoPlayer[player_whois].height - (nfoPlayer[player_whois].height / 6))) - y;
                drawSprite(&sprSkidSmoke, x, y);
            }
        }
    }
}

void RenderEnemies(void)
{
    ENEMY *curEnemy;
    
    curEnemy = level.enemyLst;
    
    if (curEnemy != NULL) {
        while (1) {
            BlitFX.FXType = BLITFX_NONE;
            drawSprite(&sprEnemies[curEnemy->id][curEnemy->frame], curEnemy->absx, curEnemy->absy);
            BlitFX.FXType = BLITFX_HENDS | BLITFX_SOFTEN;
            drawSprite(&sprEnemies[curEnemy->id][curEnemy->frame], curEnemy->absx, curEnemy->absy);
            curEnemy = curEnemy->next;
            if (curEnemy == NULL) {
                break;
            }
        }
    }
}

void RenderPickups(void)
{
    PICKUP *curPickup;
    
    curPickup = level.pickupLst;
    
    if (curPickup != NULL) {
        while (1) {
            if (curPickup->state != ES_DYING) {
                BlitFX.FXType = BLITFX_NONE;
                drawSprite(&sprPickups[curPickup->id][curPickup->frame], curPickup->absx, curPickup->absy);
                BlitFX.FXType = BLITFX_HENDS | BLITFX_SOFTEN;
                drawSprite(&sprPickups[curPickup->id][curPickup->frame], curPickup->absx, curPickup->absy);
            } else {
                BlitFX.FXType = BLITFX_BLEND;
                BlitFX.BlendSrcFactor = curPickup->trans;
                BlitFX.BlendDstFactor = 100;
                drawSprite(&sprPickups[curPickup->id][curPickup->frame], curPickup->absx, curPickup->absy);
            }
            curPickup = curPickup->next;
            if (curPickup == NULL) {
                break;
            }
        }
    }
}

void RenderObjectiles(void)
{
    OBJECTILE *curObjectile;
    
    curObjectile = objectiles;
    
    if (curObjectile != NULL) {
        while (1) {
            BlitFX.FXType = BLITFX_NONE;
            drawSprite(&sprObjectiles[curObjectile->type][curObjectile->frame], curObjectile->relx - g_VOX, curObjectile->rely - g_VOY + 32);
            curObjectile = curObjectile->next;
            if (curObjectile == NULL) {
                break;
            }
        }
    }
}

void RenderPowerBar(void)
{
    BlitFX.FXType = BLITFX_NONE;
    drawSprite(&sprBar, 100, 7);
    BlitFX.FXType = BLITFX_NONE;
    RECT rcRect = { 0, 0, 100 - player.power, 18 };
    digifxDrawRect(DFXHnd, &rcRect, 100 + player.power, 7, &BlitFX, &Bitplane);
}

void RenderLives(void)
{
    char sTmp[9], cur;
    int numchars, i, left;
    
    itoa(player.lives, sTmp, 10);
    numchars = strlen(sTmp);
    left = 48;
    
    BlitFX.FXType = BLITFX_NONE;
    
    drawSprite(&sprLives, 0, 0);
    
    for (i = 0; i < numchars; i++) {
        cur = sTmp[i];
        drawSprite(&sprDigits[atoi(&cur)], left, 0);
        left += 18;
    }
}

void RenderScore(void)
{
    char sTmp[9], cur;
    int numchars, i, left;
    
    itoa(player.score, sTmp, 10);
    numchars = strlen(sTmp);
    left = 640 - (numchars * 18);
    
    BlitFX.FXType = BLITFX_NONE;
    
    for (i = 0; i < numchars; i++) {
        cur = sTmp[i];
        drawSprite(&sprDigits[atoi(&cur)], left, 0);
        left += 18;
    }
}

void RenderCurryCount(void)
{
    char sTmp[9], cur;
    int numchars, i, left;
    
    itoa(player.objectiles, sTmp, 10);
    numchars = strlen(sTmp);
    left = 282;
    
    BlitFX.FXType = BLITFX_NONE;
    
    drawSprite(&sprCurry, 250, 0);
    
    for (i = 0; i < numchars; i++) {
        cur = sTmp[i];
        drawSprite(&sprDigits[atoi(&cur)], left, 0);
        left += 18;
    }
}

void RenderBackground(void)
{
    int x, y;
    RECT rcSky = { 0, 0, 640, 448 };
    
    BlitFX.FXType = BLITFX_NONE;
    
    BlitFX.Color.R = 19;
    BlitFX.Color.G = 90;
    BlitFX.Color.B = 163;
    digifxDrawRect(DFXHnd, &rcSky, 0, 32, &BlitFX, &Bitplane);
    
    y = -g_VOY + 32;
    drawSprite(&sprSky, 0, y);
    
    x = (g_VOX / 5) % 640;
    y = (480 - sprGround.rlehdr.Hgh + 48 - sprMiddle.rlehdr.Hgh - sprMountains.rlehdr.Hgh) + (level.height - (g_VOY + ((level.sectors_y - 1) * 448)) - (((level.height - 448) - g_VOY) / 5));
    drawSprite(&sprMountains, -x, y);
    drawSprite(&sprMountains, 640 - x, y);
    
    x = (g_VOX / 3) % 640;
    y = (480 - sprGround.rlehdr.Hgh + 32 - sprMiddle.rlehdr.Hgh) + (level.height - (g_VOY + ((level.sectors_y - 1) * 448)) - (((level.height - 448) - g_VOY) / 5));
    drawSprite(&sprMiddle, -x, y);
    drawSprite(&sprMiddle, 640 - x, y);
    
    x = g_VOX % 640;
    y = (480 - sprGround.rlehdr.Hgh) + (level.height - (g_VOY + ((level.sectors_y - 1) * 448)));
    drawSprite(&sprGround, -x, y);
    drawSprite(&sprGround, 640 - x, y);
}

void RenderExplosions(void)
{
    RECT expRect = { 0, 0, 1, 1 };
    EXPLOSION *curExplosion;
    int i, x, y, px, py;
    
    curExplosion = explosions;
    
    if (curExplosion != NULL) {
        while (1) {
            x = curExplosion->x - g_VOX;
            y = curExplosion->y - g_VOY + 32;
            
            BlitFX.FXType = BLITFX_MONO | BLITFX_BLEND;
            
            for (i = 0; i < NUM_PARTICLES; i++) {
                px = x + (int)curExplosion->particles[i].x;
                py = y + (int)curExplosion->particles[i].y;
                if (i == NUM_PARTICLES_LONG) {
                    expRect.right = expRect.bottom = 0;
                }
                if (((py >= 0) && (py < 480)) && ((px >= 0) && (px < 640))) {
                    BlitFX.Color.R = curExplosion->particles[i].r;
                    BlitFX.Color.G = curExplosion->particles[i].g;
                    BlitFX.Color.B = curExplosion->particles[i].b;
                    BlitFX.BlendSrcFactor = curExplosion->particles[i].trans;
                    BlitFX.BlendDstFactor = 100;
                    digifxDrawRect(DFXHnd, &expRect, px, py, &BlitFX, &Bitplane);
                }
            }
            
            BlitFX.FXType = BLITFX_BLEND;
            BlitFX.BlendSrcFactor = (int)(curExplosion->lifetime * 2.5);
            BlitFX.BlendDstFactor = 100;
            
            drawSprite(&sprSpotlight, x, y);
            
            curExplosion = curExplosion->next;
            if (curExplosion == NULL) {
                break;
            }
        }
    }
}