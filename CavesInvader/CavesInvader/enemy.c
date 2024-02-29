#include "enemy.h"
#include "textureManager.h"

sfSprite* enemySprite;

sfTexture* enemyTexture;

sfVector2f tmpPos;


void initEnemy(Window* _window)
{
	enemySprite = sfSprite_create();

	enemyTexture = GetTexture("enemies");

	sfSprite_setTexture(enemySprite, enemyTexture, sfTrue);

	tmpPos = vector2f(2000.f, rand_float(0.f, 800.f));
}

void updateEnemy(Window* _window)
{
	float dt = getDeltaTime();
	tmpPos.x -= dt * 1000.f;
	if (tmpPos.x < -500.f) {
		tmpPos = vector2f(2000.f, rand_float(0.f, 800.f));
	}
}

void displayEnemy(Window* _window)
{
	sfSprite_setTextureRect(enemySprite, IntRect(0, 411, 167, 114));
	sfSprite_setPosition(enemySprite, tmpPos);
	sfRenderTexture_drawSprite(_window->renderTexture, enemySprite, NULL);
}

void deinitEnemy()
{
	sfSprite_destroy(enemySprite);
}
