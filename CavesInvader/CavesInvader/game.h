#pragma once
#include "tools.h"
#include "windowManager.h"

sfBool isGameOver;

/// <summary>
/// Initializes everything needed for the Game
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void initGame(Window* _window);

/// <summary>
/// Updates everything needed for the Game
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void updateGame(Window* _window);

int getNbWave();

int getNb10Waves();

/// <summary>
/// Displays everything needed for the Game
/// </summary>
/// <param name="_window">The whole structure of the Window</param>
void displayGame(Window* _window);

/// <summary>
/// Removes from the memory everything that was needed for the Game
/// </summary>
void deinitGame();

