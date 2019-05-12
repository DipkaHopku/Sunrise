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

#include "textures.h"

using namespace std;
//using namespace std::chrono;

std::ostream& operator << (std::ostream& os, const TextureName& obj)
{
	os << static_cast<std::underlying_type<TextureName>::type>(obj);
	return os;
}

//unordered_map<const TextureNames, const char* const> textureFilenames = { //нужна hash функция https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

//const map<const TextureNames, const char* const> textureFilenames = {
map<const TextureName, const string> textureFilenames = {
		{TextureName::TEST1,"textures/test.png"},
		{TextureName::TEST2,"textures/test2.png"},
		{TextureName::TEST3,"textures/test3.png"},
		{TextureName::TEST4,"textures/test2.png"}
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

map<TextureName, TextureProperties> textureData;

//---------------------------------------------------------------------------------------------
//functions
void loadTexture(TextureName textureName) {
	//try {
	//__try {
	TextureProperties textureProperties;
	//auto data = SOIL_load_image(textureFilenames.find(textureName)->second, &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	auto _data = SOIL_load_image(textureFilenames[textureName].c_str(), &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	if (_data != nullptr) {
		textureProperties.ID = SOIL_create_OGL_texture(_data, textureProperties.width, textureProperties.height, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

		bool** controlField = new bool*[textureProperties.width];
		for (int x = 0; x < textureProperties.width; x++) {
			controlField[x] = new bool[textureProperties.height];
			for (int y = 0; y < textureProperties.height; y++) {
				bool isFilledPixel = (int)_data[(x + y * textureProperties.width) * 4 + 3] != 0;
				controlField[x][y] = isFilledPixel;
			}
		}

		textureProperties.controlField = controlField;

		SOIL_free_image_data(_data);

		textureData.insert(pair<TextureName, TextureProperties>(textureName, textureProperties));
		
		/*if (textureName != TEST3) return;
		for (int y = 0; y < textureProperties.height; y++) {
			for (int x = 0; x < textureProperties.width; x++) {
				cout << (int)controlField[x][y];
			}
			cout << endl;
		}*/

		/*if (textureName == TEST3) {
			for (int i = 0; i < textureProperties.width * 4; i = i + 4) {
				//if ((int)data[i + 3] != 204) continue;
				cout << 'r' << (int)_data[i];
				cout << 'g' << (int)_data[i + 1];
				cout << 'b' << (int)_data[i + 2];
				cout << 'a' << (int)_data[i + 3];
				cout << ' ';
			}
			cout << endl;
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
	} else cout << "NullPointerException: texture '" << textureName << "' with filename '" << textureFilenames[textureName] << "' is not available" << endl;

	/*else throw exception("NullPointerException");
	}*/
	/*__except (filter(GetExceptionCode(), GetExceptionInformation())) {
		_exception_info
	}*/
	/*catch (exception &e) {
	//catch (...) {
		//cout << textureName << " fail with exception: " << e.what() << endl;
		cout << e.what() << ": texture '"<< textureName << "' with filename '" << textureFilenames[textureName] << "' is not available" << endl;
		//log("Exception thrown:  %s", e.what());
	}*/
}

void loadTextures() {
	for (const auto &textureFilename : textureFilenames) {
		loadTexture(textureFilename.first);

		/*if (textureFilename.first == TEST2) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			cout << "TEST2" << endl;
			// create a texture object
			GLuint textureId = textureData.find(textureFilename.first)->second.ID;
			//glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 600, 400, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			// create a renderbuffer object to store depth info
			GLuint rboId;
			glGenRenderbuffers(1, &rboId);
			glBindRenderbuffer(GL_RENDERBUFFER, rboId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 600, 400);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			// create a framebuffer object
			GLuint fboId;
			glGenFramebuffers(1, &fboId);
			glBindFramebuffer(GL_FRAMEBUFFER, fboId);

			// attach the texture to FBO color attachment point
			glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
				GL_COLOR_ATTACHMENT0,  // 2. attachment point
				GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
				textureId,             // 4. tex ID
				0);                    // 5. mipmap level: 0(base)

// attach the renderbuffer to depth attachment point
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
				GL_DEPTH_ATTACHMENT, // 2. attachment point
				GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
				rboId);              // 4. rbo ID

// check FBO status
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE)
				cout << "fboUsed = false;" << endl;

			// switch back to window-system-provided framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// set rendering destination to FBO
			glBindFramebuffer(GL_FRAMEBUFFER, fboId);

			// clear buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw a scene to a texture directly
			drawTexture(10, 10, TEST2);

			// unbind FBO
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}*/
	}
}

void drawTexture(int xPos, int yPos, TextureName textureName) {
	//auto start2 = clock();
	auto _textureData = textureData.find(textureName);
	if (_textureData != textureData.end()) {
		//TextureProperties textureProperties = textureData[textureName];
		TextureProperties _textureProperties = _textureData->second;
		glEnable(GL_TEXTURE_2D);
		//auto start = clock();
		glBindTexture(GL_TEXTURE_2D, _textureProperties.ID); //чем меньше колво айди - тем лучше
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
		glDisable(GL_TEXTURE_2D);
		//cout << "processing2: " << clock() - start2 << endl; //не более 1 миллисекунды
	} //else cout << "Error: texture '" << textureName << "' hasn`t properties" << endl;
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

/*int _loadTexture(TextureNames textureName) {
	//try {
	//__try {
	TextureProperties textureProperties;
	//auto data = SOIL_load_image(textureFilenames.find(textureName)->second, &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	auto data = SOIL_load_image(textureFilenames[TEST2].c_str(), &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	textureProperties.ID = SOIL_create_OGL_texture(data, textureProperties.width, textureProperties.height, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);
	SOIL_free_image_data(data);
	textureData.insert(pair<TextureNames, TextureProperties>(textureName, textureProperties));

	return textureProperties.ID;
}*/