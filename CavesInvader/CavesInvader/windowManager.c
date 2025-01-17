#include "windowManager.h"
#include "gamepad.h"
#include "viewManager.h"
#include "stateManager.h"
#include "pause.h"


sfTexture* allTextures;
sfSprite* allSprites;

float fullscreenTimer;

sfBool IsDone(Window* This)
{
	return This->isDone;
}

sfBool IsFullscreen(Window* This)
{
	return This->isFullscreen;
}

void BeginDraw(Window* This)
{
	sfRenderTexture_clear(This->renderTexture, sfBlack);
	//sfRenderWindow_clear(This->renderWindow, sfBlack);
}

void EndDraw(Window* This)
{
	sfRenderWindow_display(This->renderWindow);
}

Window* windowSetup(const char* _Title, sfVector2i _defaultVideoMode)
{
	Window* This = malloc(sizeof(Window));
	if (This == NULL)
	{
		printf("Allocation pour la window echoue");
	}
	This->windowTitle = malloc(strlen(_Title)*sizeof(char));
	if (This->windowTitle == NULL)
	{
		printf("Allocation pour Title de la window echoue");
	}

	strcpy(This->windowTitle, _Title);
	
	fullscreenTimer = 1.f;
	This->isFullscreen = sfFalse;
	This->isDone = sfFalse;
	This->videoMode = sfVideoMode_getDesktopMode();
	This->defaultVideoMode = _defaultVideoMode;
	
	windowCreate(This);

	// MainView
	sfFloatRect rectViewPort = { 0.0f, 0.0f, 1.0f, 1.0f };
	mainView = setupView(_defaultVideoMode, rectViewPort, This->videoMode);

	// Render Texture
	This->renderTexture = sfRenderTexture_create(_defaultVideoMode.x, _defaultVideoMode.y, sfFalse);
	//allTextures = sfTexture_create(_defaultVideoMode.x, _defaultVideoMode.y);
	allSprites = sfSprite_create();

	return This;
}

void windowDestroy(Window* This)
{
	sfRenderWindow_close(This->renderWindow);
}

void windowCreate(Window* This)
{
	sfWindowStyle style = (This->isFullscreen ? sfFullscreen : sfDefaultStyle);

	This->renderWindow = sfRenderWindow_create(This->videoMode, This->windowTitle, style, NULL);
	sfRenderWindow_setFramerateLimit(This->renderWindow, 120);

	sfImage* image;
	image = sfImage_createFromFile("../Ressources/icon32x32.png");
	if (image != NULL)
	{
		sfVector2u size = sfImage_getSize(image);
		sfRenderWindow_setIcon(This->renderWindow, size.x, size.y, sfImage_getPixelsPtr(image));
	}
	sfImage_destroy(image);

	if (This->isFullscreen) {
		sfRenderWindow_setMouseCursorVisible(This->renderWindow, sfFalse);
	}
	else {
		sfRenderWindow_setMouseCursorVisible(This->renderWindow, sfTrue);
	}


}

void ToggleFullscreen(Window* This)
{
	if (fullscreenTimer > 0.5f) {
		This->isFullscreen = !This->isFullscreen;
		windowDestroy(This);
		windowCreate(This);
		fullscreenTimer = 0.f;
	}

}

void windowInit(Window* This) // useless because stateInit() is called in changeState()
{
	stateInit(This);
}

void windowUpdate(Window* This)
{
	static sfBool first = sfFalse;
	if (!first) {
		stateInit(This);
		first = sfTrue;
	}


	// TO REMOVE
	if (middleClick(This->renderWindow)) This->isDone = sfTrue;

	restartClock();
	while (sfRenderWindow_pollEvent(This->renderWindow, &This->event))
	{
		if (This->event.type == sfEvtClosed)
			This->isDone = sfTrue;

		for (int i = 0; i < /*8*//*nbPlayer*/MAX_PLAYER; i++)
		{
			GamepadManager(i, This->event);
		}
		stateEventUpdate(This, &This->event);
	}

	fullscreenTimer += getUnscaledDeltaTime();

	if (sfKeyboard_isKeyPressed(sfKeyF11) && sfRenderWindow_hasFocus(This->renderWindow))
	{
		ToggleFullscreen(This);
	}
	if (sfKeyboard_isKeyPressed(sfKeyF10) && sfRenderWindow_hasFocus(This->renderWindow))
	{
		screenshot(This->renderWindow);
	}

	// MAINVIEW 
	sfView_setCenter(mainView->view, mainView->PosView);
	
	stateUpdate(This);

	if (IsDone(This)) {
		saveOptions(This);
	}
}

void windowDraw(Window* This)
{
	sfRenderWindow_setView(This->renderWindow, mainView->view);
	stateDisplay(This);

	sfRenderTexture_display(This->renderTexture);
	allTextures = sfRenderTexture_getTexture(This->renderTexture);
	sfSprite_setTexture(allSprites, allTextures, sfFalse);

	sfRenderWindow_clear(This->renderWindow, sfBlack);
	sfRenderWindow_drawSprite(This->renderWindow, allSprites, NULL);
}