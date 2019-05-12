//#include <windows.h>
#include <gl/glew.h>

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>

#include "textures.h"

#include "gameplay.h"

static ApplicationState applicationState = ApplicationState::BATTLE;

using namespace std;

//---------------------------------------------------------------------------------------------
//functions

void gameplayProcessing() {
	auto start = clock();

	switch (applicationState) {
	case ApplicationState::MAIN_MENU :
		//Something
		break;
	case ApplicationState::BATTLE :
		drawTexture(10, 10, TextureName::TEST2);
		drawTexture(200, 100, TextureName::TEST2);
		drawTexture(150, 400, TextureName::TEST2);
		drawTexture(0, 0, TextureName::TEST3);
		drawTexture(0, 350, TextureName::TEST4);

		for (int i = 400; i < 10400; i++) { //14-20 мс медиана - 15
			if (i % 2 == 0) drawTexture(i, i, TextureName::TEST2);
			else drawTexture(i, i, TextureName::TEST4);
		}

		break;
	case ApplicationState::LOAD_MENU :

		break;
	case ApplicationState::OPTIONS:

		break;
	}
	/*unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_6_5, NULL);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 640, 480);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo); //GL_FRAMEBUFFER_EXT
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//glReadBuffer(GL_COLOR_ATTACHMENT0); //_EXT

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		cout << "complete" << endl;
		//glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, textureProperties.ID);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureProperties.width, textureProperties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		drawTexture(0, 0, TEST2);
		//glRasterPos2i(0, 0);
		//glPixelZoom(1.0, -1.0);
		//glDrawPixels(textureProperties.width, textureProperties.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		vector<unsigned char> pixels(512 * 256 * 4);
		//GLubyte* img = (GLubyte*)malloc(textureProperties.width * textureProperties.height * sizeof(GLubyte) * 4);
		glReadPixels(0, 0, 512, 256, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
		if (!pixels.empty()) {
			for (int i = 0; i < 512 * 256 * 4; i++)
				if (pixels[i] != '\0') cout << "p: " << pixels[i];
			cout << endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &fbo);
	}

	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/

	//Sleep(500);

	//cout << "gameplayProcessing: " << clock() - start << endl; //250 кадров/сек - 4 миллисекунды на кадр, 60 фпс - 16,5 мс
}