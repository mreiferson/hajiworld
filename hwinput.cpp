/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwinput.cpp): Input functions for mouse, keyboard, joystick
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <dinput.h>
#include "hwdinput.h"
#include "hwinput.h"

int MouseX, MouseY;     // Mouse X,Y Coordinates
BYTE MouseLB, MouseRB;  // Variables Which Tell If The Left/Right Mouse Buttons Are Pressed
BOOL butLeft, butRight, butDuck, butUp, butDown, butJump, butRun, butThrow; // Variables containing left/right button states

/*
 * UpdateInput:
 *    This updates the various input related variables of the game
 */
void UpdateInput(void)
{
    // Read the data from the mouse and keyboard
    DIReadMouse();
    DIReadKeyboard();
    DIReadJoystick();
    
    // Update the mouse position since it's relative the info we are given
    // is just the delta from the last time we called DI_Read_Mouse, so
    // we need to add this value to the X,Y mouse coordinates
    MouseX += (mouse_state.lX << 1); // scale the delta by multiplying by 2 (<< 1)
    MouseY += (mouse_state.lY << 1); // scale the delta by multiplying by 2 (<< 1)
    // Update the state of the mouse's buttons
    MouseLB = mouse_state.rgbButtons[0];
    MouseRB = mouse_state.rgbButtons[1];
    
    // crunch mouse coordinates to the extents of the screen
    if (MouseX < 0) {
        MouseX = 0;
    } else if (MouseX > 639) {
        MouseX = 639;
    }
    if (MouseY < 0) {
        MouseY = 0;
    } else if (MouseY > 479) {
        MouseY = 479;
    }
    
    if (keyboard_state[DIK_LEFT] || (joystick_state.lX < -2)) {
        butLeft = TRUE;
    } else {
        butLeft = FALSE;
    }
    
    if (keyboard_state[DIK_RIGHT] || (joystick_state.lX > 2)) {
        butRight = TRUE;
    } else {
        butRight = FALSE;
    }
    
    if (keyboard_state[DIK_DOWN] || (joystick_state.lY > 2)) {
        butDuck = TRUE;
    } else {
        butDuck = FALSE;
    }
    
    if (keyboard_state[DIK_SPACE] || joystick_state.rgbButtons[0]) {
        butJump = TRUE;
    } else {
        butJump = FALSE;
    }
    
    if (keyboard_state[DIK_LSHIFT] || joystick_state.rgbButtons[2]) {
        butRun = TRUE;
    } else {
        butRun = FALSE;
    }
    
    if (keyboard_state[DIK_LCONTROL] || joystick_state.rgbButtons[1]) {
        butThrow = TRUE;
    } else {
        butThrow = FALSE;
    }
}
