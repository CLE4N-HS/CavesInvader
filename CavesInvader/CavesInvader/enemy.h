#pragma once
#include "tools.h"
#include "customMath.h"
#include "stateManager.h"
#include "List.h"

#define BOSS_HEALTH 10

typedef enum enemyType {
	VENGELFY,
	ENRAGED_VENGEFLY,
	HOPPER,
	ENRAGED_HOPPER,
	TAMER
}enemyType;


typedef enum enemyState {
	NO_STATE = -1,
	FLYING,
	FOCUSING,
	ATTACKING,
	DEAD
}enemyState;

typedef enum bossPhase {
	PHASE0,
	PHASE1,
	PHASE2,
	PHASE3,
	PHASE4
}bossPhase;

typedef struct vengeflyParameters {
	float startFocusingPos;
	float startAttackingMoment;
	float startAttackingTimer;
}vengeflyParameters;

typedef struct hopperParameters {
	float startAttackingPos;
	float focusingTimer;
	float startAttackingMoment;
	float startAttackingTimer;
	sfBool upMovement;
}hopperParameters;

typedef struct tamerParameters {
	bossPhase phase;
	bossPhase lastPhase;
	float startAttackingPos;
	float focusingTimer;
	float startAttackingMoment;
	float startAttackingTimer;
	sfBool upMovement;
	int nbBullets;
	int totalBullets;
	float wantedSpeed;

	sfBool isSpecial;
	float specialAnimTimer;
	float specialTimer;
	float specialMoment;
	sfBool canLaunchBullet;
	sfBool shouldResetPos;

}tamerParameters;

typedef struct Enemies {
	enemyType type;
	enemyState state;
	enemyState lastState;
	sfIntRect rect;
	sfFloatRect bounds;
	sfVector2f origin;
	sfVector2f originToCenter;
	float radius;
	float animTimer;
	float timeBetweenFrames;
	sfVector2f pos;
	sfVector2f velocity;
	sfVector2f forward;
	float speed;
	int life;
	int damage;
	int scoreValue;

	float ftimeInAOE;
	int lastDamageSource;

	union parameters
	{
		vengeflyParameters vengefly;
		hopperParameters hopper;
		tamerParameters tamer;
	};
}Enemies;


stdList* enemiesList;


void initEnemy(Window* _window);

void createEnemy(enemyType _type);

void updateEnemy(Window* _window);

void resetBossPosition();

void displayEnemy(Window* _window);

void deinitEnemy();
