#pragma once

#include "config.h"

typedef struct {
    int tiles[WAITINGMAP_ROWS][WAITINGMAP_COLS];
    int width;
    int height;
} WAITINGMAP;

extern WAITINGMAP currentMap;