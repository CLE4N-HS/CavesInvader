#include "nameChoice.h"
#include "textureManager.h"
#include "fontManager.h"
#include "gamepadx.h"
#include "player.h"

sfSprite* nameChoiceSprite;

sfTexture* nameChoiceTexture;

sfText* nameChoiceText;
char nameChoiceChar[3];
char playerName[3];

sfFont* nameChoiceFont;

char letters[3];
int choiceName;

void initNameChoice(Window* _window)
{
	nameChoiceSprite = sfSprite_create();

	nameChoiceTexture = GetTexture("allMenus");
	sfSprite_setTexture(nameChoiceSprite, nameChoiceTexture, sfFalse);

	nameChoiceText = sfText_create();

	nameChoiceFont = GetFont("nasalization-rg");
	sfText_setFont(nameChoiceText, nameChoiceFont);
	sfText_setCharacterSize(nameChoiceText, 90);
	sfText_setColor(nameChoiceText, sfBlack);

	
	for (int i = 0; i < 3; i++)
	{
		letters[i] = 65;
	}
	choiceName = 0;
}

void updateNameChoice(Window* _window)
{
	float LStickValueX = getDominantStickPos(sfTrue, sfTrue);
	float LStickValueY = getDominantStickPos(sfTrue, sfFalse);

	float dt = getUnscaledDeltaTime();
	static float timer = 0.f;
	timer += dt;

	if ((LStickValueX > 50.f || sfKeyboard_isKeyPressed(sfKeyRight)) && timer > 0.2f) {
		choiceName += 1;
		timer = 0.f;
		if (choiceName > 3)
			choiceName = 2;

	}
	else if ((LStickValueX < -50.f || sfKeyboard_isKeyPressed(sfKeyLeft)) && timer > 0.2f) {
		choiceName -= 1;
		timer = 0.f;
		if (choiceName == 3)
			choiceName = 2;
		else if (choiceName < 0)
			choiceName = 2;
	}

	if ((LStickValueY < -50.f || sfKeyboard_isKeyPressed(sfKeyDown)) && timer > 0.2f && choiceName < 3) {
		letters[choiceName] += 1;
		timer = 0.f;
		if (letters[choiceName] > 90)
			letters[choiceName] = 65;
	}
	else if ((LStickValueY > 50.f || sfKeyboard_isKeyPressed(sfKeyUp)) && timer > 0.2f && choiceName < 4) {
		if (choiceName == 3)
			choiceName = 1;
		else {
			letters[choiceName] -= 1;
			if (letters[choiceName] < 65)
				letters[choiceName] = 90;
		}
		timer = 0.f;
	}


	if (choiceName == 3 && isSomethingPressed(sfKeyEnter, A)) {
		sprintf(common.name, "%c%c%c", letters[0], letters[1], letters[2]);
		changeState(_window, GAME);
	}
	
	if (isSomethingPressed(sfKeyEscape, B)) {
		changeMenuState(MENU_MAIN);
		choiceName = 0;
	}

}

void displayNameChoice(Window* _window)
{
	sfSprite_setTextureRect(nameChoiceSprite, IntRect(0, 0, 1010, 727));
	sfSprite_setPosition(nameChoiceSprite, vector2f(455.f, 176.f));
	sfRenderTexture_drawSprite(_window->renderTexture, nameChoiceSprite, NULL);

	if (choiceName == 0) {
		sfSprite_setTextureRect(nameChoiceSprite, IntRect(0, 727, 31, 230));
		sfSprite_setPosition(nameChoiceSprite, vector2f(669.f, 438.f));
		sfRenderTexture_drawSprite(_window->renderTexture, nameChoiceSprite, NULL);
	}
	else if (choiceName == 1) {
		sfSprite_setTextureRect(nameChoiceSprite, IntRect(0, 727, 31, 230));
		sfSprite_setPosition(nameChoiceSprite, vector2f(945.f, 438.f));
		sfRenderTexture_drawSprite(_window->renderTexture, nameChoiceSprite, NULL);
	}
	else if (choiceName == 2) {
		sfSprite_setTextureRect(nameChoiceSprite, IntRect(0, 727, 31, 230));
		sfSprite_setPosition(nameChoiceSprite, vector2f(1221.f, 438.f));
		sfRenderTexture_drawSprite(_window->renderTexture, nameChoiceSprite, NULL);
	}
	else if (choiceName == 3) {
		sfSprite_setTextureRect(nameChoiceSprite, IntRect(0, 957, 319, 78));
		sfSprite_setPosition(nameChoiceSprite, vector2f(801.f, 738.f));
		sfRenderTexture_drawSprite(_window->renderTexture, nameChoiceSprite, NULL);
	}

	for (int i = 0; i < 3; i++)
	{
		sprintf(nameChoiceChar, "%c", letters[i]);
		sfText_setString(nameChoiceText, nameChoiceChar);
		sfText_setPosition(nameChoiceText, vector2f(683.f + i * 275.f, 492.f));
		sfFloatRect tmpRect = sfText_getLocalBounds(nameChoiceText);
		sfText_setOrigin(nameChoiceText, TEXTSTRINGORIGIN);
		sfRenderTexture_drawText(_window->renderTexture, nameChoiceText, NULL);
	}
}

void deinitNameChoice()
{
	sfSprite_destroy(nameChoiceSprite);
	sfText_destroy(nameChoiceText);
}
