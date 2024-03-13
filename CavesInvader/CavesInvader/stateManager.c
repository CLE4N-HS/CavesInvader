#include "stateManager.h"
#include "ressourcesManager.h"
#include "textureManager.h"
#include "soundManager.h"
#include "fontManager.h"
#include "pause.h"
#include "menu.h"
#include "game.h"
#include "dialogBox.h"
#include "gamepad.h"
#include "options.h"
#include "viewManager.h"
#include "particlesSystemManager.h"
#include "quit.h"
#include "leaderboard.h"
#include "intro.h"

sfTexture* bgTexture;
sfTexture* loadingTexture;
float angle;

void stateInit(Window* _window)
{
	static sfBool firstload = sfFalse;
	if (!firstload)
	{
		OnLoad("../Ressources/ressources.load");
		Texture_Onload(ALL);
		Sound_Onload(ALL);
		Font_Onload(ALL);
		spLoading = sfSprite_create();
		bgTexture = GetTexture("menuBg");
		loadingTexture = GetTexture("loading2");
		angle = 0.f;
		//sfSprite_setTexture(spLoading, GetTexture("loading"), sfTrue);
		//sfSprite_setTexture(spLoading, GetTexture("loading2"), sfTrue);
		//sfIntRect AnimRect = { 0, 0, 128, 128 };
		//sfSprite_setOrigin(spLoading, vector2f(64.0f, 64.0f));
		sfSprite_setOrigin(spLoading, vector2f(98.0f, 77.0f));
		sfSprite_setPosition(spLoading, vector2f(mainView->PosView.x, mainView->PosView.y + 300.f));
		//sfSprite_setTextureRect(spLoading, AnimRect);

		SFXVolume = 0.0f; // change to 50.f
		musicVolume = 0.0f; // change to 50.f


		timeScale = 1.f;

		PlayASound("test", sfTrue);
		//nbPlayer = NbConnectedControllers();
		GamepadDetection();
		initQuit(_window);
		loadOptions(_window);
		loadLeaderboard();
		//sortLeaderboard();
		

		firstload = sfTrue;

		w.state = sfTrue;
	}

	if (!onePass)
	{
		isDialogBox = sfFalse;
		onePass = sfTrue;
		if (state == INTRO)
		{
			initIntro(_window);
		}
		if (state == MENU)
		{
			initMenu(_window);
			initOptions(_window);
			//initQuit(_window);
			// TODO ADD deinit quit
		}
		if (state == GAME)
		{
			w.mutex = sfMutex_create();
			w.state = sfFalse;
			loadingThread = sfThread_create(&initGame, _window);
			sfThread_launch(loadingThread);
			//initPause(_window);
		}
		if (state == END)
		{
			
		}
	}
}

void stateEventUpdate(Window* _window, sfEvent* _event)
{
	if (w.state)
	{
			if (state == INTRO)
			{

			}
			else if (state == MENU)
			{
				
			}
			else if (state == GAME)
			{
				
			}
			else if (state == END)
			{

			}
		
	}
}

void stateUpdate(Window* _window)
{
	if (w.state)
	{
		if (!isDialogBox)
		{
			if (state == INTRO)
			{
				updateIntro(_window);
			}
			else if (state == MENU)
			{
				if (isOption)
					updateOptions(_window);
				else if (isQuit)
					updateQuit(_window);
				else
					updateMenu(_window);
			}
			else if (state == GAME)
			{
				//if (isOption)
				//	updateOptions(_window);
				if (isPaused)
					updatePause(_window);
				else if (isQuit)
					updateQuit(_window);
				else
					updateGame(_window);
			}
			else if (state == END)
			{

			}
		}
		else
		{
			dialogBoxUpdate(_window);
		}
	}
	else if (!w.state)
	{
		//static sfIntRect AnimRect = { 0, 0, 128, 128 };
		//static int frame = 0;
		static float timer = 0.0f;
		timer += getDeltaTime();

		sfMutex_lock(w.mutex);
		//sfSprite_setOrigin(spLoading, vector2f(64.0f, 64.0f));
		//sfSprite_setPosition(spLoading, vector2f(mainView->PosView.x, mainView->PosView.y));
		//sfSprite_setTextureRect(spLoading, AnimRect);
		if (timer > 0.01f)
		{
			angle += 1.f;
			//frame++;
			//if (frame > 8)
			//	frame = 0;
			//AnimRect.left = frame * 128;
			
			timer = 0.0f;
		}
		sfMutex_unlock(w.mutex);
	}
}

void stateDisplay(Window* _window)
{
	if (w.state)
	{
		if (state == INTRO)
		{
			displayIntro(_window);
		}
		if (state == MENU)
		{
			displayMenu(_window);
			if (isOption)
			{
				displayOptions(_window);
			}
			if (isQuit)
			{
				displayQuit(_window);
			}
			if (isDialogBox)
			{
				dialogBoxDisplay(_window);
			}
		}
		if (state == GAME)
		{
			displayGame(_window);
			if (isPaused)
				displayPause(_window);
			else if (isQuit)
				displayQuit(_window);
			
			if (isDialogBox)
			{
				dialogBoxDisplay(_window);
			}
		}
		if (state == END)
		{

		}
	}
	else if (!w.state)
	{
		sfMutex_lock(w.mutex);
		
		sfSprite_setTexture(spLoading, bgTexture, sfTrue);
		sfSprite_setOrigin(spLoading, VECTOR2F_NULL);
		sfSprite_setPosition(spLoading, VECTOR2F_NULL);
		sfSprite_setRotation(spLoading, 0.f);
		sfRenderTexture_drawSprite(_window->renderTexture, spLoading, NULL);
		
		sfSprite_setTexture(spLoading, loadingTexture, sfTrue);
		sfSprite_setOrigin(spLoading, vector2f(98.f, 77.f));
		sfSprite_setPosition(spLoading, vector2f(960.f, 840.f));
		sfSprite_setRotation(spLoading, angle);
		sfRenderTexture_drawSprite(_window->renderTexture, spLoading, NULL);

		sfMutex_unlock(w.mutex);
	}
}

void stateDeinit(Window* _window)
{
	RemoveAllTextureButALL();
	RemoveAllSoundsButALL();
	RemoveAllFontsButALL();
	if (state == INTRO)
	{

	}
	if (state == MENU)
	{
		deinitMenu();
		//deinitOptions();
	}
	if (state == GAME)
	{
		deinitGame();
		//deinitOptions();
		saveLeaderboard();
		loadLeaderboard();
		
	}
	if (state == END)
	{

	}
}


void changeState(Window* _window, State _state)
{
	stateDeinit(_window);
	state = _state;
	onePass = sfFalse;
	stateInit(_window);
}

State getState()
{
	return state;
}

void changeMenuState(MenuState _menuState)
{
	menuState = _menuState;
	if (menuState == MENU_MAIN)
		resetMenu();
}

MenuState getMenuState()
{
	return menuState;
}

void togglePause()
{
	isPaused = !isPaused;
}

void toggleOptions()
{
	isOption = !isOption;
}

void toggleQuit()
{
	isQuit = !isQuit;
}