#pragma once

#include "config.h"

typedef struct {
    int tiles[WAITINGMAP_ROWS][WAITINGMAP_COLS];
    int width;
    int height;
} WAITINGMAP;

typedef struct {
    int tiles[FIRST_HALLWAYMAP_ROWS][FIRST_HALLWAYMAP_COLS];
}FIRSTHALLWAYMAP;

extern WAITINGMAP currentMap;
extern FIRSTHALLWAYMAP firsthallwayMap;