#include "leaderboard.h"
#include "textureManager.h"
#include "fontManager.h"
#include "gamepadx.h"
#include "player.h"

sfSprite* leaderboardSprite;

sfTexture* leaderboardTexture;

sfText* leaderboardText;
char leaderboardChar[20];

sfFont* leaderboardFont;

typedef struct Leaderboard{
	int score;
	char name[10];
}Leaderboard;
Leaderboard ld[3];

void initLeaderboard(Window* _window)
{

	leaderboardSprite = sfSprite_create();

	leaderboardTexture = GetTexture("allMenus");
	sfSprite_setTexture(leaderboardSprite, leaderboardTexture, sfFalse);

	leaderboardText = sfText_create();

	leaderboardFont = GetFont("nasalization-rg");
	sfText_setFont(leaderboardText, leaderboardFont);
	sfText_setCharacterSize(leaderboardText, 55.f);
	sfText_setColor(leaderboardText, sfBlack);
	
}

void updateLeaderboard(Window* _window)
{
	static float timer = 0.f;
	float udt = getUnscaledDeltaTime();
	timer += udt;

	if (isSomethingPressed(sfKeyEscape, B) && timer > 0.4f) {
		changeMenuState(MENU_MAIN);
		timer = 0.f;
	}
	else if (isSomethingPressed(sfKeyEnter, A) && timer > 0.4f) {
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

void sortLeaderboard()
{
	// create a copy of the leaderboard
	int tmpCurrentScore = common.score;
	char tmpCurrentName[10];
	strcpy(tmpCurrentName, common.name);
	Leaderboard tmpLd[3];

	for (int i = 0; i < 3; i++)
	{
		tmpLd[i].score = ld[i].score;
		strcpy(tmpLd[i].name, ld[i].name);
	}

	sfBool firstPass = sfTrue;
	sfBool secondPass = sfTrue;
	sfBool thirdPass = sfTrue;

	int ii = 0;

	// sort the copy
	if (tmpCurrentScore >= tmpLd[2].score) {
		tmpLd[2].score = tmpCurrentScore;
		strcpy(tmpLd[2].name, tmpCurrentName);
	}
	if (tmpCurrentScore >= tmpLd[1].score) {
		int tmpScore1 = tmpLd[1].score;
		char tmpName1[10];
		strcpy(tmpName1, tmpLd[1].name);
		tmpLd[1].score = tmpCurrentScore;
		tmpLd[2].score = tmpScore1;
		strcpy(tmpLd[1].name, tmpCurrentName);
		strcpy(tmpLd[2].name, tmpName1);
	}
	if (tmpCurrentScore >= tmpLd[0].score) {
		int tmpScore0 = tmpLd[0].score;
		char tmpName0[10];
		strcpy(tmpName0, tmpLd[0].name);
		tmpLd[0].score = tmpCurrentScore;
		tmpLd[1].score = tmpScore0;
		strcpy(tmpLd[0].name, tmpCurrentName);
		strcpy(tmpLd[1].name, tmpName0);
	}



	//while (tmpLd[2].score > tmpLd[1].score || tmpLd[1].score > tmpLd[0].score || tmpLd[2].score < tmpCurrentScore)
	//{
	//	if (tmpCurrentScore >= tmpLd[2].score && firstPass) {
	//		tmpLd[2].score = tmpCurrentScore;
	//		strcpy(tmpLd[2].name, tmpCurrentName);
	//		//printf("1st condition\n");
	//		firstPass = sfFalse;
	//	}
	//	if (tmpCurrentScore > tmpLd[1].score && secondPass) {
	//		int tmpScore1 = tmpLd[1].score;
	//		char tmpName1[10];
	//		strcpy(tmpName1, tmpLd[1].name);
	//		tmpLd[1].score = tmpCurrentScore;
	//		tmpLd[2].score = tmpScore1;
	//		strcpy(tmpLd[1].name, tmpCurrentName);
	//		strcpy(tmpLd[2].name, tmpName1);
	//		//printf("2nd condition\n");
	//		secondPass = sfFalse;
	//	}
	//	if (tmpCurrentScore > tmpLd[0].score && thirdPass) {
	//		int tmpScore0 = tmpLd[0].score;
	//		char tmpName0[10];
	//		strcpy(tmpName0, tmpLd[0].name);
	//		tmpLd[0].score = tmpCurrentScore;
	//		tmpLd[1].score = tmpScore0;
	//		strcpy(tmpLd[0].name, tmpCurrentName);
	//		strcpy(tmpLd[1].name, tmpName0);
	//		//printf("3rd condition\n");
	//		thirdPass = sfFalse;
	//	}

	//	if (!firstPass && !secondPass && !thirdPass)
	//		break;

	//	ii++;
	//	if (ii < 10)
	//		break;
	//}

	// transfer the copy into the real Leaderboard
	for (int i = 0; i < 3; i++)
	{
		ld[i].score = tmpLd[i].score;
		strcpy(ld[i].name, tmpLd[i].name);

	}
}

void saveLeaderboard()
{
	sortLeaderboard();
	
	// store it in a file
	FILE* file;
	file = fopen(FILE_PATH"leaderboard.ld", "wb");
	fwrite(&ld, sizeof(struct Leaderboard), 3, file);
	fclose(file);
}

void loadLeaderboard()
{
	FILE* file;
	file = fopen(FILE_PATH"leaderboard.ld", "rb");
	if (file == NULL) {
		
		for (int i = 0; i < 3; i++)
		{
			ld[i].score = 0;
			strcpy(ld[i].name, "???");
		}

		file = fopen(FILE_PATH"leaderboard.ld", "ab");
		file = fclose(file);
		
		file = fopen(FILE_PATH"leaderboard.ld", "rb");
	}
	fread(&ld, sizeof(struct Leaderboard), 3, file);
	fclose(file);
}
