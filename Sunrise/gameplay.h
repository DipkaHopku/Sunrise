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

//---------------------------------------------------------------------------------------------
//global variables and types





/*//---------------------------------------------------------------------------------------------
//class ApplicationStateController declaration

static class ApplicationStateController {
	friend void gameplayProcessing();

private:
	static void setApplicationState(ApplicationState);

	static ApplicationState getApplicationState();
};



//---------------------------------------------------------------------------------------------
//class ApplicationStateNextController declaration

static class ApplicationStateNextController {
	friend void gameplayProcessing();
	friend void callback_optionsButton_onClick();

private:
	static void setApplicationStateNext(ApplicationState);

	static ApplicationState getApplicationStateNext();
};*/



//---------------------------------------------------------------------------------------------
//functions declaration

void gameplayProcessing();