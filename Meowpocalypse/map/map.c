#include "map.h"
#include "config.h"

WAITINGMAP currentMap;

void InitWaitingRoom() {

	for (int row = 0; row < WAITINGMAP_ROWS; row++) {
		for (int col = 0; col < WAITINGMAP_COLS; col++) {
			if (row == 0 || col == 0 || 
				row == 1 || col == 1 || 
				row > WAITINGMAP_ROWS - 3 || col > WAITINGMAP_COLS - 3) {
				// 벽
				currentMap.tiles[row][col] = TILE_WALL;
			}
			else {
				// 바닥
				currentMap.tiles[row][col] = TILE_FLOOR;
			}
		}
	}
	currentMap.tiles[40][65] = TILE_DOOR;
	currentMap.tiles[40][66] = TILE_DOOR;
}