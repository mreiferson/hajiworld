/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwmemcpy.h): Memory functions includes
 *
 */

#ifndef HWMEMCPYH
#define HWMEMCPYH

typedef void (*memcpy_funcptr)(void *des, void *src, long count);

extern memcpy_funcptr g_MemCpySys2Vid;
extern memcpy_funcptr g_MemCpyBlit;

void fpucpy(void *des, void *src, long count);
void fpucpya(void *des, void *src, long count);
void mmxcpy(void *des, void *src, long count);
void Pickmemcpy(void);

#endif