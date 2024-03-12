#include "leaderboard.h"
#include "textureManager.h"
#include "fontManager.h"
#include "gamepadx.h"

sfSprite* leaderboardSprite;

sfTexture* leaderboardTexture;

sfText* leaderboardText;
char leaderboardChar[20];

sfFont* leaderboardFont;

typedef struct {
	int score;
	char name[10];
}Leaderboard;
Leaderboard ld[3];

void initLeaderboard(Window* _window)
{
	FILE* file;

	file = fopen(FILE_PATH"playerName.txt", "a");
	fclose(file);

	leaderboardSprite = sfSprite_create();

	leaderboardTexture = GetTexture("allMenus");
	sfSprite_setTexture(leaderboardSprite, leaderboardTexture, sfFalse);

	leaderboardText = sfText_create();

	leaderboardFont = GetFont("nasalization-rg");
	sfText_setFont(leaderboardText, leaderboardFont);
	sfText_setCharacterSize(leaderboardText, 55.f);
	sfText_setColor(leaderboardText, sfBlack);

	// to remove
	for (int i = 0; i < 3; i++)
	{
		ld[i].score = 8000008 + i * 1000;
		strcpy(ld[i].name, "wow");
	}
	
}

void updateLeaderboard(Window* _window)
{
	static float timer = 0.f;
	float udt = getUnscaledDeltaTime();
	timer += udt;

	if (isSomethingPressed(sfKeyEscape, B) && timer > 0.2f) {
		changeMenuState(MENU_MAIN);
		timer = 0.f;
	}
	else if (isSomethingPressed(sfKeyEnter, A) && timer > 0.2f) {
		changeMenuState(MENU_MAIN);
		timer = 0.f;
	}
}

void displayLeaderboard(Window* _window)
{
	sfSprite_setTextureRect(leaderboardSprite, IntRect(0, 6193, 879, 900));
	sfSprite_setPosition(leaderboardSprite, vector2f(521.f, 90.f));
	sfRenderTexture_drawSprite(_window->renderTexture, leaderboardSprite, NULL);

	sfSprite_setTextureRect(leaderboardSprite, IntRect(0, 7093, 238, 52));
	sfSprite_setPosition(leaderboardSprite, vector2f(841.f, 852.f));
	sfRenderTexture_drawSprite(_window->renderTexture, leaderboardSprite, NULL);

	for (int i = 0; i < 3; i++)
	{
		sprintf(leaderboardChar, "%d", ld[i].score);
		sfText_setString(leaderboardText, leaderboardChar);
		sfText_setPosition(leaderboardText, vector2f(1014.f, 415.f + i * 135.f));
		sfFloatRect tmpRect = sfText_getLocalBounds(leaderboardText);
		sfText_setOrigin(leaderboardText, TEXTSTRINGORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, leaderboardText, NULL);

		sprintf(leaderboardChar, "%s", ld[i].name);
		sfText_setString(leaderboardText, leaderboardChar);
		sfText_setPosition(leaderboardText, vector2f(1278.f, 415.f + i * 135.f));
		tmpRect = sfText_getLocalBounds(leaderboardText);
		sfText_setOrigin(leaderboardText, TEXTSTRINGORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, leaderboardText, NULL);
	}

}

void deinitLeaderboard()
{
	sfSprite_destroy(leaderboardSprite);
	sfText_destroy(leaderboardText);
}
