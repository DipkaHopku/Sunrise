#pragma once

//#include <windows.h>
//#include <GLFW/glfw3.h>
//#include <GLAPI // and GLAPIENTRY
//#define GLAPI extern
#include <GL/glew.h>
//#include <GL/gl.h>
#include <SOIL.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
//#include <ctime>
//#include <chrono>

/*#include <stdio.h>
#include <windows.h>
#include <excpt.h>*/

//#include <csignal>

using std::string;
using std::map;
using std::pair;
using std::cout;
using std::cerr;
using std::endl;

//---------------------------------------------------------------------------------------------
//global variables and types

enum class TextureName {
	MAIN_MENU_BACKGROUND, 
	TEST1,
	TEST2,
	TEST3,
	WTF_CAT
};

enum class TextureScalingByHeightRatioType {
	PIXELS_NUMBER,
	MULTIPLYNG_FACTOR
};

//---------------------------------------------------------------------------------------------
//functions

//Загрузка всех текстур игры
void loadTextures();

void drawTexture(int, int, TextureName);

void drawScaledTexture(int, int, TextureName, TextureScalingByHeightRatioType, float);

//Texture mergeTextures(struct[](idTexture, xpos, ypos) //чтобы не рисовать заново поля каждый раз

//int _loadTexture(TextureNames);