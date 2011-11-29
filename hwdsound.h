/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwdsound.h): DirectSound includes
 *
 */

#ifndef HWDSOUNDH
#define HWDSOUNDH

#define MAX_SOUNDS     64 // max number of sounds in system at once 

#define SOUND_NULL     0
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3

// voc file defines
#define NVB_SIZE       6  // size of new voice block in bytes

// MACROS /////////////////////////////////////////////////

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

// this holds a single sound
typedef struct _PCMSOUND {
    LPDIRECTSOUNDBUFFER dsbuffer;   // the ds buffer containing the sound
    int state;                      // state of the sound
    int rate;                       // playback rate
    int size;                       // size of sound
    int id;                         // id number of the sound
} PCMSOUND;

// sound
int LoadWAV(char *filename, int control_flags = DSBCAPS_CTRLDEFAULT);
int ReplicateSound(int source_id);
int PlaySound(int id, int flags);
int StopSound(int id);
int StopAllSounds(void);
int DSInit(HWND hwnd);
int DSShutdown(void);
int DeleteSound(int id);
int DeleteAllSounds(void);
int StatusSound(int id);
int SetSoundVolume(int id, int vol);
int SetSoundFreq(int id, int freq);
int SetSoundPan(int id, int pan);
int StatusSound(int id);

extern LPDIRECTSOUND            lpDS;                           // directsound interface pointer
extern LPDIRECTSOUNDBUFFER  lpDSBPrimary;               // the primary mixing buffer
extern DSBUFFERDESC         dsbd;                           // directsound description
extern DSCAPS                   dscaps;                     // directsound caps
extern HRESULT                  dsresult;                   // general directsound result
extern DSBCAPS                  dsbcaps;                        // directsound buffer caps
extern PCMSOUND             sound_fx[MAX_SOUNDS];   // the array of secondary sound buffers
extern WAVEFORMATEX         pcmwf;                      // generic waveformat structure

#endif