#pragma once
#include "tools.h"
#include "stateManager.h"
#include "gamepad.h"
#include "textureManager.h"

typedef struct Players {
	sfTexture* texture;
	sfVector2f pos;
	sfVector2f speed;
	sfVector2f velocity;
	sfVector2f forward;
	sfVector2f previousForward;
	float drag;
	sfBool isMoving;
	float timeMoving;
	float anothertimer;
}Players;
Players player[MAX_PLAYER];


void initPlayer(Window* _window);

void updatePlayer(Window* _window);

void displayPlayer(Window* _window);

void deinitPlayer();
