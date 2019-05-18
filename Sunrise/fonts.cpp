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
		64)}/*,
	{FontName::BUTTON, FontLoadProperties(
		"resources/maturasc.ttf",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
		36)},*/
	//"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
};

class CharData {
public:
	//int bitmapBufferWidth, bitmapBufferHeight;
	//unsigned char* bitmapBuffer = nullptr;

	GLuint TextureID;
	int width, height;
};

class FontProperties {
private:
	map<unsigned char, CharData> CharsData;
public:
	void addCharData(unsigned char charCode, CharData* charData) {
		CharsData.insert(pair<unsigned char, CharData>(charCode, *charData));
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

	/*void getCharData(
		unsigned char charCode,
		GLuint* TextureID,
		int* width,
		int* height
	) const {
		CharData charData = CharsData.at(charCode);
		*TextureID = charData.TextureID;
		*width = charData.width;
		*height = charData.height;
	}*/
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

		CharData charData;
		/*charData.bitmapBufferWidth = glyph->bitmap.width;
		charData.bitmapBufferHeight = glyph->bitmap.rows;
		charData.bitmapBuffer = new unsigned char[glyph->bitmap.width * glyph->bitmap.rows];
		std::memcpy(charData.bitmapBuffer, glyph->bitmap.buffer, sizeof(unsigned char) * (glyph->bitmap.width * glyph->bitmap.rows));*/
		charData.width = glyph->bitmap.width;
		charData.height = glyph->bitmap.rows;

		auto _bitmapBuffer = glyph->bitmap.buffer;
		auto _data = new unsigned char[charData.width * charData.height * 4];
		for (int i = 0; i < charData.width * charData.height; i++) { //белый цвет с альфой, затем можно будет перемножать его с помощью матрицы glColor и получать любой другой без потери альфы
			_data[i * 4] = 255;
			_data[i * 4 + 1] = 255;
			_data[i * 4 + 2] = 255;
			_data[i * 4 + 3] = _bitmapBuffer[i];
		}
		charData.TextureID = SOIL_create_OGL_texture(_data, charData.width, charData.height, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);
		delete[] _data;

		fontProperties.addCharData(charCode, &charData);

		/*for (int y = 0; y < charData.bitmapBufferHeight; ++y) {
			for (int x = 0; x < charData.bitmapBufferWidth; ++x) {
				// Получение прозрачности точки (x, y)
				//const int a = glyph->bitmap.buffer[y * pitch + x];
				const int a = charData.bitmapBuffer[y * charData.bitmapBufferWidth + x];

				//cout << "$";

				if (a > 127) {
					cout << "*";
				}
				else if (a > 63) {
					cout << ".";
				}
				else {
					cout << " ";
				}
			}
			cout << endl;
		}*/

		/*// Получение размеров глифа
		const int width = glyph->bitmap.width;
		const int height = glyph->bitmap.rows;
		//const int pitch = glyph->bitmap.pitch;
		//const int metrics = glyph->metrics;*/

		/*// Вывод символа в консоли
		for (int y = 0; y < charData.bitmapBufferHeight; ++y) {
			for (int x = 0; x < charData.bitmapBufferWidth; ++x) {
				// Получение прозрачности точки (x, y)
				//const int a = glyph->bitmap.buffer[y * pitch + x];
				const int a = glyph->bitmap.buffer[y * charData.bitmapBufferWidth + x];

				if (a > 127) {
					cout << "*";
				} else if (a > 63) {
					cout << ".";
				} else {
					cout << " ";
				}
			}
			cout << endl;
		}*/
	}

	fontsData.insert(pair<FontName, FontProperties>(fontname, fontProperties));

	// Удаление шрифта
	FT_Done_Face(face);
}

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