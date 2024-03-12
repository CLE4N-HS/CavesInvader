#include "leaderboard.h"
#include "textureManager.h"
#include "fontManager.h"

sfSprite* leaderboardSprite;

sfTexture* leaderboardTexture;

sfText* leaderboardText;
char leaderboardChar[20];

sfFont* leaderboardFont;

void initLeaderboard(Window* _window)
{
	FILE* file;

	file = fopen(FILE_PATH"playerName.txt", "a");
	fclose(file);

	leaderboardSprite = sfSprite_create();

	leaderboardTexture = GetTexture("allMenus");
	sfSprite_setTexture(leaderboardSprite, leaderboardTexture, sfFalse);

	leaderboardText = sfText_create();

	leaderboardFont = GetFont("nasalization-rb");
	sfText_setFont(leaderboardText, leaderboardFont);
	sfText_setCharacterSize(leaderboardText, 30.f);
	sfText_setColor(leaderboardText, sfBlack);

	
}

void updateLeaderboard(Window* _window)
{
	
}

void displayLeaderboard(Window* _window)
{
	sfSprite_setTextureRect(leaderboardSprite, IntRect(0, 6193, 879, 900));
	sfSprite_setPosition(leaderboardSprite, vector2f(521.f, 90.f));
	sfRenderTexture_drawSprite(_window->renderTexture, leaderboardSprite, NULL);

	sfSprite_setTextureRect(leaderboardSprite, IntRect(0, 7093, 238, 52));
	sfSprite_setPosition(leaderboardSprite, vector2f(841.f, 852.f));
	sfRenderTexture_drawSprite(_window->renderTexture, leaderboardSprite, NULL);

}
