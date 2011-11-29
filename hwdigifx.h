/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwdigifx.h): DigitalFX Library Functions Header File
 *
 */

#ifndef HWDIGIFXH
#define HWDIGIFXH

extern DFXHND       DFXHnd;     // DigitalFX Global Handle
extern DWORD        PixelFmt;   // Pixel Format
extern BITPLANE Bitplane;   // Global bitplane (points to backbuf)
extern BLITFX       BlitFX;     // Global BlitFX DigitalFX Blitting Structure

void DFXInit(LPDIRECTDRAWSURFACE lpDDS);
void DFXSetupBitplane(void);
void DFXKillBitplane(void);

#endif