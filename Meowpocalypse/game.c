#include "config.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "render.h"

void InitGame() {
	InitWaitingRoom();
	InitFirstHallWay();
	InitPlayer();
}

void Update() {
	UpdatePlayer();
	//UpdateEnemy();
	//UpdateBullet();
}

void Render(HDC mDC) {
	DrawTile(mDC);
	RenderWaitingMap(mDC);
	RenderFirstHallWayMap(mDC);
	RenderPlayer(mDC);
}