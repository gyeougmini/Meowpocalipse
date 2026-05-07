#include "render.h"
#include "map.h"
#include "player.h"

HBRUSH hBrush, oldBrush;
HPEN hPen, oldPen;

void RanderWaitingMap(HDC mDC) {
	for (int row = 0; row < WAITINGMAP_ROWS; row++) {
		for (int col = 0; col < WAITINGMAP_COLS; col++) {
			COLORREF color;
			if (currentMap.tiles[row][col] == TILE_FLOOR) color = RGB(0, 0, 0);
			else if (currentMap.tiles[row][col] == TILE_WALL) color = RGB(100, 100, 100);
			else if (currentMap.tiles[row][col] == TILE_DOOR) color = RGB(0, 0, 255);

			hBrush = CreateSolidBrush(color);
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(mDC, hPen);

			Rectangle(mDC, col * TILE_SIZE, row * TILE_SIZE, col * TILE_SIZE + TILE_SIZE, row * TILE_SIZE + TILE_SIZE);

			SelectObject(mDC, oldBrush);
			DeleteObject(hBrush);
			SelectObject(mDC, oldPen);
			DeleteObject(hPen);
		}
	}
}

void RenderPlayer(HDC mDC) {
	Rectangle(mDC, (int)player.base.x, (int)player.base.y, (int)player.base.x + player.base.width, (int)player.base.y + player.base.height);
}