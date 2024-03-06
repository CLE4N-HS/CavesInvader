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
	PLAYER_CHARGED_BULLET,
	PLAYER_LASER,
	PLAYER_MINES,
	PLAYER_FLAMETHROWER,

	ENEMY_YELLOW_BULLET,
	ENEMY_GREEN_BULLET,
	ENEMY_RED_BULLET
}bulletType;

typedef struct basicBulletParameters {
	float deathTimer;
}basicBulletParameters;

typedef struct laserParameters {
	float timer;
}laserParameters;

typedef struct minesParameters {
	float radius;
	sfBool allowedToDealDamages; // need another trick for the boss
}minesParameters;

typedef struct flamethrowerParameters {
	sfFloatRect bounds;
}flamethrowerParameters;


typedef struct Bullets {
	bulletType type;
	bulletId id;
	int ownerId;
	sfIntRect rect;
	sfFloatRect bounds;
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f velocity;
	sfVector2f scale;
	int damage;
	float fDamagePerSecond;
	float angle;
	float rotationSpeed;
	float animTimer;

	sfBool canDealDamages;
	union bulletsParameters
	{
		basicBulletParameters basicBullet;
		laserParameters laser;
		minesParameters mine;
		flamethrowerParameters flamethrower;
	};

}Bullets;


void initBullets(Window* _window);

void createBullets(bulletType _type, int _ownerId, sfVector2f _pos);

void updateBullets(Window* _window);

void displayBullets(Window* _window);

void deinitBullets();

