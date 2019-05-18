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
	{TextureName::TEST1, "resources/textures/test.png"},
	{TextureName::TEST2, "resources/textures/test2.png"},
	{TextureName::TEST3, "resources/textures/test3.png"},
	{TextureName::WTF_CAT, "resources/textures/wtf_cat_test.png"}
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

map<TextureName, TextureProperties> texturesData;

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
	} else cout << "NullPointerException: texture '" << textureName << "' with filename '" << textureFilenames.at(textureName) << "' is not available" << endl;
}

void loadTextures() {
	for (const auto &textureFilename : textureFilenames) {
		loadTexture(textureFilename.first);
	}
}

void drawTexture(const int xPos, const int yPos, const TextureName textureName) {
	//auto start2 = clock();
	auto _textureData = texturesData.find(textureName);
	if (_textureData != texturesData.end()) {
		//TextureProperties textureProperties = textureData[textureName];
		TextureProperties _textureProperties = _textureData->second;

		glColor4f(1, 1, 1, 1); //чтобы текстура имела натуральный цвет

		glEnable(GL_TEXTURE_2D);
		//auto start = clock();

		glBindTexture(GL_TEXTURE_2D, _textureProperties.ID); //чем меньше колво айди - тем лучше

		/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//выход текстурных координат за пределы 0-1
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //сглаживание при уменьшении
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //сглаживание при увеличении //NEAREST - по ближайшему пикселю*/

		glBegin(GL_QUADS); //устаревший интерфейс, НО время выполнения функции всё ещё менее 1 миллисекунды
		glTexCoord2i(0, 0); glVertex2i(xPos, yPos); //i - это int
		glTexCoord2i(1, 0); glVertex2i(xPos + _textureProperties.width, yPos);
		glTexCoord2i(1, 1); glVertex2i(xPos + _textureProperties.width, yPos + _textureProperties.height);
		glTexCoord2i(0, 1); glVertex2i(xPos, yPos + _textureProperties.height);
		/*//МОЖЕТ БЫТЬ! рисовать отдельные объекты из одной мегатекстуры быстрее, тк МОЖЕТ БЫТЬ операция glBindTexture() - очень дорогая //протестил. если уменьшение есть, то почти незаметное (<< 10%)
		glTexCoord2f(0.5f, 0); glVertex2i(xPos, yPos); //i - это int
		glTexCoord2f(1, 0); glVertex2i(xPos + textureProperties.width/2, yPos);
		glTexCoord2f(1, 1); glVertex2i(xPos + textureProperties.width/2, yPos + textureProperties.height);
		glTexCoord2f(0.5f, 1); glVertex2i(xPos, yPos + textureProperties.height);*/
		glEnd();
		//cout << "processing: " << clock() - start << endl; //не более 1 миллисекунды

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		//cout << "processing2: " << clock() - start2 << endl; //не более 1 миллисекунды
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
}

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