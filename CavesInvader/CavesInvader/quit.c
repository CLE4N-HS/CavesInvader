#include "quit.h"
#include "gamepad.h"

sfSprite* sprite;

void initQuit(Window* _window)
{
	sprite = sfSprite_create();
}

void updateQuit(Window* _window)
{
	static float timer = 0.f;
	timer += getUnscaledDeltaTime();
	static float timerQuit = 0.f;
	timerQuit += getUnscaledDeltaTime();

	if (timerQuit > 0.4f) {
		if (isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX) || isKeyboardOrControllerButtonPressed(sfKeyEscape, B_XBOX)) {
			timerQuit = 0.f;
			toggleQuit();
			return;
		}
	}


	if (timer > 0.2f)
	{

	}
}

void displayQuit(Window* _window)
{
	sfSprite_setTexture(sprite, GetTexture("menuBg"), sfTrue);
	sfSprite_setPosition(sprite, vector2f(0.f, 0.f));
	sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
}
