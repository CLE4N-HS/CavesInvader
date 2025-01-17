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
	ENEMY_RED_BULLET,
	ENEMY_WARNING_BULLET
}bulletType;

typedef struct basicBulletParameters {
	float deathTimer;
}basicBulletParameters;

typedef struct laserParameters {
	float timer;
	float timeLasering;
}laserParameters;

typedef struct minesParameters {
	float radius;
	sfBool allowedToDealDamages; // need another trick for the boss
	sfBool playSound;
}minesParameters;

typedef struct flamethrowerParameters {
	sfFloatRect bounds;
	sfBool playSound;
}flamethrowerParameters;

typedef struct enemyBulletParameters {
	float deathTimer;
}enemyBulletParameters;

typedef struct Bullets {
	bulletType type;
	bulletId id;
	int ownerId;
	sfIntRect rect;
	sfFloatRect bounds;
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f forward;
	float speed;
	sfVector2f velocity;
	sfVector2f scale;
	int damage;
	float fDamagePerSecond;
	float angle;
	float rotationSpeed;
	float animTimer;
	sfVector2f originToCenter;
	float radius;

	sfBool canDealDamages;
	union bulletsParameters
	{
		basicBulletParameters basicBullet;
		laserParameters laser;
		minesParameters mine;
		flamethrowerParameters flamethrower;
		enemyBulletParameters enemyBullet;
	};

}Bullets;


void initBullets(Window* _window);

void createBullets(bulletType _type, int _ownerId, sfVector2f _pos, float _angle);

void updateBullets(Window* _window);

void displayBullets(Window* _window);

void deinitBullets();

