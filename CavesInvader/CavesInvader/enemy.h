#pragma once
#include "tools.h"
#include "customMath.h"
#include "stateManager.h"

typedef enum enemyType {
	MOSQUITO,
	ENRAGED_MOSQUITO,


};


typedef struct Enemies {
	sfIntRect rect;
	sfVector2f pos;
	sfVector2f velocity;
	float speed;
	int life;
}Enemies;


void initEnemy(Window* _window);

void createEnemy();

void updateEnemy(Window* _window);

void displayEnemy(Window* _window);

void deinitEnemy();
