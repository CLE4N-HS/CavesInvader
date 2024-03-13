#pragma once
#include "tools.h"
#include "stateManager.h"

void initLeaderboard(Window* _window);

void updateLeaderboard(Window* _window);

void displayLeaderboard(Window* _window);

void deinitLeaderboard();

void saveLeaderboard();

void loadLeaderboard();

void sortLeaderboard();
