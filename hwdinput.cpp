/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwdinput.cpp): DirectInput functions and interfacing
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <dinput.h>
#include "hwdinput.h"

// directinput globals
LPDIRECTINPUT        lpDI      = NULL;    // dinput object
LPDIRECTINPUTDEVICE  lpDIKey   = NULL;    // dinput keyboard
LPDIRECTINPUTDEVICE  lpDIMouse = NULL;    // dinput mouse
LPDIRECTINPUTDEVICE lpDIJoystick = NULL; // dinput joystick
LPDIRECTINPUTDEVICE2 lpDIJoystick2 = NULL; // dinput2 joystick

char joyname[80];
GUID joystickGUID;

// these contain the target records for all di input packets
UCHAR keyboard_state[256]; // contains keyboard state table
DIMOUSESTATE mouse_state;  // contains state of mouse
DIJOYSTATE joystick_state;

int DIInit(HINSTANCE inst)
{
    if (DirectInputCreate(inst, DIRECTINPUT_VERSION, &lpDI, NULL) != DI_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

void DIShutdown(void)
{
    if (lpDI) {
        lpDI->Release();
        lpDI = NULL;
    }
}

int DIInitMouse(HWND hwnd)
{
    // create a mouse device
    if (lpDI->CreateDevice(GUID_SysMouse, &lpDIMouse, NULL) != DI_OK) {
        return(0);
    }
    
    // set cooperation level
    if (lpDIMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK) {
        return(0);
    }
    
    // set data format
    if (lpDIMouse->SetDataFormat(&c_dfDIMouse) != DI_OK) {
        return(0);
    }
    
    // acquire the mouse
    if (lpDIMouse->Acquire() != DI_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

int DIInitKeyboard(HWND hwnd)
{
    // create the keyboard device
    if (lpDI->CreateDevice(GUID_SysKeyboard, &lpDIKey, NULL) != DI_OK) {
        return(0);
    }
    
    // set cooperation level
    if (lpDIKey->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) {
        return(0);
    }
    
    // set data format
    if (lpDIKey->SetDataFormat(&c_dfDIKeyboard) != DI_OK) {
        return(0);
    }
    
    // acquire the keyboard
    if (lpDIKey->Acquire() != DI_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

BOOL CALLBACK InitJoystick(LPCDIDEVICEINSTANCE lpDDi, LPVOID guid_ptr)
{
    *(GUID *)guid_ptr = lpDDi->guidInstance;
    
    strcpy(joyname, (char *)lpDDi->tszProductName);
    
    return(DIENUM_STOP);
}

int DIInitJoystick(HWND hwnd)
{
    // Enum the joystick device
    if (lpDI->EnumDevices(DIDEVTYPE_JOYSTICK, InitJoystick, &joystickGUID, DIEDFL_ATTACHEDONLY) != DI_OK) {
        return(0);
    }
    
    if (lpDI->CreateDevice(joystickGUID, &lpDIJoystick, NULL) != DI_OK) {
        return(0);
    }
    
    if (lpDIJoystick->QueryInterface(IID_IDirectInputDevice2, (void **)&lpDIJoystick2) != DI_OK) {
        return(0);
    }
    
    lpDIJoystick->Release();
    
    if (lpDIJoystick2->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) {
        return(0);
    }
    
    if (lpDIJoystick2->SetDataFormat(&c_dfDIJoystick) != DI_OK) {
        return(0);
    }
    
    DIPROPRANGE diprg;
    
    diprg.diph.dwSize       = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwObj        = DIJOFS_X;
    diprg.diph.dwHow        = DIPH_BYOFFSET;
    diprg.lMin              = -10;
    diprg.lMax              = +10;
    lpDIJoystick2->SetProperty(DIPROP_RANGE, &diprg.diph);
    
    diprg.diph.dwSize       = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwObj        = DIJOFS_Y;
    diprg.diph.dwHow        = DIPH_BYOFFSET;
    diprg.lMin              = -10;
    diprg.lMax              = +10;
    lpDIJoystick2->SetProperty(DIPROP_RANGE, &diprg.diph);
    
    if (lpDIJoystick2->Acquire() != DI_OK) {
        return(0);
    }
    
    return(1);
}

void DIReadMouse(void)
{
    if (lpDIMouse) {
        if (lpDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state) != DI_OK) {
            lpDIMouse->Acquire();
        }
    } else {
        memset(&mouse_state, 0, sizeof(mouse_state));
    }
}

void DIReadKeyboard(void)
{
    if (lpDIKey) {
        if (lpDIKey->GetDeviceState(256, (LPVOID)keyboard_state) != DI_OK) {
            lpDIKey->Acquire();
        }
    } else {
        memset(keyboard_state, 0, sizeof(keyboard_state));
    }
}

void DIReadJoystick(void)
{
    if (lpDIJoystick2) {
        lpDIJoystick2->Poll();
        if (lpDIJoystick2->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joystick_state) != DI_OK) {
            lpDIJoystick2->Acquire();
        }
    } else {
        memset(&joystick_state, 0, sizeof(joystick_state));
    }
}

void DIReleaseMouse(void)
{
    if (lpDIMouse) {
        lpDIMouse->Unacquire();
        lpDIMouse->Release();
        lpDIMouse = NULL;
    }
}

void DIReleaseKeyboard(void)
{
    if (lpDIKey) {
        lpDIKey->Unacquire();
        lpDIKey->Release();
        lpDIKey = NULL;
    }
}

void DIReleaseJoystick(void)
{
    if (lpDIJoystick2) {
        lpDIJoystick2->Unacquire();
        lpDIJoystick2->Release();
        lpDIJoystick2 = NULL;
    }
}