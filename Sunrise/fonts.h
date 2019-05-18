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

const float buttonFontColorRGB[3] = { 0.f / 255, 0.f / 255, 0.f / 255 };

enum class FontName {
	TITLE,
	BUTTON,
	TEXT
};

//map<FontName, FontProperties> fontsData;

//---------------------------------------------------------------------------------------------
//functions

void freeTypeInit(int);

/*
arguments:
#1 - fontName
#2 -  charCode
#3 -  bitmapBuffer
#4 -  bitmapBufferWidth
#5 -  bitmapBufferHeight
*/
//void getCharBitmapBuffer(FontName, unsigned char, const unsigned char**, int*, int*);