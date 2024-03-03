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
#include <Windows.h>

#define NB_BG 8


sfSprite* gameSprite;

typedef struct Backgrounds {
	sfTexture* texture;
	sfVector2f pos;
	float speed;
}Backgrounds;
Backgrounds bg[NB_BG];



float timer;
int nbreJoueur;
int waveCount;

float waveTimer;
float soloTimer;
float defaultwaveTimer;
float defaultsoloTimer;
float bossTimer;

sfBool phaseWave;
sfBool phaseBoss;
sfBool phaseBossCompleted;
sfBool gameover;

void initGame(Window* _window)
{
	Texture_Onload(GAME);
	Sound_Onload(GAME);
	Font_Onload(GAME);

	gameSprite = sfSprite_create();

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

	initParticlesSystem();
	initPlayer(_window);
	initEnemy(_window);
	initBullets(_window);
	initPause(_window);
	

	w.state = sfTrue;

}

void updateGame(Window* _window)
{
	float dt = getDeltaTime();
	timer += dt;


	if (isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX) && timer > 0.4f) {
		timer = 0.f;
		togglePause();
	}



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

	updateBullets(_window);
	updatePlayer(_window);
	updateEnemy(_window);
	updateParticlesSystem(_window);
}

void displayGame(Window* _window)
{
	for (int i = 0; i < NB_BG; i++)
	{
		sfSprite_setTexture(gameSprite, bg[i].texture, sfTrue);
		sfSprite_setPosition(gameSprite, bg[i].pos);
		sfRenderTexture_drawSprite(_window->renderTexture, gameSprite, NULL);
	}


	displayParticlesSystem(_window); // the particles will be behind the 2nd player
	displayEnemy(_window);
	displayBullets(_window);
	displayPlayer(_window);

}

void deinitGame()
{
	deinitPause();
	deinitPlayer();
	deinitEnemy();
	deinitBullets();
	sfSprite_destroy(gameSprite);
	//RemoveAllTextureButALL();
}