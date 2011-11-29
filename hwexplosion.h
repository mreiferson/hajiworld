/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwexplosion.h): Particle explosion header
 *
 */

#ifndef HWEXPLOSIONH
#define HWEXPLOSIONH

#define EXPLOSION_LIFE          20
#define EXPLOSION_RADIUS        25
#define NUM_PARTICLES           400
#define NUM_PARTICLES_LONG      50
#define NUM_PARTICLES_SHORT 350
#define PARTICLEL_COLOR_R       220
#define PARTICLEL_COLOR_G       100
#define PARTICLEL_COLOR_B       20
#define PARTICLE_COLOR_R        220
#define PARTICLE_COLOR_G        200
#define PARTICLE_COLOR_B        0
#define PARTICLE_DTRANS         5
#define PARTICLE_DECEL_X        0.45
#define PARTICLE_DECEL_Y        0.45

typedef struct _PARTICLE {
    double x;               // x position
    double y;               // y position
    double vx;              // x velocity
    double vy;              // y velocity
    int r, g, b;
    int trans;
    int counter;        // use for counting
    int threshold;      // the counters threshold
} PARTICLE;

typedef struct _EXPLOSION {
    int x;
    int y;
    PARTICLE particles[NUM_PARTICLES];
    int lifetime;
    _EXPLOSION *next;
    _EXPLOSION *prev;
} EXPLOSION;

void AddExplosion(int x, int y, int r1, int g1, int b1, int r2, int g2, int b2);
void RemoveExplosion(EXPLOSION *explosion);
void AnimateExplosions(void);

extern EXPLOSION *explosions;

#endif