#pragma once
#include "tools.h"
#include "stateManager.h"
#include "customMath.h"

typedef enum bulletId {
	PLAYER_ID_BULLET,
	ENEMY_ID_BULLET
}bulletId;

typedef enum bulletType {
	PLAYER_BASIC_BULLET,
	PLAYER_CHARGED_BULLET
}bulletType;

typedef struct playerBullets {
	bulletType type;
	bulletId id;
	sfIntRect rect;
	sfVector2f pos;
	sfVector2f velocity;
}playerBullets;


void initBullets(Window* _window);

void createPlayerBullets(bulletType _type, bulletId _id, sfVector2f _pos);

void updateBullets(Window* _window);

void displayBullets(Window* _window);

void deinitBullets();

