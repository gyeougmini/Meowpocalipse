#include "render.h"
#include "map.h"
#include "player.h"

HBRUSH hBrush, oldBrush;
HPEN hPen, oldPen;

void DrawTile(HDC mDC, int screenX, int screenY, COLORREF color) {
	hBrush = CreateSolidBrush(color);
	oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = (HPEN)SelectObject(mDC, hPen);

	Rectangle(mDC, screenX, screenY, screenX + TILE_SIZE, screenY + TILE_SIZE);

	SelectObject(mDC, oldBrush);
	DeleteObject(hBrush);
	SelectObject(mDC, oldPen);
	DeleteObject(hPen);
}

void RenderWaitingMap(HDC mDC) {
	for (int row = 0; row < WAITINGMAP_ROWS; row++) {
		for (int col = 0; col < WAITINGMAP_COLS; col++) {

			int screenX = col * TILE_SIZE;
			int screenY = row * TILE_SIZE;

			COLORREF color;
			if (currentMap.tiles[row][col] == TILE_FLOOR) color = RGB(153, 76, 0);
			else if (currentMap.tiles[row][col] == TILE_WALL) color = RGB(0, 0, 0);
			else if (currentMap.tiles[row][col] == TILE_DOOR) color = RGB(0, 0, 255);

			DrawTile(mDC, screenX, screenY, color);
		}
	}
}

void RenderFirstHallWayMap(HDC mDC) {
	for (int row = 0; row < FIRST_HALLWAYMAP_ROWS; row++) {
		for (int col = 0; col < FIRST_HALLWAYMAP_COLS; col++) {
			int screenX = FIRST_HALLWAYMAP_X + col * TILE_SIZE;
			int screenY = row * TILE_SIZE;

			COLORREF color;
			if (firsthallwayMap.tiles[row][col] == TILE_WALL) color = RGB(0, 0, 0);
			else if (firsthallwayMap.tiles[row][col] == TILE_FLOOR) color = RGB(153, 76, 0);

			DrawTile(mDC, screenX, screenY, color);
		}
	}
}

void RenderPlayer(HDC mDC) {
	Rectangle(mDC, (int)player.base.x, (int)player.base.y, (int)player.base.x + player.base.width, (int)player.base.y + player.base.height);
}