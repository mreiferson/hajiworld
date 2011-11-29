/*
 * Haji World (C) Perplexed Productions
 *
 *  Module (hajiworld.h): main include file
 *
 */

#ifndef HAJIWORLDH
#define HAJIWORLDH

/*
 * Typedefs
 */
// The various things the game can be doing
typedef enum _GAMESTATE {
    XGINTRO, PPINTRO, MRINTRO, HWINTRO, CHARSEL, PLAYING, ENDLEVEL, NOTHING, INIT
} GAMESTATE;

/*
 * Variables
 */
extern BOOL         g_Active;           // is the application currently active
extern GAMESTATE    Game_State;     // state of the game
extern HWND         g_hWnd;         // save the window handle
extern HINSTANCE    g_hInstance;    // save the instance
extern RECT         g_rcWindow;     // main window rectangle

/*
 * Function Declarations
 */
void FiniApp(void);

#endif