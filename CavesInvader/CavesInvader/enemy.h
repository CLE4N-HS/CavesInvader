#pragma once
#include "tools.h"
#include "customMath.h"
#include "stateManager.h"
#include "List.h"

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

	float ftimeInAOE;

	union parameters
	{
		vengeflyParameters vengefly;
		hopperParameters hopper;
		int tni;
	};
}Enemies;


stdList* enemiesList;


void initEnemy(Window* _window);

void createEnemy(enemyType _type);

void updateEnemy(Window* _window);

void displayEnemy(Window* _window);

void deinitEnemy();
