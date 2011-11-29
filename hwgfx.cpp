/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwgfx.cpp): Graphical Functions, Loading/Restoring, Blitting
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
#include <math.h>
#include "defines.h"
#include "hajiworld.h"
#include "hwddraw.h"
#include "hwdigifx.h"
#include "hwentity.h"
#include "hwworld.h"
#include "hwenemy.h"
#include "hwplayer.h"
#include "hwobjectile.h"
#include "hwgfx.h"
#include "hwmemcpy.h"
#include "hwpickup.h"
#include "hwutil.h"
#include "hwplayer.h"

// Intro Surfaces
LPDIRECTDRAWSURFACE     lpDDSXG;
LPDIRECTDRAWSURFACE     lpDDSPP;
LPDIRECTDRAWSURFACE     lpDDSMR;
LPDIRECTDRAWSURFACE     lpDDSHW;

// Temporary Surfaces Only Needed In HWGFX.CPP
LPDIRECTDRAWSURFACE     lpDDSRef;       // Reference Surface
LPDIRECTDRAWSURFACE     lpDDSTmp;       // Temporary Transition Surface
LPDIRECTDRAWSURFACE     lpDDSSrc;       // Source Surface For A Transition
LPDIRECTDRAWSURFACE     lpDDSDes;       // Destination Surface For A Transition

// character selection graphics
SPRITE                  sprCharsel;
SPRITE                  sprCharselHaji;
SPRITE                  sprCharselSumi;

// Game graphics
SPRITE                      sprBar;
SPRITE                      sprLives;
SPRITE                      sprDigits[10];
SPRITE                      sprSky;
SPRITE                      sprGround;
SPRITE                      sprMiddle;
SPRITE                      sprMountains;
SPRITE                      sprSkidSmoke;
SPRITE                      sprCurry;
SPRITE                      sprSpotlight;
SPRITE                      *sprPlayer;
SPRITE                      **sprObjects;
SPRITE                      **sprEnemies;
SPRITE                      **sprPickups;
SPRITE                      **sprObjectiles;

// transparency and blending lookup table
WORD                            PixelShade[32][65536]; // Precomputed shaded pixels

/*
 * RestoreGraphics:
 *    Restores the GFX if the surface was lost
 */
void RestoreGraphics(void)
{
    if (lpDDSPrimary) {
        lpDDSPrimary->Restore();
    }
    if (lpDDSBack) {
        lpDDSBack->Restore();
    }
}

/*
 * InitPixelShade:
 *      Fills the PixelShade array with precomputed shades of every possible pixel (32 shades)
 */
void InitPixelShade(void)
{
    int i, j;
    int r, g, b;
    int dr, dg, db;
    const double alpha[32] = { 0.0, 0.03, 0.06, 0.09,
                               0.13, 0.17, 0.21, 0.24,
                               0.27, 0.31, 0.34, 0.37,
                               0.41, 0.44, 0.47, 0.49,
                               0.51, 0.53, 0.56, 0.59,
                               0.63, 0.66, 0.69, 0.73,
                               0.76, 0.79, 0.83, 0.87,
                               0.91, 0.94, 0.97, 1.0
                             };
                             
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 65536; j++) {
            r = RED(j);
            g = GREEN(j);
            b = BLUE(j);
            dr = (int)(r * alpha[i]);
            dg = (int)(g * alpha[i]);
            db = (int)(b * alpha[i]);
            PixelShade[i][j] = RGB16(dr, dg, db);
        }
    }
}

/*
 * AlphaTransition:
 *    Does an alpha transition from Src -> Des
 */
void AlphaTransition(LPDIRECTDRAWSURFACE Src, LPDIRECTDRAWSURFACE Des)
{
    long i;                          // index into surfaces
    int alpha;                  // Holds current alpha value
    int dpitch, spitch, tpitch, ppitch; // surface pitch for destination, source, temp surfaces
    WORD *AlphaPTR;              // pointer to the current AlphaMap level (Source)
    WORD *InvAlphaPTR;           // the inverted pointer to the current AlphaMap level (Destination)
    WORD *src, *des, *tmp, *prm;
    WORD *fastsrc, *fastdes, *fasttmp;      // Surface memory pointer for source, destination, and temporary surfaces
    RECT SrcRect, DesRect;      // Source and destination rectangles
    
    // Set source and destination rectangles to the screen size
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    
    // Create the three surface we are going to use
    lpDDSTmp = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // The temporary surface
    lpDDSSrc = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // The source surface
    lpDDSDes = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // The destination surface
    
    // Blit the transition surfaces into out newly created source/destination surfaces
    lpDDSSrc->Blt(&DesRect, Src, &SrcRect, DDBLT_WAIT, NULL); // Blit Src->lpDDSSrc
    lpDDSDes->Blt(&DesRect, Des, &SrcRect, DDBLT_WAIT, NULL); // Blit Des->lpDDSDes
    
    // lock all three surfaces temporary, source, and destination
    des = DDLockSurface(lpDDSDes, &dpitch);
    src = DDLockSurface(lpDDSSrc, &spitch);
    tmp = DDLockSurface(lpDDSTmp, &tpitch);
    prm = DDLockSurface(lpDDSPrimary, &ppitch);
    
    // for each alpha level
    for (alpha = 31; alpha >= 0; alpha--) {
        // set AlphaMap pointers to appropriate levels
        AlphaPTR = PixelShade[alpha];
        InvAlphaPTR = PixelShade[31 - alpha];
        
        // "reset" the *fast* pointers to the locked surfaces
        fastsrc = src;
        fastdes = des;
        fasttmp = tmp;
        
        // loop through every pixel
        for (i = 0; i < 307200; i++, fasttmp++, fastsrc++, fastdes++) {
            // Set the new pixel value in temporary surface
            *fasttmp = AlphaPTR[*fastsrc] + InvAlphaPTR[*fastdes];
        }
        
        // copy the temp surface to the primary surface
        // method depends on windowed/full screen
#ifdef WINDOWED
        WORD *fastprm = prm + (g_rcWindow.top * ppitch) + g_rcWindow.left;
        fasttmp = tmp;
        for (i = 0; i < 480; i++, fastprm += ppitch, fasttmp += 640) {
            g_MemCpySys2Vid(fastprm, fasttmp, 1280);    // 1280 = 614400 (see below) / 480
        }
#else
        // (640*480) = 307200 (words) * 2 = 614400 (bytes)
        g_MemCpySys2Vid(prm, tmp, 614400);
#endif
    }
    // Unlock our temporary, source, and destination surfaces
    DDUnlockSurface(lpDDSPrimary);
    DDUnlockSurface(lpDDSTmp);
    DDUnlockSurface(lpDDSDes);
    DDUnlockSurface(lpDDSSrc);
    
    // Release or temporary, source, and destination surfaces
    lpDDSTmp->Release();
    lpDDSTmp = NULL;
    lpDDSSrc->Release();
    lpDDSSrc = NULL;
    lpDDSDes->Release();
    lpDDSDes = NULL;
}

/*
 * FadeToBlack:
 *      Fades a screen to black
 */
void FadeToBlack(void)
{
    RECT SrcRect, DesRect; // Source and Destination Rectangles
    WORD *tmp;             // temporary surface memory pointer
    WORD *ref;
    WORD *prm;
    WORD *fastref, *fasttmp;
    int c, tpitch, rpitch, ppitch;         // incrementing variable, temporary surface pitch
    long i;                // another incrementing variable
    WORD *shade;
    
    // Set source and destination rectangles to size of screen
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    
    // Create our temporary surface
    lpDDSTmp = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY);
    lpDDSRef = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY);
    
    // Blit our primary surface into our temporary SYSTEM MEMORY surface
#ifdef WINDOWED
    lpDDSRef->Blt(&DesRect, lpDDSPrimary, &g_rcWindow, DDBLT_WAIT, NULL);
#else
    lpDDSRef->Blt(&DesRect, lpDDSPrimary, &SrcRect, DDBLT_WAIT, NULL);
#endif
    
    // Lock our temporary surface
    tmp = DDLockSurface(lpDDSTmp, &tpitch);
    ref = DDLockSurface(lpDDSRef, &rpitch);
    prm = DDLockSurface(lpDDSPrimary, &ppitch);
    
    for (c = 30; c >= 1; c--) {
        // get a pointer indexed to the start of the current shade level
        shade = PixelShade[c];
        
        // "reset" our *fast* surface pointers
        fastref = ref;
        fasttmp = tmp;
        
        // for every pixel on the screen (640*480=307200)
        for (i = 0; i < 307200; i++, fasttmp++, fastref++) {
            // new pixel please....
            *fasttmp = shade[*fastref];
        }
        
        // copy the temp surface to the primary surface
        // method depends on windowed/full screen
#ifdef WINDOWED
        WORD *fastprm = prm + (g_rcWindow.top * ppitch) + g_rcWindow.left;
        fasttmp = tmp;
        for (i = 0; i < 480; i++, fastprm += ppitch, fasttmp += 640) {
            g_MemCpySys2Vid(fastprm, fasttmp, 1280);    // 1280 = 614400 (see below) / 480
        }
#else
        // (640*480) = 307200 (words) * 2 = 614400 (bytes)
        g_MemCpySys2Vid(prm, tmp, 614400);
#endif
    }
    
    // unlock our temporary surface
    DDUnlockSurface(lpDDSTmp);
    DDUnlockSurface(lpDDSRef);
    DDUnlockSurface(lpDDSPrimary);
    
    // just to make sure the screen is black when this routine is over, fill it with 0
    DDFillSurface(lpDDSPrimary, 0);
    
    // release our temporary surface
    lpDDSTmp->Release();
    lpDDSTmp = NULL;
    lpDDSRef->Release();
    lpDDSRef = NULL;
}

/*
 * FadeToSurface:
 *    Fades into a surface from black
 */
void FadeToSurface(LPDIRECTDRAWSURFACE lpDDS)
{
    int c;                 // counter variable
    long i;                // incrementing variable
    WORD *tmp, *ref, *prm;
    WORD *fasttmp, *fastref;       // temporary and destination surface mem pointers
    RECT SrcRect, DesRect; // Source and destination rectangles
    int tpitch, rpitch, ppitch;    // temporary and destination surface pitch
    WORD *shade;
    
    // Set the source and destination rectangles to the size of the screen
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    
    // Create the surfaces
    lpDDSTmp = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // the temporary surface
    lpDDSRef = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // the temporary surface
    lpDDSRef->Blt(&DesRect, lpDDS, &SrcRect, DDBLT_WAIT, NULL); // blit the desired surface into our destination surface
    
    // Lock our surfaces temporary, and destination
    tmp = DDLockSurface(lpDDSTmp, &tpitch);
    prm = DDLockSurface(lpDDSPrimary, &ppitch);
    ref = DDLockSurface(lpDDSRef, &rpitch);
    
    // This can be changed, but it worx out nice to do 10 iterations
    for (c = 1; c <= 30; c++) {
        // get pointer indexed to the start of the current shade level
        shade = PixelShade[c];
        
        // "reset" our *fast* surface pointers
        fasttmp = tmp;
        fastref = ref;
        
        // for every pixel on the screen (640*480=307200)
        for (i = 0; i < 307200; i++, fasttmp++, fastref++) {
            // new pixel please.....
            *fasttmp = shade[*fastref];
        }
        
        // copy the temp surface to the primary surface
        // method depends on windowed/full screen
#ifdef WINDOWED
        WORD *fastprm = prm + (g_rcWindow.top * ppitch) + g_rcWindow.left;
        fasttmp = tmp;
        for (i = 0; i < 480; i++, fastprm += ppitch, fasttmp += 640) {
            g_MemCpySys2Vid(fastprm, fasttmp, 1280);    // 1280 = 614400 (see below) / 480
        }
#else
        // (640*480) = 307200 (words) * 2 = 614400 (bytes)
        g_MemCpySys2Vid(prm, tmp, 614400);
#endif
    }
    
    // unlock the temporary surface and destination surface
    DDUnlockSurface(lpDDSTmp);
    DDUnlockSurface(lpDDSPrimary);
    DDUnlockSurface(lpDDSRef);
    
    // blit the actual destination surface to the primary surface so we're sure
    // the screen is where it should be
#ifdef WINDOWED
    lpDDSPrimary->Blt(&g_rcWindow, lpDDS, &SrcRect, DDBLT_WAIT, NULL);
#else
    lpDDSPrimary->Blt(&DesRect, lpDDS, &SrcRect, DDBLT_WAIT, NULL);
#endif
    
    // release the temporary and destination surfaces
    lpDDSTmp->Release();
    lpDDSTmp = NULL;
    lpDDSRef->Release();
    lpDDSRef = NULL;
}

/*
 * BlitString:
 *    Blits a string to the surface passed in color passed by RGB structure
 */
void BlitString(char *txtstring, int x, int y, RGB_ *rgb)
{
    HDC hdc; // Device Context
    
    // Get the Device Context of the back surface
    if (lpDDSBack->GetDC(&hdc) == DD_OK) {
        // Set the Backgroung color to transparent
        SetBkColor(hdc, TRANSPARENT);
        // Set the text color to whatever is specified in the RGB triplet structure
        SetTextColor(hdc, RGB(rgb->r, rgb->g, rgb->b));
        // Print out the text we passed to the function
        TextOut(hdc, x, y, txtstring, lstrlen(txtstring));
        // Release the device context of the back surface
        lpDDSBack->ReleaseDC(hdc);
    }
}

void CircleFill(BITPLANE *bitplane, int x, int y, int radius)
{
    int a;
    int x1, y1, x2, y2;
    
    for (a = 0; a < 360; a++) {
        x1 = x + (int)(g_CosTab[a] * radius);
        y1 = y + (int)(g_SinTab[a] * radius);
        x2 = x + (int)(g_CosTab[a + 180] * radius);
        y2 = y + (int)(g_SinTab[a + 180] * radius);
        digifxDrawLine(DFXHnd, x1, y1, x2, y2, &BlitFX, bitplane);
    }
}

void LayerDelete(LAYER *the_layer)
{
    killSprite(&the_layer->spr);
}

void LayerBuild(LAYER *dest_layer, IMAGE *img, int width, int height, int source_y, int draw_y)
{
    dest_layer->height = height;
    dest_layer->width = width;
    dest_layer->x = 0;
    dest_layer->y = draw_y;
    
    loadSprite(&dest_layer->spr, img, 0, source_y, width, height);
}

void LayerDraw(LAYER *source_layer)
{
    drawSprite(&source_layer->spr, -source_layer->x, source_layer->y);
    drawSprite(&source_layer->spr, 640 - source_layer->x, source_layer->y);
}

void drawSprite(SPRITE *spr, int x, int y)
{
    digifxDrawRLE(DFXHnd, &spr->rlehdr, x, y, &BlitFX, &Bitplane);
}

void FlipImage(BYTE *ImagePtr, int width, int height)
{
    int dx, dy, pitch = width * 2;
    
    BYTE *DestPtr = (BYTE *)malloc(height * pitch * sizeof(BYTE));
    
    for (dy = 0; dy < height; dy++) {
        for (dx = 0; dx < pitch; dx++) {
            DestPtr[((height - dy - 1)*pitch) + dx] = ImagePtr[(dy * pitch) + dx];
        }
    }
    
    for (dy = 0; dy < height; dy++) {
        for (dx = 0; dx < pitch; dx++) {
            ImagePtr[(dy * pitch) + dx] = DestPtr[(dy * pitch) + dx];
        }
    }
    
    free(DestPtr);
}

void loadImage(IMAGE *img, char *filename)
{
    DWORD BytesCnt;
    HANDLE TmpFile;
    
    TmpFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ReadFile(TmpFile, img->header, 18, &BytesCnt, NULL);
    img->bitplane.bitsWdh = (img->header[13] * 256) + img->header[12];
    img->bitplane.bitsHgh = (img->header[15] * 256) + img->header[14];
    img->bitplane.bitsFmt = PIXFMT_555;
    img->bitplane.bitsPitch = img->bitplane.bitsWdh * 2;
    img->data = (BYTE *)malloc(img->bitplane.bitsWdh * img->bitplane.bitsHgh * 2);
    img->bitplane.bitsPtr = img->data;
    ReadFile(TmpFile, img->data, img->bitplane.bitsWdh * img->bitplane.bitsHgh * 2, &BytesCnt, NULL);
    CloseHandle(TmpFile);
    digifxConvertBitplane(DFXHnd, &img->bitplane);
    FlipImage(img->bitplane.bitsPtr, img->bitplane.bitsWdh, img->bitplane.bitsHgh);
}

void killImage(IMAGE *img)
{
    free(img->data);
    ZeroMemory(&img->bitplane, sizeof(img->bitplane));
}

void loadSprite(SPRITE *spr, IMAGE *img, int cellX, int cellY, int cellW, int cellH)
{
    DWORD BytesCnt;
    BITPLANE rleplane;
    int offx, offy, offset;
    
    offx = cellX * 2;
    offy = cellY * img->bitplane.bitsPitch;
    offset = offy + offx;
    
    rleplane.bitsWdh = cellW;
    rleplane.bitsHgh = cellH;
    rleplane.bitsFmt = PixelFmt;
    rleplane.bitsPitch = img->bitplane.bitsPitch;
    rleplane.bitsPtr = img->bitplane.bitsPtr + offset;
    
    BytesCnt = digifxCreateRLE(DFXHnd, &rleplane, digifxConvertColor(DFXHnd, 255, 0, 255), 0, 0, 0);
    
    spr->data = (BYTE *)malloc(BytesCnt);
    
    BytesCnt = digifxCreateRLE(DFXHnd, &rleplane, digifxConvertColor(DFXHnd, 255, 0, 255), &spr->rlehdr, spr->data, &spr->rlehdr);
    
    spr->rlehdr.AdjX = spr->rlehdr.SrcX;
    spr->rlehdr.AdjY = spr->rlehdr.SrcY;
}

void killSprite(SPRITE *spr)
{
    free(spr->data);
    ZeroMemory(&spr->rlehdr, sizeof(spr->rlehdr));
}

/*
 * LoadIntroGFX:
 *      Loads the intro bmp's onto their surfaces
 */
void LoadIntroGFX(void)
{
    lpDDSXG = DDLoadBitmap(".\\Gfx\\xgames.hwg", 0, 0);
    lpDDSPP = DDLoadBitmap(".\\Gfx\\perpprod.hwg", 0, 0);
    lpDDSMR = DDLoadBitmap(".\\Gfx\\mystreal.hwg", 0 , 0);
    lpDDSHW = DDLoadBitmap(".\\Gfx\\hajiworld.hwg", 0, 0);
}

/*
 * KillIntroGFX:
 *      Releases the surfaces for the intro graphics
 */
void KillIntroGFX(void)
{
    lpDDSXG->Release();
    lpDDSXG = NULL;
    lpDDSPP->Release();
    lpDDSPP = NULL;
    lpDDSMR->Release();
    lpDDSMR = NULL;
    lpDDSHW->Release();
    lpDDSHW = NULL;
}

/*
 * LoadGameGFX:
 *      Loads the gfx needed for the game
 */
void LoadGameGFX(void)
{
    int i, j;
    IMAGE imgTmp;
    
    loadImage(&imgTmp, ".\\Gfx\\bar.hwg");
    loadSprite(&sprBar, &imgTmp, 0, 0, 100, 18);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\lives.hwg");
    loadSprite(&sprLives, &imgTmp, 0, 0, 48, 32);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\digits.hwg");
    for (i = 0; i < 10; i++) {
        loadSprite(&sprDigits[i], &imgTmp, i * 18, 0, 18, 32);
    }
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\ground.hwg");
    loadSprite(&sprGround, &imgTmp, 0, 0, 640, 83);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\sky.hwg");
    loadSprite(&sprSky, &imgTmp, 0, 0, 640, 240);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\middle.hwg");
    loadSprite(&sprMiddle, &imgTmp, 0, 0, 640, 75);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\mountains.hwg");
    loadSprite(&sprMountains, &imgTmp, 0, 0, 640, 92);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\curry.hwg");
    loadSprite(&sprCurry, &imgTmp, 0, 0, 32, 32);
    killImage(&imgTmp);
    
    // Create Spotlight SPRITE
    /*imgTmp.bitplane.bitsWdh = 50;
    imgTmp.bitplane.bitsHgh = 50;
    imgTmp.bitplane.bitsFmt = PixelFmt;
    imgTmp.bitplane.bitsPitch = imgTmp.bitplane.bitsWdh*2;
    imgTmp.data = (BYTE *)malloc(imgTmp.bitplane.bitsWdh*imgTmp.bitplane.bitsHgh*2);
    imgTmp.bitplane.bitsPtr = (BYTE *)imgTmp.data;
    digifxConvertBitplane(DFXHnd, &imgTmp.bitplane);
    RECT rcRect = { 0, 0, 50, 50 };
    BlitFX.FXType = BLITFX_NONE;
    BlitFX.Color.R = 255;
    BlitFX.Color.G = 0;
    BlitFX.Color.B = 255;
    digifxDrawRect(DFXHnd, &rcRect, 0, 0, &BlitFX, &imgTmp.bitplane);
    for(i=0;i<100;i++)
    {
        BlitFX.FXType = BLITFX_MONO;
        BlitFX.Color.R = (int)((float)i*2.55);
        BlitFX.Color.G = (int)((float)i*2.55);
        BlitFX.Color.B = (int)((float)i*1.56);
        CircleFill(&imgTmp.bitplane, 25, 25, 25-i/4);
    }
    loadSprite(&sprSpotlight, &imgTmp, 0, 0, 50, 50);
    killImage(&imgTmp);*/
    loadImage(&imgTmp, ".\\Gfx\\spotlight.hwg");
    loadSprite(&sprSpotlight, &imgTmp, 0, 0, 50, 50);
    killImage(&imgTmp);
    
    sprObjects = (SPRITE **)malloc(sizeof(SPRITE) * g_NumObjects);
    for (i = 0; i < g_NumObjects; i++) {
        sprObjects[i] = (SPRITE *)malloc(sizeof(SPRITE) * nfoObject[i].pics);
        loadImage(&imgTmp, nfoObject[i].picfn);
        for (j = 0; j < nfoObject[i].pics; j++) {
            loadSprite(&sprObjects[i][j], &imgTmp, j * nfoObject[i].width, 0, nfoObject[i].width, nfoObject[i].height);
        }
        killImage(&imgTmp);
    }
    
    sprEnemies = (SPRITE **)malloc(sizeof(SPRITE) * g_NumEnemies);
    for (i = 0; i < g_NumEnemies; i++) {
        sprEnemies[i] = (SPRITE *)malloc(sizeof(SPRITE) * nfoEnemy[i].pics);
        loadImage(&imgTmp, nfoEnemy[i].picfn);
        for (j = 0; j < nfoEnemy[i].pics; j++) {
            loadSprite(&sprEnemies[i][j], &imgTmp, j * nfoEnemy[i].width, 0, nfoEnemy[i].width, nfoEnemy[i].height);
        }
        killImage(&imgTmp);
    }
    
    sprPickups = (SPRITE **)malloc(sizeof(SPRITE) * g_NumPickups);
    for (i = 0; i < g_NumPickups; i++) {
        sprPickups[i] = (SPRITE *)malloc(sizeof(SPRITE) * nfoPickup[i].pics);
        loadImage(&imgTmp, nfoPickup[i].picfn);
        for (j = 0; j < nfoPickup[i].pics; j++) {
            loadSprite(&sprPickups[i][j], &imgTmp, j * nfoPickup[i].width, 0, nfoPickup[i].width, nfoPickup[i].height);
        }
        killImage(&imgTmp);
    }
    
    sprObjectiles = (SPRITE **)malloc(sizeof(SPRITE) * g_NumObjectiles);
    for (i = 0; i < g_NumObjectiles; i++) {
        sprObjectiles[i] = (SPRITE *)malloc(sizeof(SPRITE) * nfoObjectile[i].pics);
        loadImage(&imgTmp, nfoObjectile[i].picfn);
        for (j = 0; j < nfoObjectile[i].pics; j++) {
            loadSprite(&sprObjectiles[i][j], &imgTmp, j * nfoObjectile[i].width, 0, nfoObjectile[i].width, nfoObjectile[i].height);
        }
        killImage(&imgTmp);
    }
    
    sprPlayer = (SPRITE *)malloc(sizeof(SPRITE) * nfoPlayer[player_whois].pics * 2);
    loadImage(&imgTmp, nfoPlayer[player_whois].picfn);
    for (i = 0; i < nfoPlayer[player_whois].pics; i++) {
        loadSprite(&sprPlayer[i], &imgTmp, i * nfoPlayer[player_whois].width, 0, nfoPlayer[player_whois].width, nfoPlayer[player_whois].height);
    }
    for (i = 0; i < nfoPlayer[player_whois].pics; i++) {
        loadSprite(&sprPlayer[i + nfoPlayer[player_whois].pics], &imgTmp, i * nfoPlayer[player_whois].width, nfoPlayer[player_whois].height, nfoPlayer[player_whois].width, nfoPlayer[player_whois].height);
    }
    killImage(&imgTmp);
}

/*
 * KillGameGFX:
 *      Releases the surfaces associated with graphics for the game
 */
void KillGameGFX(void)
{
    int i, j;
    
    killSprite(&sprBar);
    killSprite(&sprLives);
    for (i = 0; i < 10; i++) {
        killSprite(&sprDigits[i]);
    }
    killSprite(&sprSky);
    killSprite(&sprGround);
    killSprite(&sprMiddle);
    killSprite(&sprMountains);
    killSprite(&sprCurry);
    killSprite(&sprSpotlight);
    for (i = 0; i < g_NumObjects; i++) {
        for (j = 0; j < nfoObject[i].pics; j++) {
            killSprite(&sprObjects[i][j]);
        }
        free(sprObjects[i]);
    }
    free(sprObjects);
    for (i = 0; i < g_NumEnemies; i++) {
        for (j = 0; j < nfoEnemy[i].pics; j++) {
            killSprite(&sprEnemies[i][j]);
        }
        free(sprEnemies[i]);
    }
    free(sprEnemies);
    for (i = 0; i < g_NumPickups; i++) {
        for (j = 0; j < nfoPickup[i].pics; j++) {
            killSprite(&sprPickups[i][j]);
        }
        free(sprPickups[i]);
    }
    free(sprPickups);
    for (i = 0; i < g_NumObjectiles; i++) {
        for (j = 0; j < nfoObjectile[i].pics; j++) {
            killSprite(&sprObjectiles[i][j]);
        }
        free(sprObjectiles[i]);
    }
    free(sprObjectiles);
    for (i = 0; i < (nfoPlayer[player_whois].pics * 2); i++) {
        killSprite(&sprPlayer[i]);
    }
    free(sprPlayer);
}

void LoadAppGFX(void)
{
    IMAGE imgTmp;
    
    loadImage(&imgTmp, ".\\Gfx\\skidsmoke.hwg");
    loadSprite(&sprSkidSmoke, &imgTmp, 0, 0, 16, 16);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\charsel.hwg");
    loadSprite(&sprCharsel, &imgTmp, 0, 0, 640, 480);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\charselhaji.hwg");
    loadSprite(&sprCharselHaji, &imgTmp, 0, 0, 185, 259);
    killImage(&imgTmp);
    
    loadImage(&imgTmp, ".\\Gfx\\charselsumi.hwg");
    loadSprite(&sprCharselSumi, &imgTmp, 0, 0, 198, 266);
    killImage(&imgTmp);
}

void KillAppGFX(void)
{
    killSprite(&sprSkidSmoke);
    killSprite(&sprCharsel);
    killSprite(&sprCharselHaji);
    killSprite(&sprCharselSumi);
}
