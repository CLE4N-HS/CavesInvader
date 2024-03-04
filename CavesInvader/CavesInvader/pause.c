#include "pause.h"
#include "stateManager.h"
#include "fontManager.h"
#include "textureManager.h"
#include "viewManager.h"
#include "gamepad.h"
#include "dialogBox.h"
#include "quit.h"
#include "CustomMath.h"

//sfRectangleShape* rshape;
//
//sfText* txtPause;
//sfText* txtReturnGame;
//sfText* txtReturnMenu;
//sfText* txtSoundVolume;
//
//sfTexture* texCursor;
//sfSprite* spCursor;
//
//int pauseSelect;

sfRectangleShape* opaqueRectangle;

sfSprite* pauseSprite;

sfTexture* pauseTexture;
sfTexture* quitPauseBoxTexture;
sfTexture* fullscreenBoxTexture;
sfTexture* sliderTexture;
sfTexture* sliderBoxTexture;

float sliderSFXPosX;
float sliderMusicPosX;
sfBool allowedToSlide;


typedef enum PauseChoice {
	NOCHOICE_PAUSE,
	RESUME_PAUSE,
	SLIDERSFX_PAUSE,
	FULLSCREEN_PAUSE,
	SLIDERMUSIC_PAUSE,
	QUIT_PAUSE
}PauseChoice;

PauseChoice choicePause;

float escapePauseTimer;
float choicePauseTimer;
float sliderTimer;

PauseChoice changePauseChoice(sfKeyCode _key)
{
	if (_key == sfKeyUp) {
		switch (choicePause) {
		case RESUME_PAUSE: return QUIT_PAUSE;
		case SLIDERSFX_PAUSE: return RESUME_PAUSE;
		case FULLSCREEN_PAUSE: return RESUME_PAUSE;
		case SLIDERMUSIC_PAUSE: return SLIDERSFX_PAUSE;
		case QUIT_PAUSE: return SLIDERMUSIC_PAUSE;
		default: break;
		}
	}
	else if (_key == sfKeyDown) {
		switch (choicePause) {
		case RESUME_PAUSE: return SLIDERSFX_PAUSE;
		case SLIDERSFX_PAUSE: return SLIDERMUSIC_PAUSE;
		case FULLSCREEN_PAUSE: return QUIT_PAUSE;
		case SLIDERMUSIC_PAUSE: return QUIT_PAUSE;
		case QUIT_PAUSE: return RESUME_PAUSE;
		default: break;
		}
	}
	else if (_key == sfKeyLeft) {
		switch (choicePause) {
		case RESUME_PAUSE: return SLIDERSFX_PAUSE;
		case FULLSCREEN_PAUSE: return SLIDERSFX_PAUSE;
		case QUIT_PAUSE: return SLIDERMUSIC_PAUSE;
		default: break;
		}
	}
	else if (_key == sfKeyRight) {
		switch (choicePause) {
		case RESUME_PAUSE: return FULLSCREEN_PAUSE;
		case FULLSCREEN_PAUSE: return QUIT_PAUSE;
		case QUIT_PAUSE: return FULLSCREEN_PAUSE;
		default: break;
		}
	}

	return choicePause;
}

void resetPause()
{
	choicePause = RESUME_PAUSE;
	escapePauseTimer = 0.f;
	choicePauseTimer = 0.f;
	sliderTimer = 0.f;
	allowedToSlide = sfFalse;
}

void initPause(Window* _window)
{
	opaqueRectangle = sfRectangleShape_create();
	sfRectangleShape_setPosition(opaqueRectangle, VECTOR2F_NULL);
	sfRectangleShape_setOrigin(opaqueRectangle, VECTOR2F_NULL);
	sfRectangleShape_setFillColor(opaqueRectangle, color(0, 0, 0, 127));
	sfRectangleShape_setSize(opaqueRectangle, vector2f(1920.f, 1080.f));

	pauseSprite = sfSprite_create();

	pauseTexture = GetTexture("pause");
	quitPauseBoxTexture = GetTexture("quitPauseBox");
	fullscreenBoxTexture = GetTexture("fullscreenBox");
	sliderTexture = GetTexture("slider");
	sliderBoxTexture = GetTexture("sliderBox");

	sliderSFXPosX = 788.f;
	sliderMusicPosX = 788.f;

	resetPause();

	//rshape = sfRectangleShape_create();
	//sfRectangleShape_setSize(rshape, (sfVector2f) { (float)mainView->defaultVideoMode.x, (float)mainView->defaultVideoMode.y});
	//sfRectangleShape_setFillColor(rshape, (sfColor) {0,0,0,150});
	//txtPause = sfText_create();
	//txtReturnGame = sfText_create();
	//txtReturnMenu = sfText_create();
	//txtSoundVolume = sfText_create();
	//sfText_setFont(txtPause, getDefaultFont());
	//sfText_setColor(txtPause, sfRed);
	//sfText_setString(txtPause, "PAUSE");
	//sfText_setFont(txtReturnGame, getDefaultFont());
	//sfText_setCharacterSize(txtReturnGame, 20);
	//sfText_setColor(txtReturnGame, sfWhite);
	//sfText_setString(txtReturnGame, "Return to Game");
	//sfText_setFont(txtReturnMenu, getDefaultFont());
	//sfText_setCharacterSize(txtReturnMenu, 20);
	//sfText_setColor(txtReturnMenu, sfWhite);
	//sfText_setString(txtReturnMenu, "Exit to Menu");
	//sfText_setFont(txtSoundVolume, getDefaultFont());
	//sfText_setCharacterSize(txtSoundVolume, 20);
	//sfText_setColor(txtSoundVolume, sfWhite);
	//sfText_setString(txtSoundVolume, "Options");
	//
	//texCursor = sfTexture_createFromFile("../Ressources/Textures/cursor.png", NULL);
	//spCursor = sfSprite_create();
	//sfSprite_setTexture(spCursor, texCursor, sfTrue);
	//pauseSelect = 0;
}

void updatePause(Window* _window)
{
	float dt = getDeltaTime();
	escapePauseTimer += dt;
	choicePauseTimer += dt;
	sliderTimer += dt;

	// escape
	if (escapePauseTimer > 0.2f && (isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX) || isKeyboardOrControllerButtonPressed(sfKeyEscape, B_XBOX))) {
		togglePause();
		resetPause();
	}
	
	// movement
	if (choicePauseTimer > 0.3f)
	{
		if (isKeyboardOrControllerButtonMoved(sfKeyUp, STICKLY_XBOX, sfTrue, 50.f)) {
			choicePauseTimer = 0.f;
			allowedToSlide = sfFalse;
			choicePause = changePauseChoice(sfKeyUp);
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyDown, STICKLY_XBOX, sfFalse, 50.f)) {
			choicePauseTimer = 0.f;
			allowedToSlide = sfFalse;
			choicePause = changePauseChoice(sfKeyDown);
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)) {
			choicePauseTimer = 0.f;
			choicePause = changePauseChoice(sfKeyLeft);
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)) {
			choicePauseTimer = 0.f;
			choicePause = changePauseChoice(sfKeyRight);
		}
	}

	// interactions
	switch (choicePause)
	{
	case RESUME_PAUSE:
		if (escapePauseTimer > 0.2f && isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)) {
			togglePause();
			resetPause();
		}
		break;
	case SLIDERSFX_PAUSE:
		if (!allowedToSlide && !isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 10.f)) {
			allowedToSlide = sfTrue;
		}
		if (!allowedToSlide)
			break;

		if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)) {
			sliderSFXPosX -= dt * 100.f;
			sliderSFXPosX = MAX(sliderSFXPosX, 782.f);
			// TODO change volume
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)) {
			sliderSFXPosX += dt * 100.f;
			sliderSFXPosX = MIN(sliderSFXPosX, 1049.f);
			// TODO change volume	
		}
		break;
	case FULLSCREEN_PAUSE:
		if (escapePauseTimer > 0.2f && isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)) {
			escapePauseTimer = 0.f;
			forceReleasedButton(A_XBOX);
			ToggleFullscreen(_window);
		}
		break;
	case SLIDERMUSIC_PAUSE:
		if (!allowedToSlide && !isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 10.f)) {
			allowedToSlide = sfTrue;
		}
		if (!allowedToSlide)
			break;

		if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)) {
			sliderMusicPosX -= dt * 100.f;
			sliderMusicPosX = MAX(sliderMusicPosX, 788.f);
			// TODO change volume
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)) {
			sliderMusicPosX += dt * 100.f;
			sliderMusicPosX = MIN(sliderMusicPosX, 1055.f);
			// TODO change volume	
		}
		break;
	case QUIT_PAUSE:
		if (escapePauseTimer > 0.2f && isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)) {
			togglePause();
			resetPause();
			forceReleasedButton(A_XBOX);
			toggleQuit();
		}
		break;
	default:
		break;
	}


	//static float timer = 0.0f;
	//timer += getDeltaTime();
	//
	//for (int i = 0; i < /*8*/nbPlayer; i++)
	//{
	//	if (Gamepad_isButtonPressed(i, OPTION) && timer > 0.2f)
	//	{
	//		togglePause();
	//		timer = 0.0f;
	//	}
	//
	//
	//	if (Gamepad_isJoystickMoved(i, CROSSY) > 0 && timer > 0.2f)
	//	{
	//		pauseSelect--;
	//		if (pauseSelect < 0)
	//			pauseSelect = 2;
	//		timer = 0.0f;
	//	}
	//	if (Gamepad_isJoystickMoved(i, CROSSY) < 0 && timer > 0.2f)
	//	{
	//		pauseSelect++;
	//		if (pauseSelect > 2)
	//			pauseSelect = 0;
	//		timer = 0.0f;
	//	}
	//
	//	if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
	//	{
	//		if (pauseSelect == 0)
	//		{
	//			togglePause();
	//		}
	//		else if (pauseSelect == 2)
	//		{
	//			//CreateDialogBox(ALERT, "Voulez vous retourner au menu?\n", 1);
	//			toggleQuit();
	//			resetQuit();
	//		}
	//
	//		timer = 0.0f;
	//	}
	//}
	//if (sfKeyboard_isKeyPressed(sfKeyEscape) && timer > 0.2f)
	//{
	//	togglePause();
	//	timer = 0.0f;
	//}
	//if (sfKeyboard_isKeyPressed(sfKeyDown) && timer > 0.2f)
	//{
	//	pauseSelect++;
	//	if (pauseSelect > 2)
	//		pauseSelect = 0;
	//	timer = 0.0f;
	//}
	//if (sfKeyboard_isKeyPressed(sfKeyUp) && timer > 0.2f)
	//{
	//	pauseSelect--;
	//	if (pauseSelect < 0)
	//		pauseSelect = 2;
	//	timer = 0.0f;
	//}
	//if (sfKeyboard_isKeyPressed(sfKeyEnter) && timer > 0.2f)
	//{
	//	
	//	if (pauseSelect == 0)
	//	{
	//		togglePause();
	//	}
	//	else if (pauseSelect == 1)
	//	{
	//		toggleOptions();
	//	}
	//	else if (pauseSelect == 2)
	//	{
	//		CreateDialogBox(ALERT, "Voulez vous retourner au menu?\n", 1);
	//	}
	//
	//	timer = 0.0f;
	//}
	////remove
	//if (sfKeyboard_isKeyPressed(sfKeyA) && timer > 0.2f) {
	//	CreateDialogBox(INFO, "SALUT", 1);
	//	timer = 0.f;
	//}
	//
	//if (sfKeyboard_isKeyPressed(sfKeyP) && timer > 0.2f) {
	//	sfText_setFont(txtReturnGame, GetFont("tmpFont"));
	//	timer = 0.f;
	//}
	//
	////
	//
	//sfRectangleShape_setPosition(rshape, (sfVector2f) { mainView->PosView.x - mainView->defaultVideoMode.x/2, mainView->PosView.y - mainView->defaultVideoMode.y / 2});
	//
	//sfText_setPosition(txtPause, vector2f(GetViewPosition(mainView).x, GetViewPosition(mainView).y - 250.0f));
	//sfText_setPosition(txtReturnGame, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 200.0f));
	//sfText_setPosition(txtReturnMenu, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 100.0f));
	//sfText_setPosition(txtSoundVolume, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 150.0f));
	//
	//if (pauseSelect == 0)
	//{
	//	sfText_setPosition(txtReturnGame, vector2f(GetViewPosition(mainView).x - 80.0f, GetViewPosition(mainView).y - 200.0f));
	//	sfSprite_setPosition(spCursor, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 195.0f));
	//}
	//else if(pauseSelect == 1)
	//{
	//	sfText_setPosition(txtSoundVolume, vector2f(GetViewPosition(mainView).x - 80.0f, GetViewPosition(mainView).y - 150.0f));
	//	sfSprite_setPosition(spCursor, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 145.0f));
	//}
	//else if (pauseSelect == 2)
	//{
	//	sfText_setPosition(txtReturnMenu, vector2f(GetViewPosition(mainView).x - 80.0f, GetViewPosition(mainView).y - 100.0f));
	//	sfSprite_setPosition(spCursor, vector2f(GetViewPosition(mainView).x - 100.0f, GetViewPosition(mainView).y - 95.0f));
	//}
}

void displayPause(Window* _window)
{
	// opacity
	sfRenderTexture_drawRectangleShape(_window->renderTexture, opaqueRectangle, NULL);

	// main
	sfSprite_setTexture(pauseSprite, pauseTexture, sfTrue);
	sfSprite_setPosition(pauseSprite, vector2f(491.f, 90.f));
	sfSprite_setOrigin(pauseSprite, VECTOR2F_NULL);
	sfRenderTexture_drawSprite(_window->renderTexture, pauseSprite, NULL);

	// sliders
	sfSprite_setTexture(pauseSprite, sliderTexture, sfTrue);
	sfSprite_setOrigin(pauseSprite, vector2f(10.f, 16.f));
	for (int i = 0; i < 2; i++)
	{
		switch (i) {
		case 0:
			sfSprite_setPosition(pauseSprite, vector2f(sliderSFXPosX, 422.f));
			break;
		case 1:
			sfSprite_setPosition(pauseSprite, vector2f(sliderMusicPosX, 664.f));
			break;
		default:
			break;
		}

		sfRenderTexture_drawSprite(_window->renderTexture, pauseSprite, NULL);
	}

	// boxes
	sfSprite_setOrigin(pauseSprite, VECTOR2F_NULL);
	switch (choicePause)
	{
	case RESUME_PAUSE:
		sfSprite_setTexture(pauseSprite, quitPauseBoxTexture, sfTrue);
		sfSprite_setPosition(pauseSprite, vector2f(740.f, 200.f));
		break;
	case SLIDERSFX_PAUSE:
		sfSprite_setTexture(pauseSprite, sliderBoxTexture, sfTrue);
		sfSprite_setPosition(pauseSprite, vector2f(747.f, 394.f));
		break;
	case FULLSCREEN_PAUSE:
		sfSprite_setTexture(pauseSprite, fullscreenBoxTexture, sfTrue);
		sfSprite_setPosition(pauseSprite, vector2f(1191.f, 468.f));
		break;
	case SLIDERMUSIC_PAUSE:
		sfSprite_setTexture(pauseSprite, sliderBoxTexture, sfTrue);
		sfSprite_setPosition(pauseSprite, vector2f(747.f, 636.f));
		break;
	case QUIT_PAUSE:
		sfSprite_setTexture(pauseSprite, quitPauseBoxTexture, sfTrue);
		sfSprite_setPosition(pauseSprite, vector2f(740.f, 808.f));
		break;
	default:
		break;
	}
	sfRenderTexture_drawSprite(_window->renderTexture, pauseSprite, NULL);

	//sfRenderTexture_setView(_window->renderTexture, mainView->view);
	//sfRenderTexture_drawRectangleShape(_window->renderTexture, rshape, NULL);
	//
	//sfRenderTexture_drawSprite(_window->renderTexture, spCursor, NULL);
	//
	//sfRenderTexture_drawText(_window->renderTexture, txtPause, NULL);
	//sfRenderTexture_drawText(_window->renderTexture, txtReturnGame, NULL);
	//sfRenderTexture_drawText(_window->renderTexture, txtReturnMenu, NULL);
	//sfRenderTexture_drawText(_window->renderTexture, txtSoundVolume, NULL);
}

void deinitPause()
{
	sfSprite_destroy(pauseSprite);

	//sfRectangleShape_destroy(rshape);
	//sfText_destroy(txtPause);
	//sfText_destroy(txtReturnGame);
	//sfText_destroy(txtReturnMenu);
	//sfText_destroy(txtSoundVolume);
	//sfSprite_destroy(spCursor);
	//sfTexture_destroy(texCursor);
}