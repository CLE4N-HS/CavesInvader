#include "game.h"
#include "textureManager.h"
#include "soundManager.h"
#include "fontManager.h"
#include "viewManager.h"
#include "stateManager.h"
#include "Gamepad.h"
#include "pause.h"
#include "player.h"
#include "enemy.h"
#include "bullets.h"
#include "particlesSystemManager.h"
#include "hud.h"
#include "item.h"
#include "gameOver.h"
#include "options.h"
#include <Windows.h>
#include "gamepadx.h"

#define NB_BG 8


sfSprite* gameSprite;

typedef struct Backgrounds {
	sfTexture* texture;
	sfVector2f pos;
	float speed;
}Backgrounds;
Backgrounds bg[NB_BG];

float timer;
//int nbreJoueur;
//int waveCount;
//
//float waveTimer;
//float soloTimer;
//float defaultwaveTimer;
//float defaultsoloTimer;
//float bossTimer;
//
//sfBool phaseWave;
//sfBool phaseBoss;
//sfBool phaseBossCompleted;
//sfBool gameover;

int nbWave;
sfBool nextWave;
sfBool allowedToCreateEnemies;
int totalWaveEnemies;
int nbWaveEnemies;
float createEnemyTimer;


sfBool playGameOverSound;

float debugTimer;

void initGame(Window* _window)
{
	Texture_Onload(GAME);
	Sound_Onload(GAME);
	Font_Onload(GAME);

	gameSprite = sfSprite_create();
	timer = 0.f;
	nbWave = 0;
	nextWave = sfTrue;
	allowedToCreateEnemies = sfFalse;
	totalWaveEnemies = 0;
	nbWaveEnemies = 0;
	createEnemyTimer = 0.f;

	for (int i = 0; i < NB_BG; i++)
	{
		switch (i)
		{
		case 0:
			bg[i].texture = GetTexture("gameBg1");
			bg[i].pos = VECTOR2F_NULL;
			bg[i].speed = 50.f;
			break;
		case 1:
			bg[i].texture = GetTexture("gameBg2");
			bg[i].pos = vector2f(2880.f, 0.f);
			bg[i].speed = 50.f;
			break;
		case 2:
			bg[i].texture = GetTexture("gameCrystals1");
			bg[i].pos = VECTOR2F_NULL;
			bg[i].speed = 150.f;
			break;
		case 3:
			bg[i].texture = GetTexture("gameCrystals2");
			bg[i].pos = vector2f(2880.f, 0.f);
			bg[i].speed = 150.f;
			break;
		case 4:
			bg[i].texture = GetTexture("gameLightFg1");
			bg[i].pos = VECTOR2F_NULL;
			bg[i].speed = 300.f;
			break;
		case 5:
			bg[i].texture = GetTexture("gameLightFg2");
			bg[i].pos = vector2f(2880.f, 0.f);
			bg[i].speed = 300.f;
			break;
		case 6:
			bg[i].texture = GetTexture("gameDarkFg1");
			bg[i].pos = VECTOR2F_NULL;
			bg[i].speed = 650.f;
			break;
		case 7:
			bg[i].texture = GetTexture("gameDarkFg2");
			bg[i].pos = vector2f(2880.f, 0.f);
			bg[i].speed = 650.f;
			break;
		default:
			break;
		}
	}

	

	SetViewPosition(mainView, vector2f(mainView->defaultVideoMode.x / 2.0f, mainView->defaultVideoMode.y / 2.0f));
	
	
	GamepadDetection();

	//Sleep(2000); // to remove A enlever juste pour le test Thread

	initOptions(_window);
	initParticlesSystem();
	initPlayer(_window);
	initEnemy(_window);
	initBullets(_window);
	initPause(_window);
	initHud(_window);
	initItem(_window);
	initGameOver(_window);

	isGameOver = sfFalse;
	playGameOverSound = sfTrue;
	debugTimer = 0.f;

	PlayASound("madness", sfTrue);

	w.state = sfTrue;

}

void updateGame(Window* _window)
{
	float dt = getDeltaTime();
	timer += dt;
	debugTimer += getUnscaledDeltaTime();

	if (sfKeyboard_isKeyPressed(sfKeyO) && debugTimer > 1.f) {
		createItem(BULLET_ITEM, vector2f(1500.f, 140.f));
		createItem(LIFE_ITEM, vector2f(1500.f, 290.f));
		createItem(SHIELD_ITEM, vector2f(1500.f, 440.f));
		createItem(GAS_ITEM, vector2f(1500.f, 590.f));
		createItem(DAMAGE_ITEM, vector2f(1500.f, 740.f));
		createItem(TIMES3_ITEM, vector2f(1500.f, 890.f));

		debugTimer = 0.f;
	}

	// bg
	for (int i = 0; i < NB_BG; i++)
	{
		bg[i].pos.x -= bg[i].speed * dt;

		if (bg[i].pos.x <= -2880.f) {
			bg[i].pos.x += 5760.f;
					
			//if ((i % 2) == 0) {
			//	bg[i].pos.x = bg[i + 1].pos.x + 2880.f -  bg[i + 1].speed * dt;
			//}
			//else {
			//	bg[i].pos.x = bg[i - 1].pos.x + 2880.f;
			//}
		}
	}

	static float checkTimer = 0.f;
	checkTimer += dt;


	if (checkTimer > 1.f && createEnemyTimer > 5.f && nbWaveEnemies >= totalWaveEnemies && getTotalEnemies() == 0) {
		nextWave = sfTrue;
		checkTimer = 0.f;
	}

	if (nextWave) {
		nbWave += 1;
		nextWave = sfFalse;

		if ((nbWave % 10) == 0) {
			createEnemy(TAMER);
			allowedToCreateEnemies = sfFalse;
			totalWaveEnemies = 0;
		}
		else {
			allowedToCreateEnemies = sfTrue;
			totalWaveEnemies = nbWave;

			//int copyTotalEnemies = totalWaveEnemies;
			//int tmpTotalWaveEnemies = totalWaveEnemies;
			//
			//while (copyTotalEnemies > 10)
			//{
			//	totalWaveEnemies += tmpTotalWaveEnemies;
			//	copyTotalEnemies -= 10;
			//}

		}



		createEnemyTimer = 0.f;
		nbWaveEnemies = 0;
	}


	createEnemyTimer += dt * (1.f + (float)nbWave);

	if (allowedToCreateEnemies && createEnemyTimer > 5.f) {
		int randomEnemy = iRand(0, 4);
		if (randomEnemy == 0)
			createEnemy(VENGELFY);
		else if (randomEnemy == 1)
			createEnemy(ENRAGED_VENGEFLY);
		else if (randomEnemy == 2)
			createEnemy(HOPPER);
		else if (randomEnemy == 3)
			createEnemy(ENRAGED_HOPPER);

		nbWaveEnemies += 1;
		if (nbWaveEnemies >= totalWaveEnemies) {
			allowedToCreateEnemies = sfFalse;
		}

		createEnemyTimer = 0.f;
	}

	if (isGameOver && playGameOverSound) {
		PlayASound("gameOverSfx", sfFalse);
		playGameOverSound = sfFalse;
	}



	updateBullets(_window);
	updatePlayer(_window);
	updateEnemy(_window);
	updateParticlesSystem(_window);
	updateHud(_window);
	updateItem(_window);

	if (isGameOver)
		updateGameOver(_window);

	else if (/*isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX)*/ isSomethingPressed(sfKeyEscape, START) && timer > 0.4f) {
		timer = 0.f;
		togglePause();
		StopASound("blaster");
		StopASound("laserSfx");
		StopASound("explosionSfx");
		StopASound("flameThrowerSfx");
		StopASound("enemyDeath");
		StopASound("collectItem");
		for (int i = 0; i < nbPlayer; i++)
		{
			setVibration(i, 0.f, 0.f);
		}
	}
}

int getNbWave()
{
	return nbWave;
}

int getNb10Waves()
{
	int nb10Wave = 1;
	int tmpWave = nbWave;
	while (tmpWave > 10)
	{
		tmpWave -= 10;
		nb10Wave += 1;
	}
	return nb10Wave;
}

void displayGame(Window* _window)
{
	for (int i = 0; i < NB_BG; i++)
	{
		sfSprite_setTexture(gameSprite, bg[i].texture, sfTrue);
		sfSprite_setPosition(gameSprite, bg[i].pos);
		sfRenderTexture_drawSprite(_window->renderTexture, gameSprite, NULL);
	}


	displayParticlesSystem(_window); // the particles will be behind the 2nd player, mb add a layer sytem but it will be big
	displayItem(_window);
	displayBullets(_window);
	displayEnemy(_window);
	displayPlayer(_window);

	if (isGameOver)
		displayGameOver(_window);
	else
		displayHud(_window);
}

void deinitGame()
{
	deinitPause();
	deinitPlayer();
	deinitEnemy();
	deinitBullets();
	deinitHud();
	deinitItem();
	deinitGameOver();

	sfSprite_destroy(gameSprite);
	//RemoveAllTextureButALL();
}