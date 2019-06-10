#pragma once

#include <GL/glew.h>
#include <SOIL.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iterator>

using std::map;
using std::pair;
using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
//using std::copy;

//---------------------------------------------------------------------------------------------
//global variables and types

const float TITLE_FONT_COLOR_RGB[4] = { 255.f/255, 255.f/255, 255.f/255, 255.f/255};
const float BUTTON_FONT_COLOR_RGB[4] = { 255.f/255, 255.f/255, 255.f/255, 255.f/255}; //buttonFontColorRGB

enum class FontName {
	TITLE,
	BUTTON,
	LABEL,
	SIMPLE_TEXT
};

//map<FontName, FontProperties> fontsData;

//---------------------------------------------------------------------------------------------
//functions

void freeTypeInit(int);

/*
arguments:
#1 - fontName,
#2-  string,
#3 - width,
#4 - height
*/
void getStringProperties(FontName, string, int*, int*);

/*
arguments:
#1 - xPos,
#2-  yPos,
#3 - fontName,
#4 - string,
#5 - RGB-color vector
*/
void drawString(int, int, FontName, string, const float[3]);

/*
arguments:
#1 - fontName
#2 -  charCode
#3 -  bitmapBuffer
#4 -  bitmapBufferWidth
#5 -  bitmapBufferHeight
*/
//void getCharBitmapBuffer(FontName, unsigned char, const unsigned char**, int*, int*);