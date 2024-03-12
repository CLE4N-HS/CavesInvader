#include "quit.h"
#include "gamepad.h"
#include "menu.h"
#include "pause.h"

typedef enum QuitChoice {
	NO_QUIT,
	YES_QUIT
}QuitChoice;
QuitChoice choiceQuit;


sfRectangleShape* opaqueRectangle;

sfSprite* quitSprite;

sfTexture* menuBgTexture;
sfTexture* quitTexture;
sfTexture* quitBoxTexture;

float timer;
float timerQuit;


void resetQuit()
{
	choiceQuit = NO_QUIT;
	timer = 0.f;
	timerQuit = 0.f;
}

void initQuit(Window* _window)
{
	opaqueRectangle = sfRectangleShape_create();
	sfRectangleShape_setFillColor(opaqueRectangle, color(0, 0, 0, 127));
	sfRectangleShape_setPosition(opaqueRectangle, VECTOR2F_NULL);
	sfRectangleShape_setSize(opaqueRectangle, vector2f(1920.f, 1080.f));

	quitSprite = sfSprite_create();

	menuBgTexture = GetTexture("menuBg");
	quitTexture = GetTexture("quit");
	quitBoxTexture = GetTexture("quitBox");


	resetQuit();
}

void updateQuit(Window* _window)
{
	timer += getUnscaledDeltaTime();
	timerQuit += getUnscaledDeltaTime();

	if (timerQuit > 0.4f) {
		if (isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX) || isKeyboardOrControllerButtonPressed(sfKeyEscape, B_XBOX)) {
			timerQuit = 0.f;
			toggleQuit();
			resetQuit();
			if (getState() == MENU) {
				resetMenu();
			}
			else {
				togglePause();
				resetPause();
			}
			return;
		}
		else if (isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)) {
			
			if (getState() == MENU) {
				if (choiceQuit == YES_QUIT) {
					_window->isDone = sfTrue;
				}
				else {
					toggleQuit();
					resetQuit();
					resetMenu();
				}
			}
			else {
				if (choiceQuit == YES_QUIT) {
					toggleQuit();
					resetQuit();
					resetMenu();
					changeMenuState(MENU_MAIN);
					changeState(_window, MENU);
				}
				else {
					toggleQuit();
					resetQuit();
					togglePause();
					resetMenu();
				}
			}
			return;

		}
	}


	if (timer > 0.2f)
	{
		if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 20.f)) {
			choiceQuit = YES_QUIT;
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 20.f)) {
			choiceQuit = NO_QUIT;
		}
	}
}

void displayQuit(Window* _window)
{
	// bg
	if (getState() == MENU) {
		sfSprite_setTexture(quitSprite, menuBgTexture, sfTrue);
		sfSprite_setPosition(quitSprite, vector2f(0.f, 0.f));
		sfRenderTexture_drawSprite(_window->renderTexture, quitSprite, NULL);
	}
	else {
	// opacity
	sfRenderTexture_drawRectangleShape(_window->renderTexture, opaqueRectangle, NULL);
	}


	// main
	sfSprite_setTexture(quitSprite, quitTexture, sfTrue);
	sfSprite_setPosition(quitSprite, vector2f(385.f, 176.f)); // bro reverted x and y
	sfRenderTexture_drawSprite(_window->renderTexture, quitSprite, NULL);

	// boxes
	sfSprite_setTexture(quitSprite, quitBoxTexture, sfTrue);
	if (choiceQuit == YES_QUIT) sfSprite_setPosition(quitSprite, vector2f(489.f, 562.f));
	else sfSprite_setPosition(quitSprite, vector2f(1081.f, 562.f));
	sfRenderTexture_drawSprite(_window->renderTexture, quitSprite, NULL);

}

void deinitQuit()
{
	sfRectangleShape_destroy(opaqueRectangle);

	sfSprite_destroy(quitSprite);

	//sfTexture_destroy(menuBgTexture);
	//sfTexture_destroy(quitTexture);
	//sfTexture_destroy(quitBoxTexture);
}

