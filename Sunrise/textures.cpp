//#include <windows.h>
//#include <GLFW/glfw3.h>
//#include <GLAPI // and GLAPIENTRY
//#define GLAPI extern
#include <GL/glew.h>
#include <GL/gl.h>
#include <SOIL.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <map>

/*#include <stdio.h>
#include <windows.h>
#include <excpt.h>*/

#include <csignal>

#include "textures.h"

using namespace std;

//unordered_map<const TextureNames, const char* const> textureFilenames = { //нужна hash функция https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

//const map<const TextureNames, const char* const> textureFilenames = {
map<const TextureNames, const string> textureFilenames = {
		{TextureNames::TEST2,"textures/test2.png"},
		{TextureNames::TEST3,"textures/test3.png"},
		{TextureNames::TEST4,"textures/test4.png"}
};

struct TextureProperties {
	GLuint ID;
	int width;
	int height;
};

map<TextureNames, TextureProperties> textureData;

void loadTexture(TextureNames textureName) {
	//try {
	//__try {
	TextureProperties textureProperties;
	//auto data = SOIL_load_image(textureFilenames.find(textureName)->second, &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	auto data = SOIL_load_image(textureFilenames[textureName].c_str(), &textureProperties.width, &textureProperties.height, NULL, SOIL_LOAD_AUTO);
	if (data != nullptr) {
		textureProperties.ID = SOIL_create_OGL_texture(data, textureProperties.width, textureProperties.height, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

		textureData.insert(pair<TextureNames, TextureProperties>(textureName, textureProperties));
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
	}
}

void drawTexture(int xPos, int yPos, TextureNames textureName) {
	auto temp = textureData.find(textureName);
	if (temp != textureData.end()) {
		//TextureProperties textureProperties = textureData[textureName];
		TextureProperties textureProperties = temp->second;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureProperties.ID);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(xPos, yPos); //i - это int
		glTexCoord2i(1, 0); glVertex2i(xPos + textureProperties.width, yPos);
		glTexCoord2i(1, 1); glVertex2i(xPos + textureProperties.width, yPos + textureProperties.height);
		glTexCoord2i(0, 1); glVertex2i(xPos, yPos + textureProperties.height);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	} //else cout << "Error: texture '" << textureName << "' hasn`t properties" << endl;
}