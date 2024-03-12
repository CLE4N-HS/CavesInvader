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

sfRectangleShape* opaqueRectangle;

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

//sfBool firstLaunch = sfTrue;

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
	opaqueRectangle = sfRectangleShape_create();
	sfRectangleShape_setPosition(opaqueRectangle, VECTOR2F_NULL);
	sfRectangleShape_setOrigin(opaqueRectangle, VECTOR2F_NULL);
	sfRectangleShape_setFillColor(opaqueRectangle, color(0, 0, 0, 127));
	sfRectangleShape_setSize(opaqueRectangle, vector2f(1920.f, 1080.f));

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
	if (escapeOptionsTimer > 0.2f && (isKeyboardOrControllerButtonPressed(sfKeyEscape, START_XBOX) || isKeyboardOrControllerButtonPressed(sfKeyEscape, B_XBOX))) {
		//toggleOptions();
		resetOptions();
		changeMenuState(MENU_MAIN);
	}
	
	// movement
	if (choiceOptionsTimer > 0.3f)
	{
		if (isKeyboardOrControllerButtonMoved(sfKeyUp, STICKLY_XBOX, sfTrue, 50.f)) {
			choiceOptionsTimer = 0.f;
			allowedToSlide = sfFalse;
			choiceOptions = changeOptionsChoice(sfKeyUp);
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyDown, STICKLY_XBOX, sfFalse, 50.f)) {
			choiceOptionsTimer = 0.f;
			allowedToSlide = sfFalse;
			choiceOptions = changeOptionsChoice(sfKeyDown);
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)) {
			choiceOptionsTimer = 0.f;
			choiceOptions = changeOptionsChoice(sfKeyLeft);
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)) {
			choiceOptionsTimer = 0.f;
			choiceOptions = changeOptionsChoice(sfKeyRight);
		}
	}

	// interactions
	switch (choiceOptions)
	{
	case SLIDERSFX_OPTIONS:
		if (!allowedToSlide && !isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 10.f)) {
			allowedToSlide = sfTrue;
		}
		if (!allowedToSlide)
			break;

		if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)) {
			sliderSFXPosX -= dt * 100.f;
			sliderSFXPosX = MAX(sliderSFXPosX, 778.f);
			ChangeVolume(SOUNDFX, getSliderValue(sliderSFXPosX, 778.f, 1049.f));
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)) {
			sliderSFXPosX += dt * 100.f;
			sliderSFXPosX = MIN(sliderSFXPosX, 1049.f);
			ChangeVolume(SOUNDFX, getSliderValue(sliderSFXPosX, 778.f, 1049.f));
		}
		break;
	case FULLSCREEN_OPTIONS:
		if (escapeOptionsTimer > 0.2f && isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)) {
			escapeOptionsTimer = 0.f;
			forceReleasedButton(A_XBOX);
			ToggleFullscreen(_window);
		}
		break;
	case SLIDERMUSIC_OPTIONS:
		if (!allowedToSlide && !isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 10.f)) {
			allowedToSlide = sfTrue;
		}
		if (!allowedToSlide)
			break;

		if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX_XBOX, sfTrue, 50.f)) {
			sliderMusicPosX -= dt * 100.f;
			sliderMusicPosX = MAX(sliderMusicPosX, 784.f);
			ChangeVolume(MUSIC, getSliderValue(sliderMusicPosX, 784.f, 1055.f));
		}
		else if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX_XBOX, sfFalse, 50.f)) {
			sliderMusicPosX += dt * 100.f;
			sliderMusicPosX = MIN(sliderMusicPosX, 1055.f);
			ChangeVolume(MUSIC, getSliderValue(sliderMusicPosX, 784.f, 1055.f));
		}
		break;
	case QUIT_OPTIONS:
		if (escapeOptionsTimer > 0.2f && isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX)) {
			//toggleOptions();
			resetOptions();
			
			forceReleasedButton(A_XBOX);
			//toggleQuit();
			//resetQuit();
			changeMenuState(MENU_MAIN);
		}
		break;
	default:
		break;
	}

}

void displayOptions(Window* _window)
{
	// opacity
	sfRenderTexture_drawRectangleShape(_window->renderTexture, opaqueRectangle, NULL);

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

//#include "options.h"
//#include "soundManager.h"
//#include "stateManager.h"
//#include "viewManager.h"
//#include "gamepad.h"
//
//sfRectangleShape* backgroundOpt;
//sfRectangleShape* sfxVolumeBar;
//sfRectangleShape* MusicVolumeBar;
//sfRectangleShape* fullscreenTick;
//sfText* Options;
//sfText* Fullscreen;
//sfText* SfxSoundTxt;
//sfText* MusicSoundTxt;
//sfText* returnTxt;
//int optionSelect;
//
//
//void initOptionss(Window* _window)
//{
//	backgroundOpt = sfRectangleShape_create();
//	sfRectangleShape_setSize(backgroundOpt, vector2f(500.0f, 300.0f));
//	sfRectangleShape_setFillColor(backgroundOpt, sfBlack);
//	sfRectangleShape_setOutlineColor(backgroundOpt, sfWhite);
//	sfRectangleShape_setOutlineThickness(backgroundOpt, 2.0f);
//	sfRectangleShape_setOrigin(backgroundOpt, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).width / 2.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).height / 2.0f));
//	sfRectangleShape_setPosition(backgroundOpt, mainView->PosView);
//
//	Options = sfText_create();
//	sfText_setFont(Options, getDefaultFont());
//	sfText_setColor(Options, sfRed);
//	sfText_setString(Options, "OPTIONS");
//	sfText_setCharacterSize(Options, 30);
//	sfText_setOrigin(Options, vector2f(sfText_getGlobalBounds(Options).width / 2.0f, sfText_getGlobalBounds(Options).height / 2.0f));
//	sfText_setPosition(Options, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + sfRectangleShape_getGlobalBounds(backgroundOpt).width / 2.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).top + 20.0f));
//
//	returnTxt = sfText_create();
//	sfText_setFont(returnTxt, getDefaultFont());
//	sfText_setColor(returnTxt, sfWhite);
//	sfText_setString(returnTxt, "RETOUR");
//	sfText_setCharacterSize(returnTxt, 20);
//	sfText_setOrigin(returnTxt, vector2f(sfText_getGlobalBounds(returnTxt).width / 2.0f, sfText_getGlobalBounds(returnTxt).height / 2.0f));
//	sfText_setPosition(returnTxt, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + sfRectangleShape_getGlobalBounds(backgroundOpt).width / 2.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).top + sfRectangleShape_getGlobalBounds(backgroundOpt).height - 40.0f));
//
//
//	Fullscreen = sfText_create();
//	sfText_setFont(Fullscreen, getDefaultFont());
//	sfText_setColor(Fullscreen, sfWhite);
//	sfText_setString(Fullscreen, "FullScreen : ");
//	sfText_setCharacterSize(Fullscreen, 20);
//	sfText_setPosition(Fullscreen, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + 20.0f , sfRectangleShape_getGlobalBounds(backgroundOpt).top + 80.0f));
//
//
//	SfxSoundTxt = sfText_create();
//	sfText_setFont(SfxSoundTxt, getDefaultFont());
//	sfText_setColor(SfxSoundTxt, sfWhite);
//	sfText_setString(SfxSoundTxt, "Sfx Volume : ");
//	sfText_setCharacterSize(SfxSoundTxt, 20);
//	sfText_setPosition(SfxSoundTxt, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + 20.0f,  sfRectangleShape_getGlobalBounds(backgroundOpt).top + 140.0f));
//
//
//	MusicSoundTxt = sfText_create();
//	sfText_setFont(MusicSoundTxt, getDefaultFont());
//	sfText_setColor(MusicSoundTxt, sfWhite);
//	sfText_setString(MusicSoundTxt, "Music Volume : ");
//	sfText_setCharacterSize(MusicSoundTxt, 20);
//	sfText_setPosition(MusicSoundTxt, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + 20.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).top + 200.0f));
//
//
//	sfxVolumeBar = sfRectangleShape_create();
//	sfRectangleShape_setSize(sfxVolumeBar, vector2f(SFXVolume, 20.0f));
//	sfRectangleShape_setFillColor(sfxVolumeBar, sfWhite);
//	sfRectangleShape_setPosition(sfxVolumeBar, vector2f( sfText_getGlobalBounds(SfxSoundTxt).left + sfText_getGlobalBounds(SfxSoundTxt).width + 10.0f , sfText_getGlobalBounds(SfxSoundTxt).top));
//
//	MusicVolumeBar = sfRectangleShape_create();
//	sfRectangleShape_setSize(MusicVolumeBar, vector2f(musicVolume, 20.0f));
//	sfRectangleShape_setFillColor(MusicVolumeBar, sfWhite);
//	sfRectangleShape_setPosition(MusicVolumeBar, vector2f(sfText_getGlobalBounds(MusicSoundTxt).left + sfText_getGlobalBounds(MusicSoundTxt).width + 10.0f, sfText_getGlobalBounds(MusicSoundTxt).top));
//
//	fullscreenTick = sfRectangleShape_create();
//	sfRectangleShape_setSize(fullscreenTick, vector2f(20.0f, 20.0f));
//	if (IsFullscreen(_window))
//		sfRectangleShape_setFillColor(fullscreenTick, sfWhite);
//	else
//		sfRectangleShape_setFillColor(fullscreenTick, sfBlack);
//	sfRectangleShape_setOutlineColor(fullscreenTick, sfWhite);
//	sfRectangleShape_setOutlineThickness(fullscreenTick, 2.0f);
//	sfRectangleShape_setPosition(fullscreenTick, vector2f(sfText_getGlobalBounds(Fullscreen).left + sfText_getGlobalBounds(Fullscreen).width + 10.0f, sfText_getGlobalBounds(Fullscreen).top));
//
//
//	optionSelect = 0;
//}
//
//void updateOptionss(Window* _window)
//{
//	static float timer = 0.0f;
//	timer += getDeltaTime();
//
//	if (sfKeyboard_isKeyPressed(sfKeyEscape) && timer > 0.2f)
//	{
//		toggleOptionss();
//		timer = 0.0f;
//	}
//	if (isKeyboardOrControllerButtonMoved(sfKeyDown, STICKLY_XBOX, sfFalse, 30.f) && timer > 0.2f)
//	{
//		optionSelect++;
//		if (optionSelect > 3)
//			optionSelect = 0;
//		timer = 0.0f;
//	}
//	if (isKeyboardOrControllerButtonMoved(sfKeyUp, STICKLY_XBOX, sfTrue, 30.f) && timer > 0.2f)
//	{
//		optionSelect--;
//		if (optionSelect < 0)
//			optionSelect = 3;
//		timer = 0.0f;
//	}
//	if (isKeyboardOrControllerButtonPressed(sfKeyEnter, A_XBOX) && timer > 0.2f)
//	{
//		if (optionSelect == 0)
//		{
//			ToggleFullscreen(_window);
//			for (unsigned int i = 0; i < nbPlayer; i++)
//			{
//			Gamepad[i].Bouton[A_XBOX] = sfFalse;
//			}
//		}
//		if (optionSelect == 3)
//		{
//			toggleOptionss();
//		}
//		timer = 0.0f;
//	}
//	if (isKeyboardOrControllerButtonMoved(sfKeyRight, STICKLX, sfFalse, 30.f) && timer > 0.1f)
//	{
//		if (optionSelect == 1)
//		{
//			if(SFXVolume < 100 )
//			SFXVolume++;
//			ChangeVolume(SOUNDFX, SFXVolume);
//		}
//		else if (optionSelect == 2)
//		{
//			if (musicVolume < 100)
//				musicVolume++;
//			ChangeVolume(MUSIC, musicVolume);
//		}
//		timer = 0.0f;
//	}
//	if (isKeyboardOrControllerButtonMoved(sfKeyLeft, STICKLX, sfTrue, 30.f) && timer > 0.1f)
//	{
//		if (optionSelect == 1)
//		{
//			if (SFXVolume > 0)
//				SFXVolume--;
//			ChangeVolume(SOUNDFX, SFXVolume);
//		}
//		else if (optionSelect == 2)
//		{
//			if (musicVolume > 0)
//				musicVolume--;
//			ChangeVolume(MUSIC, musicVolume);
//		}
//		timer = 0.0f;
//	}
//
//	if (optionSelect == 0)
//	{
//		sfText_setColor(Fullscreen, sfRed);
//		sfText_setColor(SfxSoundTxt, sfWhite);
//		sfText_setColor(MusicSoundTxt, sfWhite);
//		sfText_setColor(returnTxt, sfWhite);
//	}
//	else if (optionSelect == 1)
//	{
//		sfText_setColor(Fullscreen, sfWhite);
//		sfText_setColor(SfxSoundTxt, sfRed);
//		sfText_setColor(MusicSoundTxt, sfWhite);
//		sfText_setColor(returnTxt, sfWhite);
//	}
//	else if (optionSelect == 2)
//	{
//		sfText_setColor(Fullscreen, sfWhite);
//		sfText_setColor(SfxSoundTxt, sfWhite);
//		sfText_setColor(MusicSoundTxt, sfRed);
//		sfText_setColor(returnTxt, sfWhite);
//	}
//	else if (optionSelect == 3)
//	{
//		sfText_setColor(Fullscreen, sfWhite);
//		sfText_setColor(SfxSoundTxt, sfWhite);
//		sfText_setColor(MusicSoundTxt, sfWhite);
//		sfText_setColor(returnTxt, sfRed);
//	}
//
//	sfText_setPosition(Options, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + sfRectangleShape_getGlobalBounds(backgroundOpt).width / 2.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).top + 20.0f));
//	sfText_setPosition(Fullscreen, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + 20.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).top + 80.0f));
//	sfText_setPosition(SfxSoundTxt, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + 20.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).top + 140.0f));
//	sfText_setPosition(MusicSoundTxt, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + 20.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).top + 200.0f));
//	sfText_setPosition(returnTxt, vector2f(sfRectangleShape_getGlobalBounds(backgroundOpt).left + sfRectangleShape_getGlobalBounds(backgroundOpt).width / 2.0f, sfRectangleShape_getGlobalBounds(backgroundOpt).top + sfRectangleShape_getGlobalBounds(backgroundOpt).height - 40.0f));
//
//	sfRectangleShape_setSize(sfxVolumeBar, vector2f(SFXVolume, 20.0f));
//	sfRectangleShape_setSize(MusicVolumeBar, vector2f(musicVolume, 20.0f));
//	sfRectangleShape_setPosition(backgroundOpt, mainView->PosView);
//	sfRectangleShape_setPosition(fullscreenTick, vector2f(sfText_getGlobalBounds(Fullscreen).left + sfText_getGlobalBounds(Fullscreen).width + 10.0f, sfText_getGlobalBounds(Fullscreen).top));
//	sfRectangleShape_setPosition(MusicVolumeBar, vector2f(sfText_getGlobalBounds(MusicSoundTxt).left + sfText_getGlobalBounds(MusicSoundTxt).width + 10.0f, sfText_getGlobalBounds(MusicSoundTxt).top));
//	sfRectangleShape_setPosition(sfxVolumeBar, vector2f(sfText_getGlobalBounds(SfxSoundTxt).left + sfText_getGlobalBounds(SfxSoundTxt).width + 10.0f, sfText_getGlobalBounds(SfxSoundTxt).top));
//
//
//	if (IsFullscreen(_window))
//		sfRectangleShape_setFillColor(fullscreenTick, sfWhite);
//	else
//		sfRectangleShape_setFillColor(fullscreenTick, sfBlack);
//}
//
//void displayOptionss(Window* _window)
//{
//	sfRenderTexture_drawRectangleShape(_window->renderTexture, backgroundOpt, NULL );
//	sfRenderTexture_drawRectangleShape(_window->renderTexture, fullscreenTick, NULL);
//	sfRenderTexture_drawRectangleShape(_window->renderTexture, sfxVolumeBar, NULL);
//	sfRenderTexture_drawRectangleShape(_window->renderTexture, MusicVolumeBar, NULL);
//	sfRenderTexture_drawText(_window->renderTexture, Options, NULL);
//	sfRenderTexture_drawText(_window->renderTexture, Fullscreen, NULL);
//	sfRenderTexture_drawText(_window->renderTexture, SfxSoundTxt, NULL);
//	sfRenderTexture_drawText(_window->renderTexture, MusicSoundTxt, NULL);
//	sfRenderTexture_drawText(_window->renderTexture, returnTxt, NULL);
//}
//
//void deinitOptionss()
//{
//	sfRectangleShape_destroy(backgroundOpt);
//	sfRectangleShape_destroy(fullscreenTick);
//	sfRectangleShape_destroy(sfxVolumeBar);
//	sfRectangleShape_destroy(MusicVolumeBar);
//	sfText_destroy(Options);
//	sfText_destroy(Fullscreen);
//	sfText_destroy(SfxSoundTxt);
//	sfText_destroy(MusicSoundTxt);
//	sfText_destroy(returnTxt);
//}