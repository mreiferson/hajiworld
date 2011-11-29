/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwgfx.h): Graphical functions includes
 *
 */

#ifndef HWGFXH
#define HWGFXH

/*
 * Typedefs
 */
// RGB (red, green, blue) triplet
typedef struct _RGB {
    int r; // red component
    int g; // green component
    int b; // blue component
} RGB_;

typedef struct _IMAGE {
    BITPLANE bitplane;
    BYTE *data;
    BYTE header[18];
} IMAGE;

typedef struct _SPRITE {
    RLEHDR rlehdr;
    BYTE *data;
} SPRITE;

// Layer structure for parallax scrolling
typedef struct _LAYER {
    int x, y;
    int width, height;
    SPRITE spr;
} LAYER;

/*
 * Variables
 */
// Intro Surfaces
extern LPDIRECTDRAWSURFACE      lpDDSXG;
extern LPDIRECTDRAWSURFACE      lpDDSPP;
extern LPDIRECTDRAWSURFACE      lpDDSMR;
extern LPDIRECTDRAWSURFACE      lpDDSHW;
extern SPRITE                       sprCharsel;
extern SPRITE                       sprCharselHaji;
extern SPRITE                       sprCharselSumi;
extern SPRITE                       sprBar;
extern SPRITE                       sprLives;
extern SPRITE                       sprDigits[10];
extern SPRITE                       sprSky;
extern SPRITE                       sprGround;
extern SPRITE                       sprMiddle;
extern SPRITE                       sprMountains;
extern SPRITE                       sprSkidSmoke;
extern SPRITE                       sprCurry;
extern SPRITE                       sprSpotlight;
extern SPRITE                       *sprPlayer;
extern SPRITE                       **sprObjects;
extern SPRITE                       **sprEnemies;
extern SPRITE                       **sprPickups;
extern SPRITE                       **sprObjectiles;

/*
 * Function Declarations
 */
void RestoreGraphics(void);
void InitPixelShade(void);
void AlphaTransition(LPDIRECTDRAWSURFACE Src, LPDIRECTDRAWSURFACE Des);
void FadeToBlack(void);
void FadeToSurface(LPDIRECTDRAWSURFACE lpDDS);
void BlitString(char *txtstring, int x, int y, RGB_ *rgb);
void CircleFill(BITPLANE *bitplane, int x, int y, int radius);
void LayerDelete(LAYER *the_layer);
void LayerBuild(LAYER *dest_layer, IMAGE *img, int width, int height, int source_y, int draw_y);
void LayerDraw(LAYER *source_layer);
void LoadIntroGFX(void);
void KillIntroGFX(void);
void LoadGameGFX(void);
void KillGameGFX(void);
void LoadAppGFX(void);
void KillAppGFX(void);
void loadImage(IMAGE *img, char *filename);
void killImage(IMAGE *img);
void drawSprite(SPRITE *spr, int x, int y);
void loadSprite(SPRITE *spr, IMAGE *img, int cellX, int cellY, int cellW, int cellH);
void killSprite(SPRITE *spr);

#endif