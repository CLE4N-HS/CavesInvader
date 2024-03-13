#include "pause.h"
#include "stateManager.h"
#include "fontManager.h"
#include "textureManager.h"
#include "viewManager.h"
#include "gamepad.h"
#include "dialogBox.h"
#include "quit.h"
#include "CustomMath.h"
#include "soundManager.h"

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

//sfBool firstLaunch = sfTrue;

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
	escapePauseTimer = 0.f;
	choicePauseTimer = 0.f;
	sliderTimer = 0.f;
	allowedToSlide = sfFalse;

	setSliderPos(&sliderSFXPosX, SFXVolume, 782.f, 1049.f);
	setSliderPos(&sliderMusicPosX, musicVolume, 788.f, 1055.f);
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
	choicePause = RESUME_PAUSE;

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
			choicePause = RESUME_PAUSE;
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
			ChangeVolume(SOUNDFX, getSliderValue(sliderSFXPosX, 782.f, 1049.f));
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)) {
			sliderSFXPosX += dt * 100.f;
			sliderSFXPosX = MIN(sliderSFXPosX, 1049.f);
			ChangeVolume(SOUNDFX, getSliderValue(sliderSFXPosX, 782.f, 1049.f));
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
			ChangeVolume(MUSIC, getSliderValue(sliderMusicPosX, 788.f, 1055.f));
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)) {
			sliderMusicPosX += dt * 100.f;
			sliderMusicPosX = MIN(sliderMusicPosX, 1055.f);
			ChangeVolume(MUSIC, getSliderValue(sliderMusicPosX, 788.f, 1055.f));
		}
		break;
	case QUIT_PAUSE:
		if (escapePauseTimer > 0.2f && isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)) {
			togglePause();
			resetPause();
			
			forceReleasedButton(A_XBOX);
			toggleQuit();
			resetQuit();
		}
		break;
	default:
		break;
	}

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
}

void deinitPause()
{
	sfSprite_destroy(pauseSprite);
	sfRectangleShape_destroy(opaqueRectangle);
}

//void saveOptions(Window* _window)
//{
//	config.isFullscreen = IsFullscreen(_window);
//	//SFXVolume = config.sfxVolume;
//	//musicVolume = config.musicVolume;
//
//	config.sfxVolume = SFXVolume;
//	config.musicVolume = musicVolume;
//
//	FILE* file;
//	file = fopen(FILE_PATH"config.cfg", "wb");
//	fwrite(&config, sizeof(struct Config), 1, file);
//	fclose(file);
//}
//
//void loadOptions(Window* _window)
//{
//	FILE* file;
//	file = fopen(FILE_PATH"config.cfg", "rb");
//	if (file == NULL) {
//		config.isFullscreen = sfFalse;
//		config.sfxVolume = 50.f;
//		config.musicVolume = 50.f;
//
//		SFXVolume = config.sfxVolume;
//		musicVolume = config.musicVolume;
//
//		file = fopen(FILE_PATH"config.cfg", "ab");
//		file = fclose(file);
//		return;
//	}
//	fread(&config, sizeof(struct Config), 1, file);
//	fclose(file);
//
//	SFXVolume = config.sfxVolume;
//	musicVolume = config.musicVolume;
//
//	if (config.isFullscreen && firstLaunch) {
//		firstLaunch = sfFalse;
//		ToggleFullscreen(_window);
//	}
//
//	ChangeVolume(SOUNDFX, SFXVolume);
//	ChangeVolume(MUSIC, musicVolume);
//}