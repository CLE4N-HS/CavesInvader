#include "intro.h"
#include "textureManager.h"
#include "gamepadx.h"

sfSprite* introSprite;

sfTexture* introTexture;

float introTimer;
sfBool isCopyright;
sfColor introColor;

void initIntro(Window* _window)
{
	introSprite = sfSprite_create();

	introTexture = GetTexture("allStates");
	sfSprite_setTexture(introSprite, introTexture, sfTrue);

	introTimer = 0.f;
	isCopyright = sfTrue;
	introColor = color(255, 255, 255, 0);
}

void updateIntro(Window* _window)
{
	float udt = getUnscaledDeltaTime();
	introTimer += udt / 3.f;

	if (introTimer >= 1.f) {
		introColor.a = 255;
	}
	else {
		float fcolor = fabs(cosf(introTimer));
		sfUint8 color = 1.f - fcolor * 255 * 2;
		introColor.a = color;
	}

	if (isCopyright) {
		if (introTimer > 0.7f && isSomethingPressed(sfKeyEnter, A)) {
			introTimer = 0.f;
			isCopyright = sfFalse;
			introColor.a = 0;
		}
		
	}
	else {
		if (introTimer > 0.7f && isSomethingPressed(sfKeyEnter, A)) {
			changeState(_window, MENU);
		}
	}


}

void displayIntro(Window* _window)
{
	sfSprite_setPosition(introSprite, VECTOR2F_NULL);
	sfSprite_setColor(introSprite, introColor);
	if (isCopyright) {
		sfSprite_setTextureRect(introSprite, IntRect(0, 0, 1920, 1080));
		sfRenderTexture_drawSprite(_window->renderTexture, introSprite, NULL);
	}
	else {
		sfSprite_setTextureRect(introSprite, IntRect(0, 1080, 1920, 1080));
		sfRenderTexture_drawSprite(_window->renderTexture, introSprite, NULL);

		sfSprite_setPosition(introSprite, vector2f(257.f, 925.f));
		sfSprite_setTextureRect(introSprite, IntRect(0, 2160, 1407, 91));
		sfRenderTexture_drawSprite(_window->renderTexture, introSprite, NULL);
	}

}
