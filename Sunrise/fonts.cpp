#include "fonts.h"

//---------------------------------------------------------------------------------------------
//local variables and types


class FontLoadProperties {
private:
	string _filename;
	string _chars;
	int _charSize;
public:
	FontLoadProperties(string filename, string chars, int charSize) {
		_filename = filename;
		_chars = chars;
		_charSize = charSize;
	}
	~FontLoadProperties() = default;

	string getFilename() const {
		return _filename;
	}

	string getChars() const {
		return _chars;
	}

	int getCharSize() const {
		return _charSize;
	}
};

const map<const FontName, const FontLoadProperties> fontsLoadData = {
	{FontName::TITLE, FontLoadProperties(
		"resources/maturasc.ttf", 
		"Sunrise", 
		128)},
	{FontName::BUTTON, FontLoadProperties(
		"resources/maturasc.ttf",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ",
		36)},
	{FontName::SIMPLE_TEXT, FontLoadProperties(
		"resources/maturasc.ttf",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ",
		24)},
	//"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
};

class CharProperties {
public:
	//int bitmapBufferWidth, bitmapBufferHeight;
	//unsigned char* bitmapBuffer = nullptr;

	GLuint textureID;
	int width, height;

	int bearingX, bearingY, advance;
};

class FontProperties {
private:
	map<unsigned char, CharProperties> _CharsData;

	int _upperIndent = 0, _bottomIndent = 0;
public:
	void addCharProperties(unsigned char charCode, CharProperties* charProperties) {
		_CharsData.insert(pair<unsigned char, CharProperties>(charCode, *charProperties));
	}

	void setIndents(int upperIndent, int bottomIndent) {
		if (upperIndent > _upperIndent) {
			_upperIndent = upperIndent;
		}
		if (bottomIndent > _bottomIndent) {
			_bottomIndent = bottomIndent;
		}
	}

	void getIndents(int* upperIndent, int* bottomIndent) const {
		*upperIndent = _upperIndent;
		*bottomIndent = _bottomIndent;
	}

	/*const unsigned char* getCharBitmapBuffer(unsigned char charCode) const {
		CharData charData = CharsData.at(charCode);
		return charData.bitmapBuffer;
	}

	const unsigned char* getCharBitmapBuffer(unsigned char charCode) const {
		CharData charData = CharsData.at(charCode);
		return charData.bitmapBuffer;
	}*/

	/*void getCharBitmapBuffer(
		unsigned char charCode,
		const unsigned char** bitmapBuffer,
		int* bitmapBufferWidth,
		int* bitmapBufferHeight
	) const {
		CharData charData = CharsData.at(charCode);
		*bitmapBuffer = charData.bitmapBuffer;
		*bitmapBufferWidth = charData.bitmapBufferWidth;
		*bitmapBufferHeight = charData.bitmapBufferHeight;
	}*/

	/*void getCharPropertiesForDrawing(
		unsigned char charCode,
		GLuint* textureID,
		int* width,
		int* height
	) const {
		auto _charData = _CharsData.find(charCode);
		if (_charData != _CharsData.end()) {
			//CharProperties _charProperties = _charData->second;
			*textureID = _charData->second.textureID;
			*width = _charData->second.width;
			*height = _charData->second.height;
		}
		else {
			*textureID = 0;
		}
	}
	
	void getCharPropertiesForStationing(
		unsigned char charCode,
		int* width,
		int* height,
		int* bearingX,
		int* bearingY,
		int* advance
	) const {
		auto _charData = _CharsData.find(charCode);
		if (_charData != _CharsData.end()) {
			*width = _charData->second.width;
			*height = _charData->second.height;
			*bearingX = _charData->second.bearingX;
			*bearingY = _charData->second.bearingY;
			*advance = _charData->second.advance;
		}
		else {
			charCode = 0;
		}
	}*/

	int getCharWidth(unsigned char charCode) {
		auto _charData = _CharsData.find(charCode);
		if (_charData != _CharsData.end()) {
			return _charData->second.advance;
		}
		else {
			return 0;
		}
	}

	void getCharProperties(
		unsigned char charCode,
		GLuint* textureID,
		int* width,
		int* height,
		int* bearingX,
		int* bearingY,
		int* advance
	) const {
		auto _charData = _CharsData.find(charCode);
		if (_charData != _CharsData.end()) {
			CharProperties _charProperties = _charData->second;
			*textureID = _charProperties.textureID;
			*width = _charProperties.width;
			*height = _charProperties.height;
			*bearingX = _charProperties.bearingX;
			*bearingY = _charProperties.bearingY;
			*advance = _charProperties.advance;
		} else {
			*textureID = 0;
		}
	}
};

map<FontName, FontProperties> fontsData;

void loadFont(FontName, FT_Library&, int);

//---------------------------------------------------------------------------------------------
//functions

void freeTypeInit(int DPI) {
	// Библиотека FreeType
	FT_Library library;

	// Инициализация библиотеки
	if (FT_Init_FreeType(&library)) {
		std::cerr << "ERROR::FREETYPE: Could not init freetype library" << endl;
	}

	for (const auto &fontLoadData : fontsLoadData) {
		loadFont(fontLoadData.first, library, DPI);
	}	

	// Удаление библиотеки
	FT_Done_FreeType(library);
}

void loadFont(FontName fontname, FT_Library& library, int DPI) {
	FontLoadProperties fontLoadProperties = fontsLoadData.at(fontname);

	// Шрифт
	FT_Face face;

	// Загрузка шрифта
	if (FT_New_Face(library, fontLoadProperties.getFilename().c_str(), 0, &face)) {
		std::cerr << "ERROR::FREETYPE: Failed to load font" << endl;
	}

	// Установка размера пикселя
	//FT_Set_Pixel_Sizes(face, 24, 12);
	//FT_Set_Pixel_Sizes(face, 0, 24);

	// FreeType измеряет размер шрифта в терминах 1/64 пикселя.
	// Для того чтобы сделать шрифт выстой h пикселей, мы запрашиваем размер h*64.
	// (h << 6 тоже самое что и h*64)
	FT_Set_Char_Size(face, fontLoadProperties.getCharSize() << 6, 0, DPI, 0);

	FontProperties fontProperties;

	//const char* const chars = fontLoadProperties.getChars().c_str();
	for (int i = 0; i < fontLoadProperties.getChars().length(); i++) {
		// Код символа (юникод)
		//const FT_ULong charCode = chars[0];
		const FT_ULong charCode = fontLoadProperties.getChars()[i];

		// Загрузка глифа из шрифта с его отрисовкой
		if (FT_Load_Char(face, charCode, FT_LOAD_RENDER)) {
			std::cerr << "Error: Failed to load Glyph of char '" << (char)charCode << "' from file '" << fontLoadProperties.getFilename() << "'" << endl;
		}

		// Получение готового к использованию глифа
		FT_GlyphSlot glyph = face->glyph;

		CharProperties charProperties;
		
		charProperties.width = glyph->bitmap.width;
		charProperties.height = glyph->bitmap.rows;

		/*charProperties.bearingX = glyph->metrics.horiBearingX/64; //возвращает в 1/64 пикселя
		charProperties.bearingY = glyph->metrics.horiBearingY/64;
		charProperties.advance = glyph->metrics.horiAdvance/64;*/
		charProperties.bearingX = glyph->bitmap_left;
		charProperties.bearingY = glyph->bitmap_top;
		charProperties.advance = glyph->advance.x/64; //возвращает в 1/64 пикселя

		auto _bitmapBuffer = glyph->bitmap.buffer;
		auto _data = new unsigned char[charProperties.width * charProperties.height * 4];
		for (int i = 0; i < charProperties.width * charProperties.height; i++) { //белый цвет с альфой, затем можно будет перемножать его с помощью матрицы glColor и получать любой другой без потери альфы
			_data[i * 4] = 255;
			_data[i * 4 + 1] = 255;
			_data[i * 4 + 2] = 255;
			_data[i * 4 + 3] = _bitmapBuffer[i];
		}
		charProperties.textureID = SOIL_create_OGL_texture(_data, charProperties.width, charProperties.height, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);
		delete[] _data;

		fontProperties.setIndents(charProperties.bearingY, charProperties.height - charProperties.bearingY);
		fontProperties.addCharProperties(charCode, &charProperties);
	}

	fontsData.insert(pair<FontName, FontProperties>(fontname, fontProperties));

	// Удаление шрифта
	FT_Done_Face(face);
}

//возвращает высоту строки, ширину строки
void getStringProperties(const FontName fontName, const string string, int* width, int* height) { //, int*** charsLocation) {
	*width = 0;
	*height = 0;
	auto _fontData = fontsData.find(fontName);
	if (_fontData != fontsData.end()) {
		FontProperties fontProperties = _fontData->second;
		int upperIndent, bottonIndent;
		fontProperties.getIndents(&upperIndent, &bottonIndent);
		*height = upperIndent + bottonIndent;

		int defaultCharSize = fontsLoadData.at(fontName).getCharSize();
		for (int i = 0; i < string.length(); i++) {
			int charWidth = _fontData->second.getCharWidth(string[i]);
			if (charWidth != 0) {
				*width += charWidth;
			} else {
				*width += defaultCharSize;
			}
		}
	}
}

int drawChar(
	const int xPos, 
	const int yPos, 
	const FontName fontName, 
	const unsigned char charCode
) {
	auto _fontData = fontsData.find(fontName);
	if (_fontData != fontsData.end()) {
		GLuint _textureID;
		int _width, _height, _bearingX, _bearingY, _advance;
		_fontData->second.getCharProperties(charCode, &_textureID, &_width, &_height, &_bearingX, &_bearingY, &_advance);

		int _xPos, _yPos;
		if (_textureID != 0) {
			_xPos = xPos + _bearingX;
			_yPos = yPos - _bearingY;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureID);

			glBegin(GL_QUADS);
			glTexCoord2i(0, 0); glVertex2i(_xPos, _yPos);
			glTexCoord2i(1, 0); glVertex2i(_xPos + _width, _yPos);
			glTexCoord2i(1, 1); glVertex2i(_xPos + _width, _yPos + _height);
			glTexCoord2i(0, 1); glVertex2i(_xPos, _yPos + _height);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);

			return xPos + _advance;
		}
		else {
			int _defaultCharSize = fontsLoadData.at(fontName).getCharSize();
			_xPos = xPos + _defaultCharSize/10;
			_yPos = yPos - _bearingY;
			_width = (_defaultCharSize / 10) * 8;

			glLineWidth((int)_defaultCharSize / 10);
			glBegin(GL_LINE_LOOP);
			glVertex2i(_xPos, yPos - _defaultCharSize);			//(0, 0)
			glVertex2i(xPos + _width, yPos - _defaultCharSize);	//(1, 0)
			glVertex2i(xPos + _width, yPos);					//(1, 1)
			glVertex2i(_xPos, yPos);							//(0, 1)
			glEnd();
			/*glPointSize((int)_defaultCharSize / 10);
			glBegin(GL_POINTS);
			glVertex2i(_xPos, yPos - _defaultCharSize);			//(0, 0)
			glVertex2i(xPos + _width, yPos - _defaultCharSize);	//(1, 0)
			glVertex2i(xPos + _width, yPos);					//(1, 1)
			glVertex2i(_xPos, yPos);							//(0, 1)
			glEnd();*/

			return xPos + _defaultCharSize;
		}
		
	} else return xPos;
}

void drawString( //позиции для отрисовки каждого символа должны быть известны до отрисовке на момент создания элемента интерфейса, вмешающего в себя текст - сделал наполовину
	const int xPos, 
	const int yPos, 
	const FontName fontName, 
	const string string, 
	const float color[3]
) {
	auto _fontData = fontsData.find(fontName);
	if (_fontData != fontsData.end()) {
		int _xPosNextChar = xPos, _yPosNextChar = yPos;
		int _upperIndent, _bottomIndent;
		_fontData->second.getIndents(&_upperIndent, &_bottomIndent);
		_yPosNextChar += _upperIndent;

		glColor4fv(color); //цвет символа
		//glColor4fv(BUTTON_FONT_COLOR_RGB);

		for (int i = 0; i < string.length(); i++) {
			_xPosNextChar = drawChar(_xPosNextChar, _yPosNextChar, fontName, string[i]); //координаты origin-точки
		}
	}
}

/*void drawChar(
	const int xPos,
	const int yPos,
	const FontName fontName,
	const unsigned char charCode,
	const float color[3]
) {
	auto _fontData = fontsData.find(fontName);
	if (_fontData != fontsData.end()) {
		GLuint _textureID;
		int _width, _height;
		_fontData->second.getCharPropertiesForDrawing(charCode, &_textureID, &_width, &_height);
		if (_textureID == 0) return; //символ не найден

		glColor4fv(color); //цвет символа

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureID);

		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(xPos, yPos); //i - это int
		glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos);
		glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height);
		glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}*/

/*void getCharBitmapBuffer(
	FontName fontName,
	unsigned char charCode,
	const unsigned char** bitmapBuffer,
	int* bitmapBufferWidth,
	int* bitmapBufferHeight
) {
	fontsData.at(fontName).getCharBitmapBuffer(
		charCode,
		bitmapBuffer,
		bitmapBufferWidth,
		bitmapBufferHeight
	);
}*/