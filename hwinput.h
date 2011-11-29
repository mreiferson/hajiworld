/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwinput.h): Input functions (mouse, keybaord, joystick) includes
 *
 */

#ifndef HWINPUTH
#define HWINPUTH

/*
 * Variables
 */
extern int MouseX, MouseY;
extern BYTE MouseLB, MouseRB;
extern BOOL butLeft, butRight, butDuck, butUp, butDown, butJump, butRun, butThrow;

/*
 * Function Declarations
 */
void UpdateInput(void);

#endif