#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/gl.h>
#include <iostream>
//#include <vector>
#include <SOIL.h>
//#include <GL/glu.h>

//#include "json.hpp"

#include "gameplay.h"
#include "textures.h"
#include "userInterface.h"

using namespace std;

#define DEFAULT_SCREEN_WIDTH        640
#define DEFAULT_SCREEN_HEIGHT       480
#define DEFAULT_WINDOW_TITLE		"Sunrise"
#define ICON_FILENAME				"textures/sunrise.png"

int main(void) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/*if (glewInit() != GLEW_OK) {
		return -1;
	}*/

	//Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, DEFAULT_WINDOW_TITLE, NULL, NULL); //������������� ��� ���������
	if (!window) {
		glfwTerminate();
		return -1;
	}

	//Make the window's context current
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, callback_windowSize);
	glfwSetKeyCallback(window, callback_key);
	glfwSetMouseButtonCallback(window, callback_mouseButton);

	if (glewInit() != GLEW_OK) {
		return -1;
	}

	//������
	GLFWimage icons[1];
	icons[0].pixels = SOIL_load_image(ICON_FILENAME, &icons[0].width, &icons[0].height, NULL, SOIL_LOAD_AUTO);
	if (icons[0].pixels != nullptr) {
		glfwSetWindowIcon(window, 1, icons);
		SOIL_free_image_data(icons[0].pixels);
	} else cout << "NullPointerException: icon with filename '" << ICON_FILENAME << "' is not available" << endl; //������ ��� ������
	//cout << icons[0].width << '*' << icons[0].height << endl;

	glViewport(0, 0, (GLsizei)DEFAULT_SCREEN_WIDTH, (GLsizei)DEFAULT_SCREEN_HEIGHT); //TODO: ������� ������ ��� ����? �������� ������. �������� ������ �� �������� ��� ��������� �� ������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //��������� �������. ���� ������������ ����� �� ���� ����� ��� ���? ��������������� ������ �� ������ ����� ��
	glOrtho(0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, 0, -1); //��������� �����. ������ ��������� - ������ �����, ����������� ������������ ����� - ���� ������

	//glfwSwapInterval(1); //TODO: UPS ����������, FPS ������������

	glEnable(GL_SMOOTH); //� ����� �� ��� ���?
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //��� ���?
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_TEXTURE_2D); /�������� ������ �� �����

	//glPointSize(1); //����� �� �����

	//��������� ��� ��������
	loadTextures();

	ControlField::Instance().setWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

	//Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		//Render here
		glClear(GL_COLOR_BUFFER_BIT); //default
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 0);
		
		//��������� �������� � ��������� ������� � ����������
		gameplayProcessing();

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents(); //������� ��������� ���������� � ������� � ������. ������� �������������� �� ����� ������ �� ������. glfwPollEvents - ������������ �������
	}

	glfwTerminate();
	return 0;
}