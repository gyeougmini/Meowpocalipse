#include "config.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "render.h"

void InitGame() {
	InitWaitingRoom();
	InitPlayer();
}

void Update() {
	UpdatePlayer();
	//UpdateEnemy();
	//UpdateBullet();
}

void Render(HDC mDC) {
	RanderWaitingMap(mDC);
	RenderPlayer(mDC);
}