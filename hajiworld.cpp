/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hajiworld.cpp): Main functions, window initialization
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
#include <time.h>
#include "defines.h"
#include "hajiworld.h"
#include "hwworld.h"
#include "hwddraw.h"
#include "hwdsound.h"
#include "hwdinput.h"
#include "hwdigifx.h"
#include "hwengine.h"
#include "hwplayer.h"
#include "hwgfx.h"
#include "hwmidi.h"
#include "hwmemcpy.h"
#include "hwutil.h"
#include "resource.h"

GAMESTATE Game_State = INIT;

BOOL            g_Active;           // is the application active
HWND            g_hWnd;             // Global Main Window Handle
HINSTANCE   g_hInstance;        // Global Window Instance Handle
RECT            g_rcWindow;         // Main Window Rectangle

/*
 * WinProc:
 *    Processes windows messages.
 */
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // determine which message was sent in order to handle it
    switch (message) {
            // the app is being activated
        case WM_ACTIVATEAPP:
            g_Active = wParam; // is it active, TRUE or FALSE?
            break;
            // we've returned, kill the windows cursor
        case WM_SETCURSOR:
            SetCursor(NULL); // kill the windows cursor
            break;
            // it's telling us it's time to go, quit.....
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            // wow, time to restart the midi, if the looping variable is true.....
        case MM_MCINOTIFY:
            if ((wParam == MCI_NOTIFY_SUCCESSFUL) && IsMidiLooping()) {
                MidiRestart();    // restart the midi currently playing
            }
            break;
            // update the client area rectangle
        case WM_MOVE:
            GetClientRect(hWnd, &g_rcWindow); // Get The Current Client Rectangle
            ClientToScreen(hWnd, (LPPOINT)&g_rcWindow); // Get Top-Left
            ClientToScreen(hWnd, (LPPOINT)&g_rcWindow + 1); // Get Bottom-Right
            break;
    }
    
    // return to caller
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
 * FiniApp:
 *    Cleans up the application, frees memory.
 */
void FiniApp(void)
{
    WriteError("\n	====== KILL ======");
    
    // Kill the game stuff
    KillStuff();
    
    // Blacken the surfaces of the flipping chain
    // prevents residue gfx showing up after closing
    WriteError("\n	Clearing Display Surfaces...");
    DDFillSurface(lpDDSPrimary, 0); // Fill the primary surface with black
    DDFillSurface(lpDDSBack, 0);    // Fill the back surface with black
    
    // shutdown directdraw
    WriteError("\n	Shutting down DirectDraw...");
    DDShutdown();
    WriteError("\n	DirectDraw shut down...");
    
    // shutdown DigitalFX
    WriteError("\n	Shutting down DigitalFX...");
    digifxDone();
    WriteError("\n	DigitalFX shut down...");
    
    // shutdown directsound
    WriteError("\n	Shutting down DirectSound...");
    DSShutdown();
    WriteError("\n	DirectSound shut down...");
    
    // release all input devices
    WriteError("\n	Releasing Control of Input Devices...");
    DIReleaseKeyboard();
    DIReleaseJoystick();
    WriteError("\n	Input Devices released...");
    
    // shutdown directinput
    WriteError("\n	Shutting down DirectInput...");
    DIShutdown();
    WriteError("\n	DirectInput shut down...");
    
    WriteError("\n	====== END KILL ======");
    
    // close the error file
    CloseErrorFile();
}

/*
 * InitGame:
 *    Initializes core game components
 */
void InitGame()
{
    // open the error file, useful for debugging
    OpenErrorFile("error.dat");
    
    WriteError("\n	====== INITIALIZATION ======");
    
    // Generate trig lookup tables
    WriteError("\n	Generating trig lookup tables...");
    GenerateTabs();
    WriteError("\n	Trig lookups generated...");
    
    // Initialize directdraw
#ifdef WINDOWED
    WriteError("\n	Initializing DirectDraw Windowed mode...");
    DDInitWindowed(640, 480, 16, g_hWnd, RestoreGraphics);
    WriteError("\n	DirectDraw Windowed mode initialized...");
#else
    WriteError("\n	Initializing DirectDraw FullScreen mode...");
    DDInitFullscreen(640, 480, 16, g_hWnd, RestoreGraphics);
    WriteError("\n	DirectDraw FullScreen mode initialized...");
#endif
    
    // initialize Digital FX
    WriteError("\n	Initializing DigitalFX...");
    DFXInit(lpDDSPrimary);
    WriteError("\n	DigitalFX Initialized...");
    
    // initialize directsound
    WriteError("\n	Initializing DirectSound...");
    DSInit(g_hWnd);
    WriteError("\n	DirectSound Initialized...");
    
    // initialize Directinput
    WriteError("\n	Initializing DirectInput...");
    DIInit(g_hInstance);
    WriteError("\n	DirectInput Initialized...");
    
    // initialize all input devices
    WriteError("\n	Initializing Keyboard Input Device...");
    DIInitKeyboard(g_hWnd);
    WriteError("\n	Keyboard initialized...");
    WriteError("\n	Initializing Joystick Input Device...");
    DIInitJoystick(g_hWnd);
    WriteError("\n	Joystick initialized...");
    
    // Pick which memcpy we should use
    WriteError("\n	Determining fastest Memcpy() method...");
    Pickmemcpy();
    
    // Initialize the pixel shade table
    WriteError("\n	Initializing Pixel Shade lookup...");
    InitPixelShade();
    WriteError("\n	Pixel Shade lookup initialized...");
    
    // Initialize game stuff
    InitStuff();
    
    WriteError("\n	====== END INITIALIZATION ======");
    
    // Hide the mouse cursor
    ShowCursor(FALSE);
}

/*
 * InitApp:
 *    Initializes the application window.
 */
BOOL InitApp(HINSTANCE hInst, int nCmdShow)
{
    WNDCLASSEX WndClassEx;
    HWND hWnd;
    char *name = "Haji World";
    
    WndClassEx.cbSize = sizeof(WNDCLASSEX);
    WndClassEx.style = CS_HREDRAW | CS_VREDRAW;
    WndClassEx.lpfnWndProc = WinProc;
    WndClassEx.cbClsExtra = 0;
    WndClassEx.cbWndExtra = 0;
    WndClassEx.hInstance = hInst;
    WndClassEx.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(HW_ICON));
    WndClassEx.hIconSm = (HICON__ *)LoadImage(hInst, MAKEINTRESOURCE(HW_ICON), IMAGE_ICON, 16, 16, 0);
    WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClassEx.hbrBackground = (HBRUSH__ *)GetStockObject(BLACK_BRUSH);
    WndClassEx.lpszMenuName = 0;
    WndClassEx.lpszClassName = name;
    RegisterClassEx(&WndClassEx);
    
#ifndef WINDOWED
    // create the window, it's fullscreen mode
    hWnd = CreateWindowEx(
               WS_EX_TOPMOST,
               name,
               name,
               WS_POPUP,
               0, 0,
               64,
               48,
               NULL,
               NULL,
               hInst,
               NULL);
#else
    // Get Screen Metrics
    RECT rcScreen;
    SystemParametersInfo(SPI_GETWORKAREA, NULL, (RECT *)&rcScreen, NULL);
    // Determine how big we should make our window in order to have a 640x480 Client area
    RECT rcNewWindow = { 0, 0, 640, 480 };
    AdjustWindowRect((RECT *)&rcNewWindow, WS_VISIBLE | WS_OVERLAPPEDWINDOW, FALSE);
    // create the window using our data above, it's windowed mode
    hWnd = CreateWindow(
               name,
               name,
               WS_VISIBLE | WS_OVERLAPPEDWINDOW,
               (rcScreen.right / 2) - 320,
               (rcScreen.bottom / 2) - 240,
               rcNewWindow.right - rcNewWindow.left,
               rcNewWindow.bottom - rcNewWindow.top,
               NULL,
               NULL,
               hInst,
               NULL);
#endif
               
    if (!hWnd) {
        return FALSE;
    }
    
    // initialize global window variables
    g_hWnd      = hWnd;
    g_hInstance  = hInst;
    
    // Show the Window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    // Initialize the game
    InitGame();
    
    return TRUE;
}

/*
 * WinMain:
 *    Contains the message loop.
 */
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    
    srand(time(NULL));
    
    // initialize the app
    if (!InitApp(hInst, nCmdShow)) {
        return FALSE;
    }
    
    strcpy(level_file, lpCmdLine);
    
    // infinite loop
    while (1) {
        // peekmessage so as not to interrupt processes
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // if the message said to quit, get the hell outta here
            if (msg.message == WM_QUIT) {
                break;
            }
            
            // translate and dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // if there are no messages, update the game
        if (g_Active) {
            GameMain();
        }
    }
    
    // shutdown game and release all resources
    FiniApp();
    
    // return to Windows like this
    return(msg.wParam);
}

