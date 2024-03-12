#include "gameOver.h"
#include "textureManager.h"
#include "fontManager.h"
#include "gamepadx.h"
#include "player.h"

sfSprite* gameOverSprite;

sfTexture* gameOverTexture;

sfText* gameOverText;
char gameOverChar[20];

sfFont* gameOverFont;

sfColor gameOverColor;

int gameOverChoice;

void initGameOver(Window* _window)
{
	//FILE* file;
	//
	//file = fopen(FILE_PATH"playerName.txt", "a");
	//fclose(file);

	gameOverSprite = sfSprite_create();

	gameOverTexture = GetTexture("allStates");
	sfSprite_setTexture(gameOverSprite, gameOverTexture, sfFalse);

	gameOverText = sfText_create();

	gameOverFont = GetFont("nasalization-rg");
	sfText_setFont(gameOverText, gameOverFont);

	gameOverColor = color(255, 255, 255, 0);

	gameOverChoice = -1;
}

void updateGameOver(Window* _window)
{
	float udt = getUnscaledDeltaTime();
	static float timer = 0.f;
	timer += udt;
	static float quitTimer = 0.f;
	quitTimer += udt;

	timeScale -= udt / 3.f;
	timeScale = MAX(timeScale, 0.f);

	float fColor = fabs(cosf(timeScale));
	sfUint8 color = fColor * 255 * 2;
	color = MAX(color, 0);
	color = MIN(color, 255);

	gameOverColor.a = color;
	
	if (isSomethingMoved(sfKeyLeft, sfTrue, -50.f) < -50.f && timer > 2.f) {
		gameOverChoice -= 1;
		timer = 1.8f;
		if (gameOverChoice < 0)
			gameOverChoice = 0;
	}
	else if (isSomethingMoved(sfKeyRight, sfTrue, 50.f) > 50.f && timer > 2.f) {
		if (gameOverChoice == -1) gameOverChoice = 0;
		gameOverChoice += 1;
		timer = 1.8f;
		if (gameOverChoice > 1)
			gameOverChoice = 1;
	}


	if (isSomethingPressed(sfKeyEscape, B) && quitTimer > 0.2f) {
		timeScale = 1.f;
		changeMenuState(MENU_LEADERBOARD);
		changeState(_window, MENU);
	}
	else if (isSomethingPressed(sfKeyEnter, A) && quitTimer > 0.2f && timer > 2.f) {
		if (gameOverChoice == 0) {
			timeScale = 1.f;
			changeMenuState(MENU_MAIN);
			changeState(_window, MENU);
		}
		else if (gameOverChoice == 1) {
			timeScale = 1.f;
			changeMenuState(MENU_CHOICENAME);
			changeState(_window, MENU);
		}
		else {
			gameOverChoice = 0;
			timer = 1.8f;
		}
	}
}

void displayGameOver(Window* _window)
{
	if (gameOverChoice == 0) {
		sfSprite_setTextureRect(gameOverSprite, IntRect(0, 3390, 370, 63));
		sfSprite_setPosition(gameOverSprite, vector2f(126.f, 1006.f));
		sfRenderTexture_drawSprite(_window->renderTexture, gameOverSprite, NULL);
	}
	else {
		sfSprite_setTextureRect(gameOverSprite, IntRect(0, 3331, 363, 59));
		sfSprite_setPosition(gameOverSprite, vector2f(130.f, 1008.f));
		sfRenderTexture_drawSprite(_window->renderTexture, gameOverSprite, NULL);
	}

	if (gameOverChoice == 1) {
		sfSprite_setTextureRect(gameOverSprite, IntRect(0, 3390, 370, 63));
		sfSprite_setPosition(gameOverSprite, vector2f(1444.f, 1006.f));
		sfRenderTexture_drawSprite(_window->renderTexture, gameOverSprite, NULL);
	}
	else {
		sfSprite_setTextureRect(gameOverSprite, IntRect(0, 3331, 363, 59));
		sfSprite_setPosition(gameOverSprite, vector2f(1447.f, 1008.f));
		sfRenderTexture_drawSprite(_window->renderTexture, gameOverSprite, NULL);
	}

	sfSprite_setColor(gameOverSprite, gameOverColor);
	sfSprite_setTextureRect(gameOverSprite, IntRect(0, 2251, 1920, 1080));
	sfSprite_setPosition(gameOverSprite, VECTOR2F_NULL);
	sfRenderTexture_drawSprite(_window->renderTexture, gameOverSprite, NULL);

	if (sfKeyboard_isKeyPressed(sfKeyP))
		common.score = 9475430;
	else
		common.score = 10145;
	sprintf(gameOverChar, "%d", common.score);

	if (common.score >= 1000000)
		sfText_setCharacterSize(gameOverText, 50.f);
	else
		sfText_setCharacterSize(gameOverText, 60.f);
	sfText_setString(gameOverText, gameOverChar);
	sfText_setPosition(gameOverText, vector2f(1070.f, 998.f));
	sfText_setColor(gameOverText, color(0, 0, 0, gameOverColor.a));
	sfFloatRect tmpRect = sfText_getLocalBounds(gameOverText);
	sfText_setOrigin(gameOverText, TEXTORIGIN);
	sfRenderTexture_drawText(_window->renderTexture, gameOverText, NULL);

}

void deinitGameOver()
{
	sfSprite_destroy(gameOverSprite);
	sfText_destroy(gameOverText);
}