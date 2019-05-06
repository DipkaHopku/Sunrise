//#include <windows.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <SOIL.h>
#include <iostream>
//#include <GL/glu.h>
//#include <Gdiplusheaders.h>

//#pragma comment(lib, "OpenGL32.lib")

using namespace std;

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       480

void resize_clb(GLFWwindow*, int, int);
void key_click_clb(GLFWwindow*, int, int, int, int);
void mouse_click_clb(GLFWwindow*, int, int, int);

void drawTexture(int, int, int, int, GLuint);

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunrise", NULL, NULL); //предпоследний это фуллскрин
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_click_clb);
	//glfwSetWindowSizeCallback(window, resize_clb);
	//glfwSetMouseButtonCallback(window, mouse_click_clb);

	//glfwSwapInterval(1);

	//GLFWimage images[1];
	
	//HRSRC myResource = FindResource(NULL, MAKEINTRESOURCE(IDI_ICON), RT_RCDATA);
	//HRSRC myResource = FindResource(NULL, LPCSTR(101), RT_RCDATA);
	//HANDLE load_image = LoadImage(NULL, LPCSTR("Sunrise.ico"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//Bitmap bitmap = Bitmap::FromHICON((LoadIcon(NULL, LPCTSTR("Sunrise"))));// ->GetHeight();
	//GetObject(hBitmap, sizeof(bitmap), (LPVoid)&bitmap);

	/*Bitmap iconBitmap("Sunrise.ico");

	HBITMAP Hbitmap;
	GetObject(Hbitmap, sizeof(iconBitmap), &iconBitmap);

	//images[0] = load_icon("my_icon.png");
	SOIL_load_image
	images[0] = GLFWimage(10, 10, (LPSTR)"fgdfg");
	//images[1] = load_icon("my_icon_small.png");
	glfwSetWindowIcon(window, 2, images);*/

	glViewport(0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT); //TODO: двигать камеру или поле? наверное камеру. рисовать только те текстуры что находятся на экране
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //единичная матрица. одна координатная сетка на весь экран или как?
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1); //что это?
	glfwSwapInterval(1); //TODO: UPS стабильный, FPS произвольный

	glEnable(GL_SMOOTH); //а нужно ли оно мне?
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //шта эта?
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_TEXTURE_2D); /текстуры сейчас не рисую

	//glPointSize(1); //точки не рисую

	GLuint text[1];
	//text[0] = SOIL_load_OGL_texture("textures/test2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);
	//glGetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint *params); //нужен опенгл 4.5+, а у меня почему то не 4.5
	int width;
	int height;
	auto data = SOIL_load_image("textures/test2.png", &width, &height, NULL, SOIL_LOAD_AUTO);
	cout << width << '-' << height << endl;
	text[0] = SOIL_create_OGL_texture(data, width, height, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO); //SOIL_FLAG_DDS_LOAD_DIRECT <-- загрузка без обраотки?
	//text[0] = SOIL_load_OGL_texture_from_memory(data, sizeof(data), 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO); - не работает

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //default

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 0);

		//хз_старт
		//рисовать тут
		drawTexture(0, 0, 512, 256, text[0]);
		//хз_энд

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_click_clb(GLFWwindow *pWindow, int aKey, int aScanCode, int aAction, int aMods)
{
	if (aKey == GLFW_KEY_ESCAPE && aAction == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

void drawTexture(int xPos, int yPos, int textureWidth, int textureHeight, GLuint textureID)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(xPos, yPos); //i - это int
	glTexCoord2i(1, 0); glVertex2i(xPos + textureWidth, yPos);
	glTexCoord2i(1, 1); glVertex2i(xPos + textureWidth, yPos + textureHeight);
	glTexCoord2i(0, 1); glVertex2i(xPos, yPos + textureHeight);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}