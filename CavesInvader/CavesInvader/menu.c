#include "menu.h"
#include "textureManager.h"
#include "gamepad.h"
#include "viewManager.h"
#include "dialogBox.h"
#include "particlesSystemManager.h"

sfFont* venture3D;

sfText* Play;
sfText* Editor;
sfText* Options;
sfText* Quit;

sfTexture* texture;
sfSprite* sprite;

int menuSelection;
MenuChoice choiceMenu;

MenuChoice changeChoiceMenu(sfKeyCode _key)
{
	if (_key == sfKeyUp) {
		switch (choiceMenu)
		{
		case NOCHOICE_MENU: return PLAY_MENU;
		case LEADERBOARD_MENU: return PLAY_MENU;
		case COMMANDS_MENU: return PLAY_MENU;
		case OPTIONS_MENU: return LEADERBOARD_MENU;
		case CREDITS_MENU: return LEADERBOARD_MENU;
		case QUIT_MENU: return COMMANDS_MENU;
		default: break;
		}
	}
	else if (_key == sfKeyDown) {
		switch (choiceMenu)
		{
		case NOCHOICE_MENU: return PLAY_MENU;
		case PLAY_MENU: return LEADERBOARD_MENU;
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

	venture3D = sfFont_createFromFile("../Ressources/Fonts/3Dventure.ttf");
	sprite = sfSprite_create();

	SetViewPosition(mainView, vector2f(mainView->defaultVideoMode.x / 2.0f, mainView->defaultVideoMode.y / 2.0f));

	Play = sfText_create();
	Editor = sfText_create();
	Options = sfText_create();
	Quit = sfText_create();
	sfText_setFont(Play, venture3D);
	sfText_setFont(Editor, venture3D);
	sfText_setFont(Options, venture3D);
	sfText_setFont(Quit, venture3D);
	sfText_setString(Play, "Play");
	sfText_setString(Editor, "Editor");
	sfText_setString(Options, "Options");
	sfText_setString(Quit, "Quit");
	sfText_setCharacterSize(Play, 72);
	sfText_setCharacterSize(Editor, 72);
	sfText_setCharacterSize(Options, 72);
	sfText_setCharacterSize(Quit, 72);
	sfText_setPosition(Play, vector2f(mainView->PosView.x + 100.0f ,mainView->PosView.y - 100.0f));
	sfText_setPosition(Editor, vector2f(mainView->PosView.x + 200.0f, mainView->PosView.y - 0.0f));
	sfText_setPosition(Options, vector2f(mainView->PosView.x + 300.0f, mainView->PosView.y + 100.0f));
	sfText_setPosition(Quit, vector2f(mainView->PosView.x + 200.0f, mainView->PosView.y + 200.0f));

	GamepadDetection();
	menuSelection = 0;
	choiceMenu = NOCHOICE_MENU;
}

void updateMenu(Window* _window)
{
	static float timer = 0.0f;
	timer += getUnscaledDeltaTime();
	static float choiceTimer = 0.f;
	choiceTimer += getUnscaledDeltaTime();

	//// to remove
	//sfVector2f mousePos = getfMousePos(_window->renderWindow);
	//if ((sfKeyboard_isKeyPressed(sfKeyE) || lClick(_window->renderWindow)) && timer > 0.01f) {
	//	CreateParticles(mousePos, vector2f(20.f, 20.f), vector2f(10.f, 10.f), vector2f(0.5f, 0.5f), 0.f, 360.f, 1.f, 0.f, 100.f, 1000.f, 15.f, color(0, 255, 255, 255), color(255, 0, 255, 0), 1.f, 3.f, 10, "yes", IntRect(0.f, 0.f, 0.f, 0.f), NULL, 1.f, 3.f, 10.f);
	//	timer = 0.f;
	//}
	//if (rClick(_window->renderWindow) && timer > 0.01f) {
	//	CreateParticles(mousePos, vector2f(1.f, 1.f), vector2f(0.0f, 0.0f), vector2f(400.f, 300.f), -45.f, 45.f, 1000.f, 5.f, 100.f, 1000.f, 20.0f, color(0, 255, 255, 255), color(255, 0, 255, 0), 5.f, 10.f, 1, "bg", IntRect(100.f, 100.f, 400.f, 300.f), NULL, 0.f, 1.f, 2.f);
	//	timer = -0.3f;
	//}
	if (middleClick(_window->renderWindow)) _window->isDone = sfTrue;
	////

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
		else if (isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX)) {
			timer = 0.f;
			toggleQuit();
		}
		// TODO ADD ECHAP = QUIT

	}

	// Menu Choice
	if (isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX) && choiceTimer > 0.2f) {
		switch (choiceMenu)
		{
		case NOCHOICE_MENU:
			choiceMenu = changeChoiceMenu(sfKeyEnter);
			timer = 0.f;
			choiceTimer = -0.1f;
			break;
		case PLAY_MENU:
			timer = 0.f;
			changeState(_window, GAME);
			break;
		case QUIT_MENU:
			timer = 0.f;
			toggleQuit();
			break;
		default:
			break;
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
	sfSprite_setTexture(sprite, GetTexture("menuBg"), sfTrue);
	sfSprite_setPosition(sprite, VECTOR2F_NULL);
	sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);

	// Boxes
	sfTexture* boxTexture = GetTexture("menuBox");
	sfTexture* hoverBoxTexture = GetTexture("menuHoverBox");
	for (int i = 0; i < 6; i++)
	{
		if (choiceMenu != PLAY_MENU) {
			sfSprite_setTexture(sprite, boxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(797.f, 666.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		else {
			sfSprite_setTexture(sprite, hoverBoxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(785.f, 663.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		if (choiceMenu != LEADERBOARD_MENU) {
			sfSprite_setTexture(sprite, boxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(549.f, 779.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		else {
			sfSprite_setTexture(sprite, hoverBoxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(537.f, 776.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		if (choiceMenu != COMMANDS_MENU) {
			sfSprite_setTexture(sprite, boxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(1068.f, 779.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		else {
			sfSprite_setTexture(sprite, hoverBoxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(1056.f, 776.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		if (choiceMenu != OPTIONS_MENU) {
			sfSprite_setTexture(sprite, boxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(797.f, 894.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		else {
			sfSprite_setTexture(sprite, hoverBoxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(785.f, 891.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		if (choiceMenu != CREDITS_MENU) {
			sfSprite_setTexture(sprite, boxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(80.f, 944.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		else {
			sfSprite_setTexture(sprite, hoverBoxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(68.f, 941.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		if (choiceMenu != QUIT_MENU) {
			sfSprite_setTexture(sprite, boxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(1523.f, 944.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
		else {
			sfSprite_setTexture(sprite, hoverBoxTexture, sfTrue);
			sfSprite_setPosition(sprite, vector2f(1511.f, 941.f));
			sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);
		}
	}

	// main
	sfSprite_setTexture(sprite, GetTexture("menuMain"), sfTrue);
	sfSprite_setPosition(sprite, vector2f(158.f, 0.f));
	sfRenderTexture_drawSprite(_window->renderTexture, sprite, NULL);

	//sfRenderTexture_drawText(_window->renderTexture, Play, NULL);
	//sfRenderTexture_drawText(_window->renderTexture, Editor, NULL);
	//sfRenderTexture_drawText(_window->renderTexture, Options, NULL);
	//sfRenderTexture_drawText(_window->renderTexture, Quit, NULL);
}

void deinitMenu()
{
	
	sfFont_destroy(venture3D);
	RemoveAllTextureButALL();
	sfText_destroy(Play);
	sfText_destroy(Editor);
	sfText_destroy(Options);
	sfText_destroy(Quit);
}