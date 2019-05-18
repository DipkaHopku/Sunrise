#pragma once

//#include <windows.h>
#include <GL/glew.h>

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>

#include "textures.h"
#include "userInterface.h"

using std::cout;
using std::endl;

enum class ApplicationState {
	MAIN_MENU,
	BATTLE,
	LOAD_MENU,
	OPTIONS
};

void gameplayProcessing();