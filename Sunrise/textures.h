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

#include "userInterface.h"

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
	GRASS,
	STONE,
	BUSH,
	PINE,
	SMALL_STONE,
	SHADOW,
	WIZARD,
	NONE //"пустая" текстура
};

enum class TextureScalingByHeightRatioType {
	PIXELS_NUMBER,
	MULTIPLYNG_FACTOR
};

class ActiveGraphicItem;

//---------------------------------------------------------------------------------------------
//functions

//Загрузка всех текстур игры
void loadTextures();

//void drawTexture(int, int, TextureName);

//void drawScaledTexture(int, int, TextureName, TextureScalingByHeightRatioType, float);

/*
arguments:
#1 - xPos,
#2 - yPos,
#3 - texture name,
#4 (optional) - ActiveGraphicItem,
#5 (optional) - type of scaling relative to height,
#6 (optional) - scaling height factor (multiply or pixels),
#7 (optional) - flip horizontal,
#8 (optional) - flip vertical,
#9 (optional) - transparency (from 0 (transparent) to 1 (opaque))
*/
void drawTexture(int, int, 
	TextureName, 
	ActiveGraphicItem* = nullptr,
	//bool = false,
	TextureScalingByHeightRatioType = TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR,
	float = 1,
	bool = false,
	bool = false,
	float = 1
);

//void _drawTextureWithDepth(int, int, int, const TextureName);

/*
arguments:
#1 - texture name
#2 - returning width
#3 - returning height
*/
void getTextureProperties(TextureName, int*, int*);

//bool** const getTextureControlField(TextureName);

//Texture mergeTextures(struct[](idTexture, xpos, ypos) //чтобы не рисовать заново поля каждый раз

//int _loadTexture(TextureNames);