/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwutil.h): Utility includes and defines
 *
 */

#ifndef HWUTILH
#define HWUTILH

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// general utility functions
DWORD GetClock(void);
DWORD StartClock(void);
DWORD WaitClock(DWORD count);
void GenerateTabs(void);

// error functions
int OpenErrorFile(char *filename);
int CloseErrorFile(void);
int WriteError(char *string, ...);

extern FILE     *g_fError;            // general error file
extern DWORD    g_StartClockCount;    // used for timing
extern double   g_SinTab[360], g_CosTab[360];

#endif