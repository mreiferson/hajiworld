/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwmemcpy.cpp): Memory speed transfer functions (FPU,MMX)
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include "hwmemcpy.h"
#include "hwutil.h"

memcpy_funcptr g_MemCpySys2Vid;
memcpy_funcptr g_MemCpyBlit;

/*
 * fpucopy:
 *      Use the floating point co-processor to copy memory, not necessarily aligned
 */
void fpucpy(void *des, void *src, long count)
{
    long notaligned;
    long qcount;
    long wcount;
    
    notaligned = count % 4;
    qcount = count >> 2;
    
    if (!notaligned) {
        _asm {
            mov ecx, [qcount]
            mov esi, [src]
            mov edi, [des]
            qwordcopy:
            fild qword ptr [esi]
            fistp qword ptr [edi]
            add esi, 8
            add edi, 8
            dec ecx
            jnz qwordcopy
        }
    } else if (qcount) {
        wcount = notaligned;
        _asm {
            mov ecx, [qcount]
            mov esi, [src]
            mov edi, [des]
            qwordcpy:
            fild qword ptr [esi]
            fistp qword ptr [edi]
            add esi, 8
            add edi, 8
            dec ecx
            jnz qwordcpy
            mov ecx, [wcount]
            wordcpy:
            fild word ptr [esi]
            fistp word ptr [edi]
            add esi, 2
            add edi, 2
            dec ecx
            jnz wordcpy
        }
    } else {
        _asm {
            mov esi, [src]
            mov edi, [des]
            mov ecx, [count]
            rep movsw
        }
    }
}

#define FPU_ALIGN 8
#define FPU_SHIFT 3

/*
 * fpucopya:
 *      Use the Floating Point Co-Processor to copy memory, aligned
 */
void fpucpya(void *des, void *src, long count)
{
    long prebytes;
    long postbytes;
    long qcount;
    
    postbytes = count;
    prebytes = (FPU_ALIGN - (((long)des) % FPU_ALIGN)) % FPU_ALIGN;
    if (prebytes > postbytes) {
        prebytes = postbytes;
    }
    postbytes = postbytes - prebytes;
    qcount = (postbytes >> FPU_SHIFT);
    postbytes = postbytes - (qcount << FPU_SHIFT);
    
    __asm {
        mov esi, [src]
        mov edi, [des]
        mov ecx, [prebytes]
        jcxz QWORDSTART
        rep movs    [edi], [esi]
        QWORDSTART:
        mov ecx, [qcount]
        jcxz POSTBYTES
        QDWORDLOOP:
        fild qword ptr [esi]
        fistp qword ptr [edi]
        add esi, FPU_ALIGN
        add edi, FPU_ALIGN
        dec ecx
        jnz QDWORDLOOP
        POSTBYTES:
        mov ecx, [postbytes]
        jcxz DONE
        rep movs [edi], [esi]
        DONE:
    }
}

/*
 * mmxcpy:
 *      uses, if available, the mmx instruction set for fast memory copy
 */
void mmxcpy(void *des, void *src, long count)
{
    long notaligned;
    long qcount;
    
    notaligned = count % 8;
    qcount = count >> 3;
    
    __asm {
        mov esi, [src]
        mov edi, [des]
        mov ecx, [qcount]
        TOP:
        movq mm0, [esi]
        movq [edi], mm0
        add esi, 8
        add edi, 8
        dec ecx
        jnz top
        emms
        mov ecx, [notaligned]
        jcxz DONE
        rep movs [edi], [esi]
        DONE:
    }
}

#define CPUID __asm _emit 0x0F __asm _emit 0xA2

/*
 * CheckMMXTechnology:
 *      Checks for the presence of a MMX compatible processor
 */
int CheckMMXTechnology(void)
{
    volatile int retval = 1;
    volatile long RegEDX;
    
    __try {
        _asm {
            mov eax, 1 // set up CPUID to return processor version and features 0 = vendor string, 1 = version info, 2 = cache info
            CPUID // code bytes = 0fh,  0a2h
            mov RegEDX, edx // features returned in edx
        }
    }
    
    __except (EXCEPTION_EXECUTE_HANDLER) {
        retval = 0;
    }
    
    if (retval == 0) {
        return 0;    // processor does not support CPUID
    }
    
    if (RegEDX & 0x800000) { // bit 23 is set for MMX technology
        __try {
            _asm emms // try executing the MMX instruction "emms"
        }
        
        __except (EXCEPTION_EXECUTE_HANDLER) {
            retval = 0;
        }
        
        return retval;
    } else {
        return 0;    // processor supports CPUID but does not support MMX technology
    }
    
    return retval;
}

/*
 * Pickmemcpy:
 *      Picks which memcpy replacement to use, an fpucopy or mmxcopy
 */
void Pickmemcpy(void)
{
    if (CheckMMXTechnology()) {
        WriteError("\n		g_MemCpySys2Vid = mmxcpy");
        g_MemCpySys2Vid = mmxcpy;
        WriteError("\n		g_MemCpyBlt = mmxcpy");
        g_MemCpyBlit = mmxcpy;
    } else {
        WriteError("\n		g_MemCpySys2Vid = fpucpya");
        g_MemCpySys2Vid = fpucpya;
        WriteError("\n		g_MemCpyBlt = fpucpy");
        g_MemCpyBlit = fpucpy;
    }
}