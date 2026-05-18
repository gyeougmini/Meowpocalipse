#include <stdlib.h>
#include <math.h>

#include "enemy.h"
#include "config.h"
#include "map.h"
#include "collision.h"
#include "player.h"
#include "object.h"
#include "boss.h"
#include "enum.h"

BOSS boss;
BOSS_PAW bossPaws[BOSS_PAW_LIMIT];
DASH_WARNING dashWarn;

void InitBoss() {
		boss.isActive = INACTIVE;
		boss.base.width = BOSS_SIZE;
		boss.base.height = BOSS_SIZE;
		boss.base.state = BOSS_IDLE;
		boss.base.dx = 0;
		boss.base.dy = 0;
		boss.base.hp = BOSS_HP;
		boss.attackTimer = BOSS_ATTACK_INTERVAL;
		boss.pawTimer = 0;
		boss.dashTimer = 0;
		boss.isDashing = 0;
		boss.dashDirX = 0;
		boss.dashDirY = 0;

		for (int i = 0; i < BOSS_PAW_LIMIT; i++) {
			bossPaws[i].isActive = INACTIVE;
		}

		dashWarn.isActive = INACTIVE;
		dashWarn.timer = 0;
}

void SpawnBoss(MAP_TYPE type) {
	if (type == MAP_WAITING ||
		type == MAP_FIRST_HALLWAY ||
		type == MAP_SECOND_HALLWAY ||
		type == MAP_THIRD_HALLWAY) return;

	MAPDATA* m = &maps[type];

	float spawnX = m->worldX + (BOSSMAP_COLS / 2) * TILE_SIZE;
	float spawnY = m->worldY + (BOSSMAP_ROWS / 2) * TILE_SIZE;

	if (type == MAP_FIRST_BOSS && boss.base.hp == BOSS_HP) {

		if (boss.isActive == INACTIVE) {
			boss.isActive = ACTIVE;
			boss.base.x = spawnX;
			boss.base.y = spawnY;
			boss.base.state = BOSS_IDLE;
			boss.base.dx = 0;
			boss.base.dy = 0;
		}
	}
	else if (type == MAP_SECOND_BOSS && boss.base.hp <= BOSS_HP / 2) {

		if (boss.isActive == INACTIVE) {
			boss.isActive = ACTIVE;
			boss.base.x = spawnX;
			boss.base.y = spawnY;
			boss.base.state = BOSS_IDLE;
			boss.base.dx = 0;
			boss.base.dy = 0;
		}
	}
	else if (type == MAP_THIRD_BOSS && boss.base.hp <= (BOSS_HP / 2) / 2) {

		if (boss.isActive == INACTIVE) {
			boss.isActive = ACTIVE;
			boss.base.x = spawnX;
			boss.base.y = spawnY;
			boss.base.state = BOSS_IDLE;
			boss.base.dx = 0;
			boss.base.dy = 0;
		}
	}
}

void SpawnBossPaws() {

	boss.isActive = BOSS_THREE_WAY_CATPAW;
	float dx = player.base.x - boss.base.x;
	float dy = player.base.y - boss.base.y;
	float len = sqrtf(dx * dx + dy * dy);
	
	if (len == 0) return;
	dx /= len;
	dy /= len;

	float offsets[3] = { -0.7854f, 0.0f, 0.7854f };

	for (int d = 0; d < 3; d++) {
		float angle = atan2f(dy, dx) + offsets[d];
		float vx = cosf(angle);
		float vy = sinf(angle);

		for (int i = 0; i < BOSS_PAW_LIMIT; i++) {
			if (bossPaws[i].isActive == INACTIVE) {
				bossPaws[i].isActive = ACTIVE;
				bossPaws[i].x = boss.base.x;
				bossPaws[i].y = boss.base.y;
				bossPaws[i].dx = vx * BOSS_PAW_SPEED;
				bossPaws[i].dy = vy * BOSS_PAW_SPEED;
				break;
			}
		}
	}
}

void StartDashWarning() {

	float dx = player.base.x - boss.base.x;
	float dy = player.base.y - boss.base.y;
	float len = sqrtf(dx * dx + dy * dy);

	if (len == 0) return;

	dashWarn.startX = boss.base.x;
	dashWarn.startY = boss.base.y;
	dashWarn.dirX = dx / len;
	dashWarn.dirY = dy / len;

	// 수직 방향 벡터 계산해서 저장 (render에서 재계산 불필요)
	dashWarn.perpX = -dashWarn.dirY;
	dashWarn.perpY = dashWarn.dirX;

	// 벽까지 거리 계산 - render.cpp 대신 여기서 한 번만 계산
	int half = BOSS_SIZE / 2;
	int maxDist = DASH_WARN_TILES * TILE_SIZE;
	dashWarn.stopDist = maxDist;

	for (int dist = 1; dist <= maxDist; dist++) {
		float cx = dashWarn.startX + dashWarn.dirX * dist;
		float cy = dashWarn.startY + dashWarn.dirY * dist;

		if (IsTileWall(cx + dashWarn.perpX * half, cy + dashWarn.perpY * half) ||
			IsTileWall(cx - dashWarn.perpX * half, cy - dashWarn.perpY * half) ||
			IsTileWall(cx, cy)) {
			dashWarn.stopDist = dist;
			break;
		}
	}

	dashWarn.timer = DASH_WARN_INTERVAL;
	dashWarn.isActive = ACTIVE;

	boss.dashDirX = dashWarn.dirX;
	boss.dashDirY = dashWarn.dirY;
	boss.base.state = BOSS_DASH;
}

void UpdateBoss() {
	if (currentMapType == MAP_WAITING ||
		currentMapType == MAP_FIRST_HALLWAY ||
		currentMapType == MAP_SECOND_HALLWAY ||
		currentMapType == MAP_THIRD_HALLWAY) return;

	// 대시 실행 중
	if (boss.isDashing) {

		float nextX = boss.base.x + boss.dashDirX * DASH_SPEED;
		float nextY = boss.base.y + boss.dashDirY * DASH_SPEED;

		// 보스 가장자리 4코너로 벽 충돌 체크
		int half = BOSS_SIZE / 2;

		if (IsTileWall(nextX - half, nextY - half) ||
			IsTileWall(nextX + half, nextY - half) ||
			IsTileWall(nextX - half, nextY + half) ||
			IsTileWall(nextX + half, nextY + half)) {
			boss.isDashing = INACTIVE;
			boss.base.state = BOSS_IDLE;
			boss.attackTimer = BOSS_ATTACK_INTERVAL;
		}

		else {
			boss.base.x = nextX;
			boss.base.y = nextY;
			boss.dashTimer--;
			
			if (boss.dashTimer <= 0) {
				boss.isDashing = INACTIVE;
				boss.base.state = BOSS_IDLE;
				boss.attackTimer = BOSS_ATTACK_INTERVAL;
			}
		}
	}

	// 경고 카운트다운 → 대시 실행
	else if (dashWarn.isActive == ACTIVE) {

		dashWarn.timer--;
		if (dashWarn.timer <= 0) {
			dashWarn.isActive = INACTIVE;
			boss.isDashing = ACTIVE;
			boss.dashTimer = DASH_INTERVAL;
		}
	}

	// 패턴 선택 타이머
	else {
		boss.attackTimer--;
		if (boss.attackTimer <= 0) {
			// 0: CAT_PAW 발사,  1: 대시 경고
			int pattern = rand() % 5;

			if (pattern <= 3) {
				SpawnBossPaws();
				boss.attackTimer = BOSS_ATTACK_INTERVAL;
			}
			else {
				StartDashWarning();
				// attackTimer는 대시 종료 후 리셋
			}
		}
	}

	// CAT_PAW 이동
	for (int i = 0; i < BOSS_PAW_LIMIT; i++) {
		if (!bossPaws[i].isActive) continue;

		bossPaws[i].x += bossPaws[i].dx;
		bossPaws[i].y += bossPaws[i].dy;

		float bosspawNextX = bossPaws[i].x + bossPaws[i].dx;
		float bosspawNextY = bossPaws[i].y + bossPaws[i].dy;

		if (IsTileWall(bossPaws[i].x, bossPaws[i].y) || IsTileWall(bosspawNextX, bossPaws[i].y) || IsTileWall(bossPaws[i].x, bosspawNextY) || IsTileWall(bosspawNextX, bosspawNextY)) {
			bossPaws[i].isActive = INACTIVE;
			continue;
		}
	}
}
