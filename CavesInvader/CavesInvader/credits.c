#include "credits.h"
#include "textureManager.h"
#include "gamepadx.h"

sfSprite* creditsSprite;

sfTexture* creditsTexture;
sfTexture* creditsBoxTexture;

void initCredits(Window* _window)
{
	creditsSprite = sfSprite_create();

	creditsTexture = GetTexture("credits");
	creditsBoxTexture = GetTexture("creditsBox");
}

void updateCredits(Window* _window)
{
	static float timer = 0.f;
	float udt = getUnscaledDeltaTime();
	timer += udt;
	if ((isSomethingPressed(sfKeyEnter, A) || isSomethingPressed(sfKeyEscape, B)) && timer > 0.4f) {
		timer = 0.f;
		changeMenuState(MENU_MAIN);
	}
}

void displayCredits(Window* _window)
{
	sfSprite_setTexture(creditsSprite, creditsTexture, sfTrue);
	sfSprite_setPosition(creditsSprite, vector2f(211.f, 90.f));
	sfRenderTexture_drawSprite(_window->renderTexture, creditsSprite, NULL);

	sfSprite_setTexture(creditsSprite, creditsBoxTexture, sfTrue);
	sfSprite_setPosition(creditsSprite, vector2f(862.f, 869.f));
	sfRenderTexture_drawSprite(_window->renderTexture, creditsSprite, NULL);
}

void deinitCredits()
{
	sfSprite_destroy(creditsSprite);
}
