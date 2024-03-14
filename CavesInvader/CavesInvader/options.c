#include "options.h"
#include "stateManager.h"
#include "fontManager.h"
#include "textureManager.h"
#include "viewManager.h"
#include "gamepad.h"
#include "dialogBox.h"
#include "quit.h"
#include "CustomMath.h"
#include "soundManager.h"

sfSprite* optionSprite;

sfTexture* optionTexture;
sfTexture* quitOptionsBoxTexture;
sfTexture* fullscreenBoxTexture;
sfTexture* sliderTexture;
sfTexture* sliderBoxTexture;

float sliderSFXPosX;
float sliderMusicPosX;
sfBool allowedToSlide;


typedef enum OptionsChoice {
	NOCHOICE_OPTIONS,
	SLIDERSFX_OPTIONS,
	FULLSCREEN_OPTIONS,
	SLIDERMUSIC_OPTIONS,
	QUIT_OPTIONS
}OptionsChoice;

OptionsChoice choiceOptions;

float escapeOptionsTimer;
float choiceOptionsTimer;
float sliderTimer;

sfBool firstLaunch = sfTrue;

OptionsChoice changeOptionsChoice(sfKeyCode _key)
{
	if (_key == sfKeyUp) {
		switch (choiceOptions) {
		case SLIDERSFX_OPTIONS: return FULLSCREEN_OPTIONS;
		case FULLSCREEN_OPTIONS: return SLIDERSFX_OPTIONS;
		case SLIDERMUSIC_OPTIONS: return SLIDERSFX_OPTIONS;
		case QUIT_OPTIONS: return SLIDERMUSIC_OPTIONS;
		default: break;
		}
	}
	else if (_key == sfKeyDown) {
		switch (choiceOptions) {
		case SLIDERSFX_OPTIONS: return SLIDERMUSIC_OPTIONS;
		case FULLSCREEN_OPTIONS: return QUIT_OPTIONS;
		case SLIDERMUSIC_OPTIONS: return QUIT_OPTIONS;
		case QUIT_OPTIONS: return SLIDERSFX_OPTIONS;
		default: break;
		}
	}
	else if (_key == sfKeyLeft) {
		switch (choiceOptions) {
		case FULLSCREEN_OPTIONS: return SLIDERSFX_OPTIONS;
		case QUIT_OPTIONS: return SLIDERMUSIC_OPTIONS;
		default: break;
		}
	}
	else if (_key == sfKeyRight) {
		switch (choiceOptions) {
		case FULLSCREEN_OPTIONS: return QUIT_OPTIONS;
		case QUIT_OPTIONS: return FULLSCREEN_OPTIONS;
		default: break;
		}
	}

	return choiceOptions;
}

void resetOptions()
{
	escapeOptionsTimer = 0.f;
	choiceOptionsTimer = 0.f;
	sliderTimer = 0.f;
	allowedToSlide = sfFalse;

	setSliderPos(&sliderSFXPosX, SFXVolume, 778.f, 1049.f);
	setSliderPos(&sliderMusicPosX, musicVolume, 784.f, 1055.f);
}

void initOptions(Window* _window)
{
	optionSprite = sfSprite_create();

	optionTexture = GetTexture("options");
	quitOptionsBoxTexture = GetTexture("quitOptBox");
	fullscreenBoxTexture = GetTexture("fullscreenBoxOpt");
	sliderTexture = GetTexture("sliderOpt");
	sliderBoxTexture = GetTexture("sliderBoxOpt");

	sliderSFXPosX = 788.f;
	sliderMusicPosX = 788.f;

	resetOptions();
	choiceOptions = QUIT_OPTIONS;

}

void updateOptions(Window* _window)
{
	float dt = getDeltaTime();
	escapeOptionsTimer += dt;
	choiceOptionsTimer += dt;
	sliderTimer += dt;

	// escape
	if (escapeOptionsTimer > 0.2f && (/*isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX)*/ isSomethingPressed(sfKeyEscape, START) || /*isKeyboardOrControllerButtonPressed(sfKeyEscape, B_XBOX)*/ isSomethingPressed(sfKeyEscape, B))) {
		//toggleOptions();
		PlayASound("button2", sfFalse);
		resetOptions();
		changeMenuState(MENU_MAIN);
	}

	OptionsChoice tmpChoiceOptions = choiceOptions;
	
	// movement
	if (choiceOptionsTimer > 0.3f)
	{
		if (/*isKeyboardOrControllerButtonMoved(sfKeyUp, STICKLY_XBOX, sfTrue, 50.f)*/ isSomethingMoved(sfKeyUp, sfFalse, 50.f) > 50.f) {
			choiceOptionsTimer = 0.f;
			allowedToSlide = sfFalse;
			choiceOptions = changeOptionsChoice(sfKeyUp);
		}
		else if (/*isKeyboardOrControllerButtonMoved(sfKeyDown, STICKLY_XBOX, sfFalse, 50.f)*/ isSomethingMoved(sfKeyDown, sfFalse, -50.f) < -50.f) {
			choiceOptionsTimer = 0.f;
			allowedToSlide = sfFalse;
			choiceOptions = changeOptionsChoice(sfKeyDown);
		}
		else if (/*isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)*/ isSomethingMoved(sfKeyLeft, sfTrue, -50.f) < -50.f) {
			choiceOptionsTimer = 0.f;
			choiceOptions = changeOptionsChoice(sfKeyLeft);
		}
		else if (/*isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)*/ isSomethingMoved(sfKeyRight, sfTrue, 50.f) > 50.f) {
			choiceOptionsTimer = 0.f;
			choiceOptions = changeOptionsChoice(sfKeyRight);
		}
	}

	if (tmpChoiceOptions != choiceOptions) {
		PlayASound("button1", sfFalse);
	}

	// interactions
	switch (choiceOptions)
	{
	case SLIDERSFX_OPTIONS:
		if (!allowedToSlide && /*!isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 10.f)*/ isSomethingMoved(sfKeyLeft, sfTrue, -50.f) < 50.f) {
			allowedToSlide = sfTrue;
		}
		if (!allowedToSlide)
			break;

		if (/*isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)*/ isSomethingMoved(sfKeyLeft, sfTrue, -50.f) < -50.f) {
			sliderSFXPosX -= dt * 100.f;
			sliderSFXPosX = MAX(sliderSFXPosX, 778.f);
			ChangeVolume(SOUNDFX, getSliderValue(sliderSFXPosX, 778.f, 1049.f));
		}
		else if (/*isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)*/ isSomethingMoved(sfKeyRight, sfTrue, 50.f) > 50.f) {
			sliderSFXPosX += dt * 100.f;
			sliderSFXPosX = MIN(sliderSFXPosX, 1049.f);
			ChangeVolume(SOUNDFX, getSliderValue(sliderSFXPosX, 778.f, 1049.f));
		}
		break;
	case FULLSCREEN_OPTIONS:
		if (escapeOptionsTimer > 0.2f && /*isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)*/ isSomethingPressed(sfKeyEnter, A)) {
			escapeOptionsTimer = 0.f;
			forceReleasedButton(A_XBOX);
			ToggleFullscreen(_window);
		}
		break;
	case SLIDERMUSIC_OPTIONS:
		if (!allowedToSlide && /*!isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 10.f)*/ isSomethingMoved(sfKeyLeft, sfTrue, -50.f) < 50.f) {
			allowedToSlide = sfTrue;
		}
		if (!allowedToSlide)
			break;

		if (/*isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)*/ isSomethingMoved(sfKeyLeft, sfTrue, -50.f) < -50.f) {
			sliderMusicPosX -= dt * 100.f;
			sliderMusicPosX = MAX(sliderMusicPosX, 784.f);
			ChangeVolume(MUSIC, getSliderValue(sliderMusicPosX, 784.f, 1055.f));
		}
		else if (/*isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)*/ isSomethingMoved(sfKeyRight, sfTrue, 50.f) > 50.f) {
			sliderMusicPosX += dt * 100.f;
			sliderMusicPosX = MIN(sliderMusicPosX, 1055.f);
			ChangeVolume(MUSIC, getSliderValue(sliderMusicPosX, 784.f, 1055.f));
		}
		break;
	case QUIT_OPTIONS:
		if (escapeOptionsTimer > 0.2f && /*isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)*/ isSomethingPressed(sfKeyEnter, A)) {
			//toggleOptions();
			resetOptions();
			
			forceReleasedButton(A_XBOX);
			//toggleQuit();
			//resetQuit();
			PlayASound("button2", sfFalse);
			changeMenuState(MENU_MAIN);
		}
		break;
	default:
		break;
	}

}

void displayOptions(Window* _window)
{
	// main
	sfSprite_setTexture(optionSprite, optionTexture, sfTrue);
	sfSprite_setPosition(optionSprite, vector2f(481.f, 90.f));
	sfSprite_setOrigin(optionSprite, VECTOR2F_NULL);
	sfRenderTexture_drawSprite(_window->renderTexture, optionSprite, NULL);

	// sliders
	sfSprite_setTexture(optionSprite, sliderTexture, sfTrue);
	sfSprite_setOrigin(optionSprite, vector2f(10.f, 16.f));
	for (int i = 0; i < 2; i++)
	{
		switch (i) {
		case 0:
			sfSprite_setPosition(optionSprite, vector2f(sliderSFXPosX, 396.f));
			break;
		case 1:
			sfSprite_setPosition(optionSprite, vector2f(sliderMusicPosX, 642.f));
			break;
		default:
			break;
		}

		sfRenderTexture_drawSprite(_window->renderTexture, optionSprite, NULL);
	}

	// boxes
	sfSprite_setOrigin(optionSprite, VECTOR2F_NULL);
	switch (choiceOptions)
	{
	case SLIDERSFX_OPTIONS:
		sfSprite_setTexture(optionSprite, sliderBoxTexture, sfTrue);
		sfSprite_setPosition(optionSprite, vector2f(743.f, 367.f));
		break;
	case FULLSCREEN_OPTIONS:
		sfSprite_setTexture(optionSprite, fullscreenBoxTexture, sfTrue);
		sfSprite_setPosition(optionSprite, vector2f(1195.f, 444.f));
		break;
	case SLIDERMUSIC_OPTIONS:
		sfSprite_setTexture(optionSprite, sliderBoxTexture, sfTrue);
		sfSprite_setPosition(optionSprite, vector2f(743.f, 614.f));
		break;
	case QUIT_OPTIONS:
		sfSprite_setTexture(optionSprite, quitOptionsBoxTexture, sfTrue);
		sfSprite_setPosition(optionSprite, vector2f(825.f, 829.f));
		break;
	default:
		break;
	}
	sfRenderTexture_drawSprite(_window->renderTexture, optionSprite, NULL);
}

void deinitOptions()
{
	sfSprite_destroy(optionSprite);
}

void saveOptions(Window* _window)
{
	config.isFullscreen = IsFullscreen(_window);
	//SFXVolume = config.sfxVolume;
	//musicVolume = config.musicVolume;

	config.sfxVolume = SFXVolume;
	config.musicVolume = musicVolume;

	FILE* file;
	file = fopen(FILE_PATH"config.cfg", "wb");
	fwrite(&config, sizeof(struct Config), 1, file);
	fclose(file);
}

void loadOptions(Window* _window)
{
	FILE* file;
	file = fopen(FILE_PATH"config.cfg", "rb");
	if (file == NULL) {
		config.isFullscreen = sfFalse;
		config.sfxVolume = 50.f;
		config.musicVolume = 50.f;

		SFXVolume = config.sfxVolume;
		musicVolume = config.musicVolume;

		file = fopen(FILE_PATH"config.cfg", "ab");
		file = fclose(file);
		return;
	}
	fread(&config, sizeof(struct Config), 1, file);
	fclose(file);

	SFXVolume = config.sfxVolume;
	musicVolume = config.musicVolume;

	if (config.isFullscreen && firstLaunch) {
		firstLaunch = sfFalse;
		ToggleFullscreen(_window);
	}

	ChangeVolume(SOUNDFX, SFXVolume);
	ChangeVolume(MUSIC, musicVolume);
}