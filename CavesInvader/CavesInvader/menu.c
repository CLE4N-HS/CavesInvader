#include "menu.h"
#include "textureManager.h"
#include "gamepad.h"
#include "viewManager.h"
#include "dialogBox.h"
#include "particlesSystemManager.h"
#include "nameChoice.h"
#include "leaderboard.h"


typedef enum MenuChoice {
	NOCHOICE_MENU = -1,
	PLAY_MENU,
	LEADERBOARD_MENU,
	COMMANDS_MENU,
	CREDITS_MENU,
	OPTIONS_MENU,
	QUIT_MENU
}MenuChoice;
MenuChoice choiceMenu;


sfSprite* menuSprite;

sfTexture* menuBgTexture;
sfTexture* menuBoxTexture;
sfTexture* menuHoverBoxTexture;
sfTexture* menuMainTexture;

float timer;
float choiceTimer;


MenuChoice changeChoiceMenu(sfKeyCode _key)
{
	if (_key == sfKeyUp) {
		switch (choiceMenu)
		{
		case NOCHOICE_MENU: return PLAY_MENU;
		case LEADERBOARD_MENU: return PLAY_MENU;
		case COMMANDS_MENU: return PLAY_MENU;
		case OPTIONS_MENU: return PLAY_MENU;
		case CREDITS_MENU: return LEADERBOARD_MENU;
		case QUIT_MENU: return COMMANDS_MENU;
		default: break;
		}
	}
	else if (_key == sfKeyDown) {
		switch (choiceMenu)
		{
		case NOCHOICE_MENU: return PLAY_MENU;
		case PLAY_MENU: return OPTIONS_MENU;
		case LEADERBOARD_MENU: return OPTIONS_MENU;
		case COMMANDS_MENU: return OPTIONS_MENU;
		case OPTIONS_MENU: return CREDITS_MENU;
		default: break;
		}
	}
	else if (_key == sfKeyLeft) {
		switch (choiceMenu)
		{
		case NOCHOICE_MENU: return PLAY_MENU;
		case PLAY_MENU: return LEADERBOARD_MENU;
		case LEADERBOARD_MENU: return CREDITS_MENU;
		case COMMANDS_MENU: return LEADERBOARD_MENU;
		case OPTIONS_MENU: return CREDITS_MENU;
		case QUIT_MENU: return OPTIONS_MENU;
		default: break;
		}
	}
	else if (_key == sfKeyRight) {
		switch (choiceMenu)
		{
		case NOCHOICE_MENU: return PLAY_MENU;
		case PLAY_MENU: return COMMANDS_MENU;
		case LEADERBOARD_MENU: return COMMANDS_MENU;
		case COMMANDS_MENU: return QUIT_MENU;
		case OPTIONS_MENU: return QUIT_MENU;
		case CREDITS_MENU: return OPTIONS_MENU;
		default: break;
		}
	}
	else if (_key == sfKeyEnter && choiceMenu == NOCHOICE_MENU)
		return PLAY_MENU;

	return choiceMenu;
}

void initMenu(Window* _window)
{
	Texture_Onload(MENU);

	menuSprite = sfSprite_create();

	menuBgTexture = GetTexture("menuBg");
	menuBoxTexture = GetTexture("menuBox");
	menuHoverBoxTexture = GetTexture("menuHoverBox");
	menuMainTexture = GetTexture("menuMain");


	SetViewPosition(mainView, vector2f(mainView->defaultVideoMode.x / 2.0f, mainView->defaultVideoMode.y / 2.0f));

	GamepadDetection();

	choiceMenu = NOCHOICE_MENU;

	initNameChoice(_window);
	initLeaderboard(_window);

	resetMenu();
}

void updateMenu(Window* _window)
{
	MenuState tmpMenuState = getMenuState();

	if (tmpMenuState == MENU_CHOICENAME) {
		updateNameChoice(_window);
	}
	else if (tmpMenuState == MENU_LEADERBOARD) {
		updateLeaderboard(_window);
	}
	else {
		timer += getUnscaledDeltaTime();
		choiceTimer += getUnscaledDeltaTime();

		// buttons movement
		if (timer > 0.3f)
		{
			if (isKeyboardOrControllerButtonMoved(sfKeyUp, STICKLY_XBOX, sfTrue, 30.f)) {
				choiceMenu = changeChoiceMenu(sfKeyUp);
				timer = 0.f;
			}
			else if (isKeyboardOrControllerButtonMoved(sfKeyDown, STICKLY_XBOX, sfFalse, 30.f)) {
				choiceMenu = changeChoiceMenu(sfKeyDown);
				timer = 0.f;
			}
			else if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 30.f)) {
				choiceMenu = changeChoiceMenu(sfKeyLeft);
				timer = 0.f;
			}
			else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 30.f)) {
				choiceMenu = changeChoiceMenu(sfKeyRight);
				timer = 0.f;
			}
			else if (isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX) || isKeyboardOrControllerButtonPressed(sfKeyEscape, B_XBOX)) {
				timer = 0.f;
				forceReleasedButton(START_XBOX);
				forceReleasedButton(B_XBOX);
				toggleQuit();
			}

		}

		// Menu Choice
		if (isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX) && choiceTimer > 0.2f) {
			switch (choiceMenu)
			{
			case NOCHOICE_MENU:
				choiceTimer = 0.f;
				choiceMenu = changeChoiceMenu(sfKeyEnter);
				forceReleasedButton(A_XBOX);
				break;
			case PLAY_MENU:
				choiceTimer = 0.f;
				changeMenuState(MENU_CHOICENAME);
				//changeState(_window, GAME);
				break;
			case LEADERBOARD_MENU:
				choiceTimer = 0.f;
				changeMenuState(MENU_LEADERBOARD);
				break;
			case QUIT_MENU:
				choiceTimer = 0.f;
				toggleQuit();
				break;
			default:
				break;
			}
		}
	}

	


	//for (int i = 0; i < /*8*/nbPlayer; i++)
	//{
	//	if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
	//	{
	//		switch (menuSelection)
	//		{
	//		case 0:
	//			changeState(_window, GAME);
	//			break;
	//		case 1:
	//			changeState(_window, EDITOR);
	//			break;
	//		case 2:
	//			toggleOptions();
	//			break;
	//		case 3:
	//			_window->isDone = sfTrue;
	//			break;
	//		default:
	//			break;
	//		}
	//		timer = 0.0f;
	//	}
	//	if (Gamepad_isJoystickMoved(i, CROSSY) > 0 && timer > 0.2f)
	//	{
	//		menuSelection--;
	//		if (menuSelection < 0)
	//			menuSelection = 3;
	//		timer = 0.0f;
	//	}
	//	if (Gamepad_isJoystickMoved(i, CROSSY) < 0 && timer > 0.2f)
	//	{
	//		menuSelection++;
	//		if (menuSelection > 3)
	//			menuSelection = 0;
	//		timer = 0.0f;
	//	}
	//	if (Gamepad_isJoystickMoved(i, STICKLY) < 0 && timer > 0.5f)
	//	{
	//		menuSelection--;
	//		if (menuSelection < 0)
	//			menuSelection = 3;
	//		timer = 0.0f;
	//	}
	//	if (Gamepad_isJoystickMoved(i, STICKLY) > 0 && timer > 0.5f)
	//	{
	//		menuSelection++;
	//		if (menuSelection > 3)
	//			menuSelection = 0;
	//		timer = 0.0f;
	//	}
	//}
	//
	//if (sfKeyboard_isKeyPressed(sfKeyUp) && timer > 0.2f)
	//{
	//	menuSelection--;
	//	if (menuSelection < 0)
	//		menuSelection = 3;
	//	timer = 0.0f;
	//}
	//if (sfKeyboard_isKeyPressed(sfKeyDown) && timer > 0.2f)
	//{
	//	menuSelection++;
	//	if (menuSelection > 3)
	//		menuSelection = 0;
	//	timer = 0.0f;
	//}
	//if (sfKeyboard_isKeyPressed(sfKeyEnter) && timer > 0.2f)
	//{
	//	switch (menuSelection)
	//	{
	//	case 0:
	//		changeState(_window, GAME);
	//		break;
	//	case 1:
	//		//changeState(_window, EDITOR);
	//		break;
	//	case 2:
	//		toggleOptions();
	//		break;
	//	case 3:
	//		//_window->isDone = sfTrue;
	//		CreateDialogBox(ALERT, "Do you really want to quit ?", QUIT_TO_DESKTOP_DB);
	//		break;
	//	default:
	//		break;
	//	}
	//	timer = 0.0f;	
	//}

}

void displayMenu(Window* _window)
{
	// bg
	sfSprite_setTexture(menuSprite, menuBgTexture, sfTrue);
	sfSprite_setPosition(menuSprite, VECTOR2F_NULL);
	sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);

	MenuState tmpMenuState = getMenuState();

	if (tmpMenuState == MENU_CHOICENAME) {
		displayNameChoice(_window);
	}
	else if (tmpMenuState == MENU_LEADERBOARD) {
		displayLeaderboard(_window);
	}
	else {
		// Boxes
		for (int i = 0; i < 6; i++)
		{
			if (choiceMenu != PLAY_MENU) {
				sfSprite_setTexture(menuSprite, menuBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(797.f, 666.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			else {
				sfSprite_setTexture(menuSprite, menuHoverBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(785.f, 663.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			if (choiceMenu != LEADERBOARD_MENU) {
				sfSprite_setTexture(menuSprite, menuBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(549.f, 779.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			else {
				sfSprite_setTexture(menuSprite, menuHoverBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(537.f, 776.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			if (choiceMenu != COMMANDS_MENU) {
				sfSprite_setTexture(menuSprite, menuBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(1068.f, 779.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			else {
				sfSprite_setTexture(menuSprite, menuHoverBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(1056.f, 776.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			if (choiceMenu != OPTIONS_MENU) {
				sfSprite_setTexture(menuSprite, menuBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(797.f, 894.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			else {
				sfSprite_setTexture(menuSprite, menuHoverBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(785.f, 891.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			if (choiceMenu != CREDITS_MENU) {
				sfSprite_setTexture(menuSprite, menuBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(80.f, 944.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			else {
				sfSprite_setTexture(menuSprite, menuHoverBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(68.f, 941.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			if (choiceMenu != QUIT_MENU) {
				sfSprite_setTexture(menuSprite, menuBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(1523.f, 944.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
			else {
				sfSprite_setTexture(menuSprite, menuHoverBoxTexture, sfTrue);
				sfSprite_setPosition(menuSprite, vector2f(1511.f, 941.f));
				sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
			}
		}

		// main
		sfSprite_setTexture(menuSprite, menuMainTexture, sfTrue);
		sfSprite_setPosition(menuSprite, vector2f(158.f, 0.f));
		sfRenderTexture_drawSprite(_window->renderTexture, menuSprite, NULL);
	}


}

void resetMenu()
{
	timer = 0.f;
	choiceTimer = 0.f;

}

void deinitMenu()
{
	sfSprite_destroy(menuSprite);

}