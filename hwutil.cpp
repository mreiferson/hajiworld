/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwutil.cpp): Utility functions
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "hwutil.h"

FILE    *g_fError = NULL;  // general error file
DWORD   g_StartClockCount = 0; // used for timing

double g_SinTab[360], g_CosTab[360];

DWORD GetClock(void)
{
    // return time
    return(GetTickCount());
}

DWORD StartClock(void)
{
    // init the count
    return(g_StartClockCount = GetClock());
}

DWORD WaitClock(DWORD count)
{
    // wait specified count
    while ((GetClock() - g_StartClockCount) < count);
    return(GetClock());
}

int OpenErrorFile(char *filename)
{
    FILE *fp_temp; // temporary file
    
    // test if this file is valid
    if ((fp_temp = fopen(filename, "w")) == NULL) {
        return(0);
    }
    
    // close old file if there was one
    if (g_fError) {
        CloseErrorFile();
    }
    
    // assign new file
    g_fError = fp_temp;
    
    // write out header
    WriteError("Opening Error Output File (%s):\n", filename);
    
    // return success
    return(1);
}

int CloseErrorFile(void)
{
    if (g_fError) {
        // write close file string
        WriteError("\n\nClosing Error Output File.");
        
        // close the file handle
        fclose(g_fError);
        g_fError = NULL;
        
        // return success
        return(1);
    } else {
        return(0);
    }
}

int WriteError(char *string, ...)
{
    char buffer[80]; // working buffer
    
    va_list arglist; // variable argument list
    
    // make sure both the error file and string are valid
    if (!string || !g_fError) {
        return(0);
    }
    
    // print out the string using the variable number of arguments on stack
    va_start(arglist, string);
    vsprintf(buffer, string, arglist);
    va_end(arglist);
    
    // write string to file
    fprintf(g_fError, buffer);
    
    // return success
    return(1);
}

void GenerateTabs(void)
{
    int i;
    
    for (i = 0; i < 360; i++) {
        g_SinTab[i] = sin(i * 3.14159 / 180);
        g_CosTab[i] = cos(i * 3.14159 / 180);
    }
}
