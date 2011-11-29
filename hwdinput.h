/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwdinput.h): DirectInput includes
 *
 */

#ifndef HWDINPUTH
#define HWDINPUTH

// DirectInput Function Declarations
int DIInit(HINSTANCE inst);
void    DIShutdown(void);
int DIInitMouse(HWND hwnd);
int DIInitKeyboard(HWND hwnd);
int DIInitJoystick(HWND hwnd);
void    DIReadMouse(void);
void    DIReadKeyboard(void);
void    DIReadJoystick(void);
void    DIReleaseMouse(void);
void    DIReleaseKeyboard(void);
void    DIReleaseJoystick(void);

// DirectInput Globals
extern LPDIRECTINPUT            lpDI;       // dinput object
extern LPDIRECTINPUTDEVICE  lpDIKey;    // dinput keyboard
extern LPDIRECTINPUTDEVICE  lpDIMouse;  // dinput mouse
extern LPDIRECTINPUTDEVICE  lpDIJoystick; // dinput joystick
extern LPDIRECTINPUTDEVICE2 lpDIJoystick2; // dinput2 joystick

// these contain the target records for all di input packets
extern UCHAR keyboard_state[256]; // contains keyboard state table
extern DIMOUSESTATE mouse_state;  // contains state of mouse
extern DIJOYSTATE joystick_state;

#endif