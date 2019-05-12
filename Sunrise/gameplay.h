#pragma once

enum class ApplicationState {
	MAIN_MENU,
	BATTLE,
	LOAD_MENU,
	OPTIONS
};

void gameplayProcessing();