#pragma once
#include "tools.h"
#include "customMath.h"
#include "stateManager.h"

typedef enum enemyType {
	VENGELFY,
	ENRAGED_VENGEFLY,
	HOPPER,
	ENRAGED_HOPPER,
	TAMER
}enemyType;




void initEnemy(Window* _window);

void createEnemy(enemyType _type);

void updateEnemy(Window* _window);

void displayEnemy(Window* _window);

void deinitEnemy();
