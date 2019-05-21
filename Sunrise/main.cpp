#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

//#include <windows.h>
//#include <dos.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/gl.h>
#include <SOIL.h>

#include <iostream>
//#include <vector>
#include <thread>
#include <chrono>
//#include <cstring>

//#include <GL/glu.h>

//#include "json.hpp"

#include "gameplay.h"
#include "textures.h"
#include "fonts.h"
#include "userInterface.h"

using namespace std;

#define MM_IN_ONE_INCH        25.4

#define DEFAULT_SCREEN_WIDTH        640
#define DEFAULT_SCREEN_HEIGHT       480
#define DEFAULT_WINDOW_TITLE		"Sunrise"
#define ICON_FILENAME				"resources/textures/sunrise.png"
#define MAX_FPS 120

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

	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE)
	//glfwSetWindowSizeLimits()
	//glfwGetWindo
	//glfwWindowHint(GLFW_MIN_WINDOW_WIDTH, 640);
	//glfwWindowHint(GLFW_MIN_WINDOW_HEIGHT, 480);

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
	//double w2h = (DEFAULT_SCREEN_HEIGHT > 0) ? (double)DEFAULT_SCREEN_WIDTH / DEFAULT_SCREEN_HEIGHT : 1;
	//glOrtho(0, 640 * w2h, 0, 480, -2, +2);
	//glOrtho(0, 480 * w2h, 0, 480, -2, 2);

	//�� ������� 1 //��� 2 - 30 ���, ��� 1 - 60
	//������ ������������� ��������, �.�. ����� ��������� �������� ������������ - �������� ����� ����� ����� ��
	//TODO: UPS ����������, FPS ������������
	glfwSwapInterval(0);

	glEnable(GL_SMOOTH); //� ����� �� ��� ���?
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //"����������" > ��������� ������������
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);//���� �������� ���������, �� ������ ����� ����� ��������� ���� � �����

	/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//����� ���������� ��������� �� ������� 0-1
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);*/

	/*glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);*/

	//glEnable(GL_TEXTURE_2D); /�������� ������ �� �����

	//glPointSize(1); //����� �� �����

	//��������� ��� ��������
	loadTextures();

	//������������� ���������� ��������
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	ControlField::Instance().setDisplaySize(mode->width, mode->height);

	//������������� ����������� ������ ����
	glfwSetWindowSizeLimits(window, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, mode->width, mode->height);

	//��������� ���. �������� ��������
	int displayWidth_mm;// , height_mm;
	glfwGetMonitorPhysicalSize(monitor, &displayWidth_mm, NULL);

	//��������� ������
	freeTypeInit(mode->width * MM_IN_ONE_INCH / displayWidth_mm);
	
	//������������� ������ ���� �� ������ ������ ����������
	ControlField::Instance().setWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

	//��������� FPS �.0
	int fpsControl_startFrame;
	//int lastFrame = 0;
	float fpsControl_frameTime;
	float fpsControl_test_sleepTime;
	float fpsControl_totalSeconds = 0;
	float fpsControl_totalFrames = 0;

	//Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		//��������� FPS �.1
		fpsControl_startFrame = clock();
		if (fpsControl_totalSeconds > 1.f) {
			cout << "FPS: " << fpsControl_totalFrames; //250 ������/��� - 4 ������������ �� ����, 60 ��� - 16,5 ��
			cout << " AvFrameTime: " << fpsControl_totalSeconds / fpsControl_totalFrames << endl << endl;
			fpsControl_totalSeconds = 0;
			fpsControl_totalFrames = 0;
		}

		/*int clock_temp = clock();
		//cout << "clock " << clock_temp << endl;
		float sleeping = (float)(clock_temp - lastFrame) / CLOCKS_PER_SEC;
		//cout << sleeping << endl;
		if (sleeping < 0.015) {
			//cout << "scip" << endl;
			test_frameTime = (float)(clock() - startFrame) / CLOCKS_PER_SEC;
			test_totalSeconds += test_frameTime;
			continue;
		}*/

		//lastFrame = clock();

		/*for (int i = 0; i < 10; i++) {
			int first_clock = clock();
			int first_time = time(NULL);

			while (time(NULL) <= first_time) {}

			int second_time = time(NULL);
			int second_clock = clock();

			cout << "Actual clocks per second = " << (second_clock - first_clock) / (second_time - first_time) << "\n";

			cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << "\n";
		}*/

		//auto test_startPreparation = clock();

		//Render here
		glClear(GL_COLOR_BUFFER_BIT); //default
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_STENCIL_BUFFER_BIT
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 0);

		//cout << "Time of preparatio: " << (float)(clock() - test_startPreparation) / CLOCKS_PER_SEC << endl;
		
		//��������� FPS �.2.2.1
		//auto test_startGameplayProcessing = clock();
		
		//��������� �������� � ��������� ������� � ����������
		gameplayProcessing();
		
		//��������� FPS �.2.2.2
		//float test_timeGameplayProcessing = (float)(clock() - test_startGameplayProcessing) / CLOCKS_PER_SEC;
		//cout << "Time of gameplayProcessing: " << test_timeGameplayProcessing << endl;

		//auto test_startSwapBuffers = clock();

		//Swap front and back buffers
		glfwSwapBuffers(window); //������ ����� 16 ����������, ���� swapBuffer = 1!
		//cout << "Time of swap buffers: " << (float)(clock() - test_startSwapBuffers) / CLOCKS_PER_SEC << endl;

		//auto test_startPollEvents = clock();

		//Poll for and process events
		glfwPollEvents(); //������� ��������� ���������� � ������� � ������. ������� �������������� �� ����� ������ �� ������. glfwPollEvents - ������������ �������

		//cout << "Time of poll events: " << (float)(clock() - test_startPollEvents) / CLOCKS_PER_SEC << endl;

		//��������� FPS �.2
		fpsControl_frameTime = (float)(clock() - fpsControl_startFrame) / CLOCKS_PER_SEC;
		fpsControl_totalSeconds += fpsControl_frameTime;
		fpsControl_totalFrames++;
		//cout << "Time of frame: " << test_frameTime << endl << endl;

		//test_sleepTime = 0;
		fpsControl_test_sleepTime = 1.f / MAX_FPS - fpsControl_frameTime; //������� ���� ����� ������, ����� ���� ��� �� ������ ����������� ����� �������
		if (fpsControl_test_sleepTime > 0) {
			Sleep(fpsControl_test_sleepTime * 1000);
			//system("sleep");
			//std::this_thread::sleep_for(std::chrono::microseconds((int) test_sleepTime * 1000000));
			//std::this_thread::sleep_for(std::chrono::milliseconds((int) test_sleepTime * 1000));
			fpsControl_totalSeconds += fpsControl_test_sleepTime;
		}
	}

	glfwTerminate();
	return 0;
}








//���� ��������� ������, ������� � ���� �� ������. http://www.hardforum.ru/t81129/
//glOrtho(0, DEFAULT_SCREEN_WIDTH, 0, DEFAULT_SCREEN_HEIGHT, -1, 1);
/*//��������� � �����
GLfloat white[3] = { 1.0,1.0,1.0 };

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3fv(white);
		glRasterPos2i(20, 60);
		printString("THE QUICK BROWN FOX JUMPS");
		glRasterPos2i(20, 40);
		printString("OVER A LAZY DOG");
		glFlush();*/
/*GLubyte space[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,0x00,0x00,0x00 };
GLubyte letters[][13] = {
{0x00,0x00,0xc3,0xc3,0xc3,0xc3,0xff,0xc3,0xc3,0xc3 ,0x66,0x3c,0x18},
{0x00,0x00,0xfe,0xc7,0xc3,0xc3,0xc7,0xfe,0xc7,0xc3 ,0xc3,0xc7,0xfe},
{0x00,0x00,0x7e,0x7e,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0 ,0xc0,0xe7,0x7e},
{0x00,0x00,0xfc,0xce,0xc7,0xc3,0xc3,0xc3,0xc3,0xc3 ,0xc7,0xce,0xfc},
{0x00,0x00,0xff,0xc0,0xc0,0xc0,0xc0,0xfc,0xc0,0xc0 ,0xc0,0xc0,0xff},
{0x00,0x00,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xfc,0xc0 ,0xc0,0xc0,0xff},
{0x00,0x00,0x7e,0x7e,0xc3,0xc3,0xcf,0xc0,0xc0,0xc0 ,0xc0,0xe7,0x7e},
{0x00,0x00,0xc3,0xc3,0xc3,0xc3,0xc3,0xff,0xc3,0xc3 ,0xc3,0xc3,0xc3},
{0x00,0x00,0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x18 ,0x18,0x18,0x7e},
{0x00,0x00,0x7c,0xee,0xc6,0x06,0x06,0x06,0x06,0x06 ,0x06,0x06,0x06},
{0x00,0x00,0xc3,0xc6,0xcc,0xd8,0xf0,0xe0,0xf0,0xd8 ,0xcc,0xc6,0xc3},
{0x00,0x00,0xff,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0 ,0xc0,0xc0,0xc0},
{0x00,0x00,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xdb,0xff ,0xff,0xe7,0xc3},
{0x00,0x00,0xc7,0xc7,0xcf,0xcf,0xdf,0xdb,0xfb,0xf3 ,0xf3,0xe3,0xe3},
{0x00,0x00,0x7e,0xe7,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3 ,0xc3,0xe7,0x7e},
{0x00,0x00,0xc0,0xc0,0xc0,0xc0,0xc0,0xfe,0xc7,0xc3 ,0xc3,0xc7,0xfe},
{0x00,0x00,0x3f,0x6e,0xdf,0xdb,0xc3,0xc3,0xc3,0xc3 ,0xc3,0x66,0x3c},
{0x00,0x00,0xc3,0xc6,0xcc,0xd8,0xf0,0xfe,0xc7,0xc3 ,0xc3,0xc7,0xfe},
{0x00,0x00,0x7e,0xe7,0x03,0x03,0x07,0x7e,0xe0,0xc0 ,0xc0,0xe7,0x7e},
{0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 ,0x18,0x18,0xff},
{0x00,0x00,0x7e,0xe7,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3 ,0xc3,0xc3,0xc3},
{0x00,0x00,0x18,0x3c,0x3c,0x66,0x66,0xc3,0xc3,0xc3 ,0xc3,0xc3,0xc3},
{0x00,0x00,0xc3,0xe7,0xff,0xff,0xdb,0xdb,0xc3,0xc3 ,0xc3,0xc3,0xc3},
{0x00,0x00,0xc3,0x66,0x66,0x3c,0x3c,0x18,0x3c,0x3c ,0x66,0x66,0xc3},
{0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x3c ,0x66,0x66,0xc3},
{0x00,0x00,0xff,0xc0,0xc0,0x60,0x30,0x7e,0x0c,0x06 ,0x03,0x03,0xff}
};
GLuint fontOffset;

void makeRasterFont()
{
	GLuint i, j;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	fontOffset = glGenLists(128);
	for (i = 0, j = 'A'; i < 26; i++, j++)
	{
		glNewList(fontOffset + j, GL_COMPILE);
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, letters[i]);
		glEndList();
	}
	glNewList(fontOffset + ' ', GL_COMPILE);
	glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, space);
	glEndList();
}

void init(void)
{
	glShadeModel(GL_FLAT);
	makeRasterFont();
}

void printString(const char *s)
{
	glPushAttrib(GL_LIST_BIT);
	glListBase(fontOffset);
	glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte*)s);
	glPopAttrib();
}*/