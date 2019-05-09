//#include <windows.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <SOIL.h>
#include <iostream>
//#include <GL/glu.h>
//#include <Gdiplusheaders.h>

//#include "json.hpp"

#include "textures.h"

//#pragma comment(lib, "OpenGL32.lib")

using namespace std;

#define DEFAULT_SCREEN_WIDTH        640
#define DEFAULT_SCREEN_HEIGHT       480
#define DEFAULT_WINDOW_TITLE		"Sunrise"
#define ICON_FILENAME				"textures/sunrise.png"

void callback_windowSize(GLFWwindow*, int, int);
void callback_key(GLFWwindow*, int, int, int, int);
void callback_mouseButton(GLFWwindow*, int, int, int);

int main(void) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, DEFAULT_WINDOW_TITLE, NULL, NULL); //предпоследний это фуллскрин
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, callback_windowSize);
	glfwSetKeyCallback(window, callback_key);
	glfwSetMouseButtonCallback(window, callback_mouseButton);

	//int width, height;

	//иконка
	/*try {
		GLFWimage icons[1];
		icons[0].pixels = SOIL_load_image(ICON_FILENAME, &icons[0].width, &icons[0].height, NULL, SOIL_LOAD_AUTO);
		if (icons[0].pixels == nullptr) throw exception("NullPointerException");
		glfwSetWindowIcon(window, 1, icons);
	}
	catch (exception &e) {
		//просто без иконки
		cout << e.what() << ": icon with filename '" << ICON_FILENAME << "' is not available" << endl;
	}*/

	GLFWimage icons[1];
	icons[0].pixels = SOIL_load_image(ICON_FILENAME, &icons[0].width, &icons[0].height, NULL, SOIL_LOAD_AUTO);
	if (icons[0].pixels != nullptr) {
		glfwSetWindowIcon(window, 1, icons);
	} else cout << "NullPointerException: icon with filename '" << ICON_FILENAME << "' is not available" << endl;
	//cout << icons[0].width << '*' << icons[0].height << endl;

	glViewport(0, 0, (GLsizei)DEFAULT_SCREEN_WIDTH, (GLsizei)DEFAULT_SCREEN_HEIGHT); //TODO: двигать камеру или поле? наверное камеру. рисовать только те текстуры что находятся на экране
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //единичная матрица. одна координатная сетка на весь экран или как? комментирование ничего не меняет вроде бы
	glOrtho(0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, 0, -1); //положение сцены. начало кооридант - вверху слева, направление координатной сетки - вниз вправо
	//glOrtho(-100, 100, 100, -100, 0, -1);
	//glOrtho(0, DEFAULT_SCREEN_WIDTH, 0, DEFAULT_SCREEN_HEIGHT, 1, -1);

	/*float aspect = (float)DEFAULT_SCREEN_WIDTH / DEFAULT_SCREEN_HEIGHT;
	glOrtho(-aspect, aspect, -1, 1, -1, 1);*/
	//glfwSwapInterval(1); //TODO: UPS стабильный, FPS произвольный

	glEnable(GL_SMOOTH); //а нужно ли оно мне?
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //шта эта?
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_TEXTURE_2D); /текстуры сейчас не рисую

	//glPointSize(1); //точки не рисую

	//загружаем все текстуры
	loadTextures();

	/*GLuint text[1];
	//text[0] = SOIL_load_OGL_texture("textures/test2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);
	//glGetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint *params); //нужен опенгл 4.5+, а у меня почему то не 4.5
	int width, height;
	auto data = SOIL_load_image("textures/test2.png", &width, &height, NULL, SOIL_LOAD_AUTO);
	//if (data == NULL) cout << "NULL" << endl;
	text[0] = SOIL_create_OGL_texture(data, width, height, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO); //SOIL_FLAG_DDS_LOAD_DIRECT <-- загрузка без обраотки?
	//text[0] = SOIL_load_OGL_texture_from_memory(data, sizeof(data), 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO); - не работает*/

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //default

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 0);

		//хз_старт
		//рисовать тут
		drawTexture(10, 10, TextureNames::TEST2);
		drawTexture(200, 100, TextureNames::TEST2);
		drawTexture(150, 400, TextureNames::TEST2);
		drawTexture(0, 0, TextureNames::TEST3);
		//хз_энд

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void callback_windowSize(GLFWwindow* window, int width, int height) {
	/*GLint data[4];
	glGetIntegerv(GL_VIEWPORT, data); //glGet with argument GL_VIEWPORT*/
	//cout << ':' << data[0] << '%' << data[1] << '%' << data[2] << '%' << data[3] << '%' << endl;
	//glViewport(0, height - DEFAULT_SCREEN_HEIGHT, width, height);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, -1);

	/*
	//glScissor(0, 0, width, height);
	float aspect = (float)width / height;
	glOrtho(-aspect, aspect, height, 0, 0, -1);*/
	/*float ratio = (float)width / height;
	float aratio = (float)height / width;
	cout << "ratio: " << ratio << endl;
	//glOrtho(0, width * ratio, height, 0, 0, -1);
	//glOrtho(0, DEFAULT_SCREEN_HEIGHT * ratio, 0, DEFAULT_SCREEN_HEIGHT, 1, -1);
	//glOrtho(0, DEFAULT_SCREEN_HEIGHT * ratio, height, 0, 0, -1); //работает в ширину
	//glOrtho(0, DEFAULT_SCREEN_WIDTH * ratio, DEFAULT_SCREEN_HEIGHT * aratio, 0, 0, -1);
	//glViewport(0, height - DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	//https://github.com/LWJGL/lwjgl3-demos/blob/master/src/org/lwjgl/demo/opengl/SimpleDrawElements.java
	//https://stackoverflow.com/questions/3267243/in-opengl-how-can-i-adjust-for-the-window-being-resized
	//нужно было просто загрузить матрицу проекции и единичную матрицу
	*/

	/*glfwGetWindowSize(window, &scr_width, &scr_height);
	w_ratio = (float)scr_width / (float)SCREEN_WIDTH;
	h_ratio = (float)scr_height / (float)SCREEN_HEIGHT;*/
}

void callback_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void callback_mouseButton(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		cout << "GLFW_MOUSE_BUTTON_1:" << endl;

		double cursor_x, cursor_y;

		glfwGetCursorPos(window, &cursor_x, &cursor_y);

		cout << "cursor_x: " << cursor_x << "  cursor_y: " << cursor_y << endl;

		/*int ix = ((float)cursor_x / w_ratio);
		int iy = ((float)cursor_y / h_ratio);
		crear_square(ix, iy);*/
	}
}