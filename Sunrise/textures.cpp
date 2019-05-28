#include "textures.h"

//using namespace std::chrono;

std::ostream& operator << (std::ostream& os, const TextureName& obj) {
	os << static_cast<std::underlying_type<TextureName>::type>(obj);
	return os;
}

//unordered_map<const TextureNames, const char* const> textureFilenames = { //нужна hash функция https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

//const map<const TextureNames, const char* const> textureFilenames = {

const map<const TextureName, const string> textureFilenames = {
	{TextureName::MAIN_MENU_BACKGROUND, "resources/textures/mainMenuBackground.png"},
	{TextureName::GRASS, "resources/textures/grass.png"},
	{TextureName::STONE, "resources/textures/stone.png"},
	{TextureName::BUSH, "resources/textures/bush.png"},
	{TextureName::PINE, "resources/textures/pine.png"},
	{TextureName::SMALL_STONE, "resources/textures/smallStone.png"},
	{TextureName::SHADOW, "resources/textures/shadow.png"}
};

class TextureProperties {
public:
	GLuint ID;
	int width;
	int height;
	bool** controlField = nullptr;

	/*~TextureProperties() {
		for (int x = 0; x < height; x++) {
			delete[] controlField[x];
		}
		delete[] controlField;
	}*/
};

//map<TextureName, TextureProperties> texturesData;
map<TextureName, const TextureProperties> texturesData;

//---------------------------------------------------------------------------------------------
//functions

void loadTexture(const TextureName textureName) {
	TextureProperties textureProperties;
	//auto data = SOIL_load_image(textureFilenames.find(textureName)->second, &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	//auto _data = SOIL_load_image(textureFilenames[textureName].c_str(), &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	auto _data = SOIL_load_image(textureFilenames.at(textureName).c_str(), &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	if (_data != nullptr) {
		textureProperties.ID = SOIL_create_OGL_texture(_data, textureProperties.width, textureProperties.height, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT); //SOIL_FLAG_DDS_LOAD_DIRECT нет искажения текстур, при это SOIL_FLAG_POWER_OF_TWO оно насильно меняет ширину и высоту на степень двойки и изза этого появляется неотключаемое размытие

		bool** controlField = new bool*[textureProperties.width];
		for (int x = 0; x < textureProperties.width; x++) {
			controlField[x] = new bool[textureProperties.height];
			for (int y = 0; y < textureProperties.height; y++) {
				bool isFilledPixel = (int)_data[(x + y * textureProperties.width) * 4 + 3] != 0;
				controlField[x][y] = isFilledPixel;

				/*if (textureName == TextureName::TEST3 && y<5 && x < 6) {
					cout << (float)_data[(y + x * textureProperties.width) * 4] / 255;
					cout << (float)_data[(y + x * textureProperties.width) * 4 + 1] / 255;
					cout << (float)_data[(y + x * textureProperties.width) * 4 + 2] / 255;
					cout << (float)_data[(y + x * textureProperties.width) * 4 + 3] / 255;
					cout << '-';
				}*/
			}
			//if (textureName == TextureName::TEST3 && x<6) cout << endl;
		}

		textureProperties.controlField = controlField;

		SOIL_free_image_data(_data);

		texturesData.insert(pair<TextureName, TextureProperties>(textureName, textureProperties));
	} else cout << "NullPointerException: texture '" << textureName << "' with filename '" << textureFilenames.at(textureName) << "' is not available" << endl;
}

void loadTextures() {
	for (const auto &textureFilename : textureFilenames) {
		loadTexture(textureFilename.first);
	}
}

/*void drawTexture(const int xPos, const int yPos, const TextureName textureName) {
	auto _textureData = texturesData.find(textureName);
	if (_textureData != texturesData.end()) {
		TextureProperties _textureProperties = _textureData->second;

		glColor4f(1, 1, 1, 1); //чтобы текстура имела натуральный цвет

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureProperties.ID); //чем меньше колво айди - тем лучше

		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//выход текстурных координат за пределы 0-1
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //сглаживание при уменьшении
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //сглаживание при увеличении //NEAREST - по ближайшему пикселю

		glBegin(GL_QUADS); //устаревший интерфейс, НО время выполнения функции всё ещё менее 1 миллисекунды
		glTexCoord2i(0, 0); glVertex2i(xPos, yPos); //i - это int
		glTexCoord2i(1, 0); glVertex2i(xPos + _textureProperties.width, yPos);
		glTexCoord2i(1, 1); glVertex2i(xPos + _textureProperties.width, yPos + _textureProperties.height);
		glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _textureProperties.height);
		//МОЖЕТ БЫТЬ! рисовать отдельные объекты из одной мегатекстуры быстрее, тк МОЖЕТ БЫТЬ операция glBindTexture() - очень дорогая //протестил. если уменьшение есть, то почти незаметное (<< 10%)
		//glTexCoord2f(0.5f, 0); glVertex2i(xPos, yPos); //i - это int
		//glTexCoord2f(1, 0); glVertex2i(xPos + textureProperties.width/2, yPos);
		//glTexCoord2f(1, 1); glVertex2i(xPos + textureProperties.width/2, yPos + textureProperties.height);
		//glTexCoord2f(0.5f, 1); glVertex2i(xPos, yPos + textureProperties.height);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	} //else cout << "Error: texture '" << textureName << "' hasn`t properties" << endl;
}

void drawScaledTexture(
	const int xPos, const int yPos,
	const TextureName textureName,
	const TextureScalingByHeightRatioType scalingByHeightRatioType, 
	const float scalingHeightFactor
) {
	auto _textureData = texturesData.find(textureName);
	if (_textureData != texturesData.end()) {
		TextureProperties _textureProperties = _textureData->second;

		int _width, _height;
		float _ratio;

		switch (scalingByHeightRatioType) {
		case TextureScalingByHeightRatioType::PIXELS_NUMBER:
			_height = scalingHeightFactor;
			_ratio = (float) _height / _textureProperties.height;
			_width = _textureProperties.width * _ratio;
			break;
		case TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR:
			_ratio = scalingHeightFactor;
			_height = _textureProperties.height * _ratio;
			_width = _textureProperties.width * _ratio;
			break;
		default: //никогда не заходит?
			cerr << "Error: Unknown ratio type of texture scaling by height" << endl;
			return;
			break;
		}

		glColor4f(1, 1, 1, 1); //чтобы текстура имела натуральный цвет

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //сглаживание при уменьшении
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //сглаживание при увеличении //NEAREST - по ближайшему пикселю

		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(xPos, yPos);
		glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos);
		glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height);
		glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height);	
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);	
	}
}*/



void drawTexture(
	const int xPos, const int yPos,
	const TextureName textureName,
	const bool isActived,
	const TextureScalingByHeightRatioType scalingByHeightRatioType, // = TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR,
	const float scalingHeightFactor // = 1
) {
	auto _textureData = texturesData.find(textureName);
	if (_textureData != texturesData.end()) {
		TextureProperties _textureProperties = _textureData->second;

		int _width, _height;
		float _ratio;

		switch (scalingByHeightRatioType) {
		case TextureScalingByHeightRatioType::PIXELS_NUMBER:
			_height = scalingHeightFactor;
			_ratio = (float)_height / _textureProperties.height;
			_width = _textureProperties.width * _ratio;
			break;
		case TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR:
			_ratio = scalingHeightFactor;
			_height = _textureProperties.height * _ratio;
			_width = _textureProperties.width * _ratio;
			break;
		default: //никогда не заходит?
			cerr << "Error: Unknown ratio type of texture scaling by height" << endl;
			return;
			break;
		}

		glColor4f(1, 1, 1, 1); //чтобы текстура имела натуральный цвет

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //сглаживание при уменьшении
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //сглаживание при увеличении //NEAREST - по ближайшему пикселю

		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(xPos, yPos);
		glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos);
		glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height);
		glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}



void getTextureProperties(TextureName textureName, int* width, int* height) {
	if (width != nullptr) *width = 0;
	if (height != nullptr) *height = 0;

	auto _textureData = texturesData.find(textureName);
	if (_textureData != texturesData.end()) {
		if (width != nullptr) *width = _textureData->second.width;
		if (height != nullptr) *height = _textureData->second.height;
	}
}



/*void _drawTextureWithDepth(const int xPos, const int yPos, int zPos, const TextureName textureName) {
	auto _textureData = texturesData.find(textureName);
	if (_textureData != texturesData.end()) {
		TextureProperties _textureProperties = _textureData->second;

		glColor4f(1, 1, 1, 1); //чтобы текстура имела натуральный цвет

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureProperties.ID); //чем меньше колво айди - тем лучше

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //сглаживание при уменьшении
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //сглаживание при увеличении //NEAREST - по ближайшему пикселю

		glBegin(GL_QUADS); //устаревший интерфейс, НО время выполнения функции всё ещё менее 1 миллисекунды
		glTexCoord3i(0, 0, 1); glVertex3i(xPos, yPos, zPos); //i - это int
		glTexCoord3i(1, 0, 1); glVertex3i(xPos + _textureProperties.width, yPos, zPos);
		glTexCoord3i(1, 1, 1); glVertex3i(xPos + _textureProperties.width, yPos + _textureProperties.height, zPos);
		glTexCoord3i(0, 1, 1); glVertex3i(xPos, yPos + _textureProperties.height, zPos);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}*/


/*void drawQuadRotated45DegreesWithSameAngles(int xPos, int yPos, int width) {
	glVertex2i(_xPos + _stencilLineWindth - 4, _yPos + _stencilLineWindth + 4); //<
	glVertex2i(_xPos + _stencilLineWindth + 4, _yPos + _stencilLineWindth - 4); //^
	glVertex2i(_xPos - _stencilLineWindth + _width + 4, _yPos - _stencilLineWindth + _height - 4); //>

	glVertex2i(_xPos + _stencilLineWindth - 4, _yPos + _stencilLineWindth + 4); //<
	glVertex2i(_xPos - _stencilLineWindth + _width - 4, _yPos - _stencilLineWindth + _height + 4); //V
	glVertex2i(_xPos - _stencilLineWindth + _width + 4, _yPos - _stencilLineWindth + _height - 4); //>

	glVertex2i(_xPos + _stencilLineWindth - 4 + 1, _yPos + _stencilLineWindth + 4); //<
	glVertex2i(_xPos + _stencilLineWindth + 4 + 1, _yPos + _stencilLineWindth - 4); //^
	glVertex2i(_xPos - _stencilLineWindth + _width + 4 + 1, _yPos - _stencilLineWindth + _height - 4); //>

	glVertex2i(_xPos + _stencilLineWindth - 4 + 1, _yPos + _stencilLineWindth + 4); //<
	glVertex2i(_xPos - _stencilLineWindth + _width - 4 + 1, _yPos - _stencilLineWindth + _height + 4); //V
	glVertex2i(_xPos - _stencilLineWindth + _width + 4 + 1, _yPos - _stencilLineWindth + _height - 4); //>
}*/







/*unsigned int fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo); //GL_FRAMEBUFFER_EXT

		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 640, 480);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureProperties.ID, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_6_5, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		//glReadBuffer(GL_COLOR_ATTACHMENT0); //_EXT

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			cout << "complete" << endl;
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//glBindTexture(GL_TEXTURE_2D, textureProperties.ID);

			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureProperties.width, textureProperties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			drawTexture(0, 0, textureName);
			glRasterPos2i(0, 0);
			glPixelZoom(1.0, -1.0);
			glDrawPixels(textureProperties.width, textureProperties.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			vector<unsigned char> pixels(textureProperties.width * textureProperties.height * 4);
			//GLubyte* img = (GLubyte*)malloc(textureProperties.width * textureProperties.height * sizeof(GLubyte) * 4);
			glReadPixels(0, 0, textureProperties.width, textureProperties.height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
			if (!pixels.empty()) {
				for (int i = 0; i < textureProperties.width * textureProperties.height * 4; i++)
					if (pixels[i] != '\0') cout << "p: " << pixels[i];
				cout << endl;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &fbo);
		}*/






/*void drawTexture(int xPos, int yPos, TextureNames textureName) {
	TextureProperties textureProperties;
	auto data = SOIL_load_image(textureFilenames[textureName].c_str(), &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	//glColor3f(1.0f, 1.0f, 1.0f);
	//milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	auto start = clock();
	glRasterPos2i(xPos, yPos);
	glPixelZoom(1.0, -1.0);
	glDrawPixels(textureProperties.width, textureProperties.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	cout << "processing: " << clock() - start << endl; //13 миллисекунд
	SOIL_free_image_data(data);
}*/






//void __drawBorder() {
/*//мусор
//glClearColor(0, 1, 0, 1);
		/*glColor4f(1, 0, 0, 1);
		glClearColor(0, 1, 0, 0);
		glEnable(GL_BLEND);*/
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);
		//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ZERO); //+ glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND); цвет очистки со альфой 255
		//glBlendFunc(GL_ZERO, GL_ONE); //цвет очистки со 100% альфой
		//glBlendFunc(GL_ONE, GL_ONE);
		//glBlendFunc(GL_ONE, GL_ALPHA);
		//glBlendFuncSeparate(GL_DST_COLOR, GL_SRC_COLOR, GL_ONE, GL_ONE);
		/*glBlendColor(0, 0, 1, 1);
		//glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA);
		glBlendFunc(GL_CONSTANT_COLOR, GL_ZERO);
		//glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/

		//glBlendColor(0, 1, 1, 1); //пропадает прозрачность работает но не заливает тёмные места. остаётся "тень"
		//glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_COLOR);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

		//не работает
		/*glPixelTransferi(GL_MAP_COLOR, 1);//использоваь карту
		float map[3] = { 1, 0.5, 0 };
		glPixelMapfv(GL_PIXEL_MAP_R_TO_R, 1, &map[1]);
		glPixelMapfv(GL_PIXEL_MAP_G_TO_G, 1, &map[2]);
		glPixelMapfv(GL_PIXEL_MAP_B_TO_B, 1, &map[3]);*/

		//glColor4f(0, 0, 0, 1);//закрашивает текстуру в чёрный


		//glClearColor(0, 0, 0, 0);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);*/


	//int xPos, yPos, ID, _width, _height;

	/*//мусор
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0);
	glStencilMask(1);*/

	/*int _borderSize = 10;

	glAlphaFunc(GL_GEQUAL, 0.01); //рисуем только пиксель с альфой меньше 0.01
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_STENCIL_TEST);

	//рисуем только в этой области
	//что делать если не тест не пройдер; тест трафарета пройден, но тест глубины - нет; и тест трафарета и глубины - пройден
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//функция сравнения; подставляемое значение?; идентификатор маски?
	glStencilFunc(GL_ALWAYS, 1, 255); //сложно непонятно https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFunc.xml

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE); //не рисуем в окно, но рисуем в трафарет?
	glColor4f(0, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos);
	glTexCoord2i(1, 0); glVertex2i(xPos + _width + _borderSize * 2, yPos);
	glTexCoord2i(1, 1); glVertex2i(xPos + _width + _borderSize * 2, yPos + _height + _borderSize * 2);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height + _borderSize * 2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//не рисуем в этой области
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_NEVER, 2, 255);

	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilFunc(GL_ALWAYS, 1, 255);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	glColor4f(0, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos + _borderSize, yPos + _borderSize);
	glTexCoord2i(1, 0); glVertex2i(xPos + _borderSize + _width, yPos + _borderSize);
	glTexCoord2i(1, 1); glVertex2i(xPos + _borderSize + _width, yPos + _borderSize + _height);
	glTexCoord2i(0, 1); glVertex2i(xPos + _borderSize, yPos + _borderSize + _height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//объект для отображения
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_EQUAL, 1, 255);

	glColor4f(0.2, 0.5, 1, 1);
	glBegin(GL_QUADS);
	glVertex2i(xPos, yPos);
	glVertex2i(xPos + _width + _borderSize * 2, yPos);
	glVertex2i(xPos + _width + _borderSize * 2, yPos + _height + _borderSize * 2);
	glVertex2i(xPos, yPos + _height + _borderSize * 2);
	glEnd();

	glDisable(GL_STENCIL_TEST);*/







	/*
	//шикардос. идеально
	glAlphaFunc(GL_GEQUAL, 0.01); //рисуем только пиксель с альфой меньше 0.01
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_STENCIL_TEST);

	//рисуем только в этой области
	//что делать если не тест не пройдер; тест трафарета пройден, но тест глубины - нет; и тест трафарета и глубины - пройден
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//функция сравнения; подставляемое значение?; идентификатор маски?
	glStencilFunc(GL_ALWAYS, 1, 255); //сложно непонятно https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilFunc.xml

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE); //не рисуем в окно, но рисуем в трафарет?
	glColor4f(0, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos);
	glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos);
	glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos + _height);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos + _height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//не рисуем в этой области
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_NEVER, 2, 255);

	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilFunc(GL_ALWAYS, 1, 255);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	glColor4f(0, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos);
	glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos);
	glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//объект для отображения
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_EQUAL, 1, 255);

	glColor4f(0.2, 0.5, 1, 1);
	glBegin(GL_QUADS);
	glVertex2i(xPos, yPos);
	glVertex2i(xPos + _width, yPos);
	glVertex2i(xPos + _width, yPos + _height);
	glVertex2i(xPos, yPos + _height);
	glEnd();

	glDisable(GL_STENCIL_TEST);*/

	//работает ещё лучше.текстура видна только внутри пикселей первой ТЕКСТУРЫ с пороговой альфой, которая устанавливается. сама первая текстура невидимая
	/*glAlphaFunc(GL_GEQUAL, 0.01); //рисуем только пиксель с альфой меньше 0.01
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_STENCIL_TEST);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 255);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE); //не рисуем в окно, но рисуем в трафарет?
	glColor4f(0, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos + 50);
	glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos + 50);
	glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height + 50);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height + 50);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_EQUAL, 1, 255);

	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos);
	glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos);
	glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_STENCIL_TEST);*/

	//замечательно работает. текстура видна только внутри первой фигуры, сама фигура невидимая
	/*glEnable(GL_STENCIL_TEST);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilFunc(GL_ALWAYS, 1, 255);

	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glColor4f(0, 0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(30, 500);
	glVertex2f(0, 0);
	glVertex2f(60, 0);
	glEnd();
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//glStencilFunc(GL_EQUAL, 1, 0xFF); //0xFF = 255
	glStencilFunc(GL_EQUAL, 1, 255);

	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos);
	glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos);
	glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_STENCIL_TEST);*/

	/*glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	//glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_ALWAYS, 1, 1);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);

	//glUseProgram(program[PROGRAM_POINT].id);

	//glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos);
	glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos);
	glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_NEVER, 0, 1);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

	//glStencilFunc(GL_EQUAL, 1, 1);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//glBindTexture(GL_TEXTURE_2D, texture);
	//glUseProgram(program[PROGRAM_POINT].id);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(30, 100);
	glVertex2f(0, 0);
	glVertex2f(60, 0);
	glEnd();
	glDisable(GL_STENCIL_TEST);*/

	//работает
	/*glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 2, 1);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(60, 60);
	glVertex2f(20, 60);
	glVertex2f(20, 20);
	glVertex2f(60, 20);
	glEnd();

	glStencilFunc(GL_NEVER, 1, 1);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(30, 80);
	glVertex2f(10, 10);
	glVertex2f(50, 10);
	glEnd();

	glStencilFunc(GL_EQUAL, 2, 1);
	//glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(30, 100);
	glVertex2f(0, 0);
	glVertex2f(60, 0);
	glEnd();
	glDisable(GL_STENCIL_TEST);*/


	/*glEnable(GL_STENCIL_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureProperties.ID);

	glStencilFunc(GL_NEVER, 1, 1);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos);
	glTexCoord2i(1, 0); glVertex2i(xPos + _width, yPos);
	glTexCoord2i(1, 1); glVertex2i(xPos + _width, yPos + _height);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _height);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);

	glStencilFunc(GL_EQUAL, 2, 1);
	//glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(30, 100);
	glVertex2f(0, 0);
	glVertex2f(60, 0);
	glEnd();
	glDisable(GL_STENCIL_TEST);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_STENCIL_TEST);*/

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//}