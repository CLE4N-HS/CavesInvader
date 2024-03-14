#pragma once
#include "tools.h"
#include "stateManager.h"
#include "gamepad.h"
#include "textureManager.h"


#define PLAYER_SPEED 500.f
#define LIGTHNING_SECONDS_REQUIRED 15
#define KILL_COUNT_REQUIRED 30

#define NB_SHADOWS 15

typedef struct Flames {
	sfTexture* texture;
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f scale;
}Flames;

typedef struct Shadows {
	sfVector2f pos;
}Shadows;

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
	int nbBullet;
	float bulletTimer;
	sfBool isMoving;
	float timeMoving;
	float particlesTimer;

	int nbGas;
	float fGasTimer;
	int nbLightning;
	float fLightningTimer;
	sfBool isLightning;
	sfBool isFlamethrowering;
	float flameThroweringTimer;
	float damageTimer;
	int damageFactor;

	int nbMine;
	int nbRespawn;
	sfBool hasShield;

	float invulnerabilityTimer;
	sfColor color;

	Shadows shadow[NB_SHADOWS];
	float shadowsTimer;

	float anothertimer;
	sfBool wasAlreadyMoving;

	int cid;
	//yeah better mb idk fr tbh
	sfBool ISMOVING;
}Players;
Players player[MAX_PLAYER];

typedef struct Common {
	int multiplier;
	float multiplierTimer;
	int score;
	int countdown;
	float fcountdownTimer;
	char name[10];
}Common;
Common common;

void initPlayer(Window* _window);

void updatePlayer(Window* _window);

void damagePlayer(int _playerId, int _damage);

void increasePlayerKillCount(int _playerId);

void displayPlayer(Window* _window);

void deinitPlayer();

/// <summary>
/// Gets the closest player position from a world position
/// </summary>
/// <param name="_pos">The world position away from players</param>
/// <returns>The world position of the closest player</returns>
sfVector2f getClosestPlayerPos(sfVector2f _pos);

sfVector2f getPlayerPos(int _playerId);

sfVector2f getPlayerVelocity(int _playerId);

int getPlayerDamageFactor(int _playerId);
