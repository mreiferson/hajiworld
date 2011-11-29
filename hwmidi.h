/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwmidi.h): MIDI & MCI includes
 *
 */

#ifndef HWMIDIH
#define HWMIDIH

/*
 * Function Declarations
 */
BOOL MidiPlay(const char *Filename, BOOL loop);
BOOL MidiStop(void);
BOOL MidiPause(void);
BOOL MidiResume(void);
BOOL MidiRestart(void);
BOOL IsMidiLooping(void);

#endif