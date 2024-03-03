#pragma once
#include "tools.h"
#include "stateManager.h"
#include "gamepad.h"
#include "textureManager.h"

#define PLAYER_SPEED 500.f

typedef struct Flames {
	sfTexture* texture;
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f scale;
}Flames;

typedef struct Players {
	sfTexture* texture;
	int life;
	sfVector2f pos;
	sfVector2f origin;
	sfFloatRect bounds;
	Flames flame;
	float speed;
	sfVector2f velocity;
	sfVector2f forward;
	sfVector2f previousForward;
	float drag;
	float bulletTimer;
	sfBool isMoving;
	float timeMoving;
	float particlesTimer;

	// useless but i guess we never know
	float anothertimer;
	sfBool wasalreadymovingtbh;
	sfBool wasnt;
}Players;
Players player[MAX_PLAYER];



void initPlayer(Window* _window);

void updatePlayer(Window* _window);

void displayPlayer(Window* _window);

void deinitPlayer();

/// <summary>
/// Gets the closest player position from a world position
/// </summary>
/// <param name="_pos">The world position away from players</param>
/// <returns>The world position of the closest player</returns>
sfVector2f getClosestPlayerPos(sfVector2f _pos);

sfVector2f getPlayerPos(int _playerId);
