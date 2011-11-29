/*
 * Haji World (C) Perplexed Productions
 *
 * Module (hwworld.h): World data, loading, initializing include file
 *
 */

#ifndef HWWORLDH
#define HWWORLDH

#define SECTOR_W                640
#define SECTOR_H                448

/*
 * Data Structures
 */
// a single vertex
typedef struct _VERT {
    int x;
    int y;
} VERT;

// a polygon for describing level areas
typedef struct _POLY {
    int id;
    int num_verts;
    VERT    *verts;
    BOOL    canwalkon;
    BOOL    canmovethrough;
    BOOL    canjumpdown;
    _POLY   *next;
} POLY;

// a sector of a level containing a linked list of polygons
typedef struct _SECTOR {
    POLY *polyLst;
} SECTOR;

// an object in the foreground/top layer
typedef struct _OBJECT {
    int x, y;
    int id;
    int mpnum;
    int count;
    int frame;
    _OBJECT *next;
} OBJECT;

// enumerations for moving polygons
typedef enum _MOVEPOLY_TYPE { MPT_HORIZONTAL, MPT_VERTICAL, MPT_FALLING } MOVEPOLY_TYPE;
typedef enum _MOVEPOLY_STATE { MPS_MOVINGUP, MPS_MOVINGDOWN, MPS_MOVINGLEFT, MPS_MOVINGRIGHT,
                               MPS_FALLING, MPS_ABOUTTOFALL, MPS_NOTFALLING, MPS_DONEFALLING,
                               MPS_ACTIVATED, MPS_UNACTIVATED
                             } MOVEPOLY_STATE;

// a moving polygon for describing interactive surfaces
typedef struct _MOVEPOLY {
    int             ognx, ogny;
    int             x, y;
    int             w, h;
    BOOL                canwalkon;
    BOOL                canmovethrough;
    OBJECT          *ascObject;
    MOVEPOLY_STATE  state;
    MOVEPOLY_TYPE   type;
    int             velx;
    int             vely;
    int             data1;
    int             data2;
    _MOVEPOLY       *next;
    _MOVEPOLY       *prev;
} MOVEPOLY;

typedef struct _GATE {
    VERT verts[4];
} GATE;

typedef struct _OBJECTNFO {
    char picfn[255];
    int pics;
    int width;
    int height;
    int anmdel;
    int trans;
} OBJECTNFO;

/*
 * Variables
 */
extern int          g_VOX,          // The x offset for the current viewport
       g_VOY;          // The y offset for the current viewport
extern int          g_VOX_origin, g_VOY_origin;
extern char         level_file[80];
extern OBJECTNFO    *nfoObject;
extern int          g_NumObjects;

/*
 * Functions
 */
void InitLevel(int levelnum);
void KillLevel(void);
void ReadObjectNFO(void);
void DeleteObjectNFO(void);
int GetSectorList(POLY **curPoly, int x, int y, int w, int h);
void BuildMovePolyList(void);
void ProcessMovePolyList(void);
void ProcessObjects(void);

#endif