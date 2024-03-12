#include "gamepadx.h"
#include "gamepad.h"


// This define makes your program compile faster by excluding things we are not using
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif

// Retourne true si un gamepad est connecté
sfBool isConnected(t_joyNum joyNum)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    auto result = XInputGetState(joyNum, &state);
    return  (result == ERROR_SUCCESS);
}

// Retourne le nombre de bouton max d'un gamepad xbox
unsigned int getButtonCount(t_joyNum joyNum)
{
    //return 14;
    return sfJoystick_getButtonCount(joyNum);
}

// Retourne true si le bouton specifié en argument est appuyé
// Note : les gachettes ne sont pas des boutons, voir la méthode getTriggers
sfBool isButtonPressed(t_joyNum joyNum, t_buttonNum buttonNum)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    XInputGetState(joyNum, &state);
    return  (state.Gamepad.wButtons & buttonNum);
}

// Retourne true si au moins un gamepad est connecté
sfBool isAnyXBox360ControllerConnected()
{
    //return  (isConnected(0) || isConnected(1) || isConnected(2) || isConnected(3));
    for (unsigned int i = 0; i < MAX_PLAYER; i++) {
        if (isConnected(i))
            return sfTrue;
    }
    return sfFalse;
}

// Retourne true si le gamepad a un support audio
sfBool voiceSupported(t_joyNum joyNum)
{
    XINPUT_CAPABILITIES caps;
    ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));

    auto result = XInputGetCapabilities(joyNum, XINPUT_FLAG_GAMEPAD, &caps);

    if (result != ERROR_SUCCESS)
        return sfFalse;

    return  (caps.Flags & XINPUT_CAPS_VOICE_SUPPORTED);
}

// Cette méthode ne retourne rien
// Elle modifie directement l'état des arguments left et right passé, avec la valeur des gachettes LT et RT
// Les valeurs sont toujours entre 0 et 1
// TODO: besoin d'une zone morte?
void getTriggers(t_joyNum joyNum, float* left, float* right)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    XInputGetState(joyNum, &state);

    // Normalise
    *left = (float)(state.Gamepad.bLeftTrigger) / 255;
    *right = (float)(state.Gamepad.bRightTrigger) / 255;
}

float getTriggerValue(t_joyNum _id, sfBool _leftTrigger)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    XInputGetState(_id, &state);

    // Normalise
    float value = 0.f;
    if (_leftTrigger) value = (float)(state.Gamepad.bLeftTrigger) / 255;
    else value = (float)(state.Gamepad.bRightTrigger) / 255;

    return value;
}

// Cette méthode ne retourne rien
// Elle modifie directement l'état des arguments left et right passé avec la valeur des sticks
// Les valeurs verient entre -100 et 100
void getSticksPosition(t_joyNum joyNum, sfVector2f* left, sfVector2f* right)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    XInputGetState(joyNum, &state);

    // Verifie la "DEAD ZONE"
    // Stick Gauche
    if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
        state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

        state.Gamepad.sThumbLX = 0;
        state.Gamepad.sThumbLY = 0;

    }

    // Stick Droit
    if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
        state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

        state.Gamepad.sThumbRX = 0;
        state.Gamepad.sThumbRY = 0;

    }

    // Converti les valeurs dans le style SFML (-100..100)
    left->x = (float)(state.Gamepad.sThumbLX / 327);
    left->y = (float)(state.Gamepad.sThumbLY / 327);
    right->x = (float)(state.Gamepad.sThumbRX / 327);
    right->y = (float)(state.Gamepad.sThumbRY / 327);
}

float getStickPos(t_joyNum _id, sfBool _leftStick, sfBool _XAxis)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    XInputGetState(_id, &state);

    // Verifie la "DEAD ZONE"
    // Stick Gauche
    if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
        state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

        state.Gamepad.sThumbLX = 0;
        state.Gamepad.sThumbLY = 0;

    }

    // Stick Droit
    if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
        state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

        state.Gamepad.sThumbRX = 0;
        state.Gamepad.sThumbRY = 0;

    }

    // Converti les valeurs dans le style SFML (-100..100)
    float value = 0.f;
    if (_leftStick) {
        if (_XAxis) value = (float)(state.Gamepad.sThumbLX / 327);
        else value = (float)(state.Gamepad.sThumbLY / 327);
    }
    else {
        if (_XAxis) value = (float)(state.Gamepad.sThumbRX / 327);
        else value = (float)(state.Gamepad.sThumbRY / 327);
    }

    return value;
}

float getDominantStickPos(sfBool _leftStick, sfBool _XAxis)
{
    float realValue = 0.f;
    float value = 0.f;
    float value2 = 0.f;

    for (int i = 0; i < nbPlayer; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        XInputGetState(i, &state);

        // Verifie la "DEAD ZONE"
        // Stick Gauche
        if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
            (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
                state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

            state.Gamepad.sThumbLX = 0;
            state.Gamepad.sThumbLY = 0;

        }

        // Stick Droit
        if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
            (state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
                state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

            state.Gamepad.sThumbRX = 0;
            state.Gamepad.sThumbRY = 0;

        }

        // Converti les valeurs dans le style SFML (-100..100)
        if (_leftStick) {
            if (_XAxis) value = (float)(state.Gamepad.sThumbLX / 327);
            else value = (float)(state.Gamepad.sThumbLY / 327);
        }
        else {
            if (_XAxis) value = (float)(state.Gamepad.sThumbRX / 327);
            else value = (float)(state.Gamepad.sThumbRY / 327);
        }

        if (value >= 0.f) {
            if (value >= value2)
                realValue = value;
            else
                realValue = value2;
        }
        else {
            if (value <= value2)
                realValue = value;
            else
                realValue = value2;
        }

        value2 = value;
    }
  

    return realValue;
}

sfBool isAControllerButtonPressedOrKeyboard(t_joyNum _id, sfKeyCode _key, t_buttonNum _button)
{
    if (sfKeyboard_isKeyPressed(_key))
        return sfTrue;

    if (isButtonPressed(_id, _button))
        return sfTrue;

    return sfFalse;
}

sfBool isSomethingPressed(sfKeyCode _key, t_buttonNum _button)
{
    if (sfKeyboard_isKeyPressed(_key))
        return sfTrue;

    for (int i = 0; i < nbPlayer; i++)
    {
        if (isButtonPressed(i, _button))
            return sfTrue;
    }

    return sfFalse;
}

float isSomethingMoved(sfKeyCode _key, sfBool _XAxis, float _deadZone)
{
    if (sfKeyboard_isKeyPressed(_key)) {
        if (_deadZone < 0.f)
            return -100.f;
        else
            return 100.f;
    }

    float stickPos = 0.f;
    for (int i = 0; i < nbPlayer; i++)
    {
        stickPos = getStickPos(i, sfTrue, _XAxis);
        if (stickPos < _deadZone && _deadZone < 0.f)
            return stickPos;
        else if (stickPos > _deadZone && _deadZone >= 0.f)
            return stickPos;
    }

    return 0.f;
}

// Cette méthode configure les vibrations de 0.0 à 1.0
// 0 arrête les vibrations, 1 vibration au plus fort
void setVibration(t_joyNum joyNum, float leftMotor, float rightMotor)
{
    XINPUT_VIBRATION vib;
    ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));

    vib.wLeftMotorSpeed = (unsigned short)(leftMotor * 65535.0f);
    vib.wRightMotorSpeed = (unsigned short)(rightMotor * 65535.0f);

    XInputSetState(joyNum, &vib);
}

