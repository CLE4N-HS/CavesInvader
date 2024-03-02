#pragma once
#include "tools.h"
#include "customMath.h"
#include "stateManager.h"
#include "List.h"

#define GETDATA_ENEMIES STD_LIST_GETDATA(enemiesList, Enemies, i)

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


typedef struct Enemies {
	enemyType type;
	enemyState state;
	enemyState lastState;
	sfIntRect rect;
	sfFloatRect bounds;
	sfVector2f origin;
	float animTimer;
	float timeBetweenFrames;
	sfVector2f pos;
	sfVector2f velocity;
	sfVector2f forward;
	float speed;
	int life;
	int damage;

	union parameters
	{
		vengeflyParameters vengefly;
		int tni;
	};
}Enemies;


stdList* enemiesList;


void initEnemy(Window* _window);

void createEnemy(enemyType _type);

void updateEnemy(Window* _window);

void displayEnemy(Window* _window);

void deinitEnemy();
