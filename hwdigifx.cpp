/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hwdigifx.cpp): DigitalFX Library Functions
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <malloc.h>
#include <ddraw.h>
#include <digifx.h>
#include <stdio.h>
#include "hwddraw.h"
#include "hwdigifx.h"
#include "hwutil.h"

DFXHND      DFXHnd;
DWORD           PixelFmt;
BITPLANE        Bitplane;
BLITFX      BlitFX;

BOOL ChkSupport()
{
    RLEHDR TmpRLEHdr;
    BITPLANE TmpBitplane;
    
    Bitplane.bitsFmt = PixelFmt;
    BlitFX.FXType = BLITFX_BLEND;
    BlitFX.BlendSrcFactor = 0;
    BlitFX.BlendDstFactor = 0;
    
    TmpRLEHdr.PixFmt = PixelFmt;
    TmpRLEHdr.DataPtr = NULL;
    TmpBitplane.bitsFmt = PixelFmt;
    TmpBitplane.bitsPtr = NULL;
    
    if (!digifxCheckSupport(DFXHnd, DFX_DRAWRLE, &BlitFX, &TmpRLEHdr, &Bitplane)) {
        return(FALSE);
    }
    if (!digifxCheckSupport(DFXHnd, DFX_DRAWBITPLANE, &BlitFX, &TmpBitplane, &Bitplane)) {
        return(FALSE);
    }
    BlitFX.FXType = BLITFX_TEXTURED;
    TmpRLEHdr.PixFmt = PixelFmt;
    TmpRLEHdr.DataPtr = NULL;
    if (!digifxCheckSupport(DFXHnd, DFX_DRAWRLE, &BlitFX, &TmpRLEHdr, &Bitplane)) {
        return(FALSE);
    }
    BlitFX.FXType = BLITFX_MONO;
    if (!digifxCheckSupport(DFXHnd, DFX_DRAWRECT, &BlitFX, &TmpRLEHdr, &Bitplane)) {
        return(FALSE);
    }
    if (!digifxCheckSupport(DFXHnd, DFX_DRAWLINE, &BlitFX, &TmpRLEHdr, &Bitplane)) {
        return(FALSE);
    }
    
    return(TRUE);
}

BOOL DFXEnumProc(CHAR *DrvInfoPtr)
{
    DFXHnd = digifxLoadDriver(DrvInfoPtr, PixelFmt);
    if (DFXHnd) {
        if (ChkSupport()) {
            return(FALSE);
        }
    }
    digifxFreeDriver(DFXHnd);
    return(TRUE);
}

void DFXInit(LPDIRECTDRAWSURFACE lpDDS)
{
    CHAR szCurrentDir[MAX_PATH];
    
    if (pRed == 10) {
        PixelFmt = PIXFMT_555;
        WriteError("\n		Pixel Format Is 5,5,5.");
    } else if (pRed == 11) {
        PixelFmt = PIXFMT_565;
        WriteError("\n		Pixel Format Is 5,6,5.");
    } else {
        WriteError("\n		Couldn't Calculate Pixel Format.");
    }
    if (mRed < mBlue) {
        PixelFmt |= PIXFMT_BGR;
        WriteError("\n		Pixel Format Is BGR.");
    }
    
    GetCurrentDirectory(MAX_PATH, szCurrentDir);
    if (szCurrentDir[lstrlen(szCurrentDir) - 1] != '\\') {
        lstrcat(szCurrentDir, "\\");
    }
    
    digifxInit((char *)&szCurrentDir);
    digifxEnumDrivers(DFXEnumProc);
}

void DFXSetupBitplane(void)
{
    DDSURFACEDESC ddsd;
    
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    lpDDSBack->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
    
    ZeroMemory(&Bitplane, sizeof(Bitplane));
    Bitplane.bitsFmt = PixelFmt;
    Bitplane.bitsPtr = (BYTE *)ddsd.lpSurface;
    Bitplane.bitsWdh = screen_width;
    Bitplane.bitsHgh = screen_height;
    Bitplane.bitsPitch = ddsd.lPitch;
}

void DFXKillBitplane(void)
{
    lpDDSBack->Unlock(Bitplane.bitsPtr);
    ZeroMemory(&Bitplane, sizeof(Bitplane));
}
