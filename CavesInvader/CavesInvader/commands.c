#include "commands.h"
#include "textureManager.h"
#include "gamepadx.h"

sfSprite* commandsSprite;

sfTexture* commandsTexture;
sfTexture* commandsBoxTexture;

void initCommands(Window* _window)
{
	commandsSprite = sfSprite_create();

	commandsTexture = GetTexture("commands");
	commandsBoxTexture = GetTexture("commandsBox");
}

void updateCommands(Window* _window)
{
	static float timer = 0.f;
	float udt = getUnscaledDeltaTime();
	timer += udt;
	if ((isSomethingPressed(sfKeyEnter, A) || isSomethingPressed(sfKeyEscape, B)) && timer > 0.4f) {
		timer = 0.f;
		changeMenuState(MENU_MAIN);
	}
}

void displayCommands(Window* _window)
{
	sfSprite_setTexture(commandsSprite, commandsTexture, sfTrue);
	sfSprite_setPosition(commandsSprite, vector2f(211.f, 90.f));
	sfRenderTexture_drawSprite(_window->renderTexture, commandsSprite, NULL);

	sfSprite_setTexture(commandsSprite, commandsBoxTexture, sfTrue);
	sfSprite_setPosition(commandsSprite, vector2f(862.f, 869.f));
	sfRenderTexture_drawSprite(_window->renderTexture, commandsSprite, NULL);
}

void deinitCommands()
{
	sfSprite_destroy(commandsSprite);
}
