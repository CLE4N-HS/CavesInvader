#pragma once
#include "tools.h"
#include "stateManager.h"
#include "gamepad.h"
#include "textureManager.h"





void initPlayer(Window* _window);

void updatePlayer(Window* _window);

void displayPlayer(Window* _window);

void deinitPlayer();

/// <summary>
/// Gets the closest player position from a world position
/// </summary>
/// <param name="_pos">The world position away from players</param>
/// <returns>The world position of the closest player</returns>
sfVector2f getClosestPlayerPos(sfVector2f _pos);

sfVector2f getPlayerPos(int _playerId);
