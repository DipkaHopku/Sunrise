//#include <cstdlib>

#include "userInterface.h"

//bool** _controlField = nullptr;

//---------------------------------------------------------------------------------------------
//class ControlField definition

ControlField::ControlField() = default;
ControlField::~ControlField() = default;

ControlField& ControlField::Instance()
{
	static ControlField singleton;
	return singleton;
}

void ControlField::setWindowSize(int width, int height) {
	if (_controlField != nullptr) { //TODO небольшая утечка памяти до 8мб. возможно она даже не моя. зависит от величины экрана. если открывать сразу большой экран, то утечки нет.
		for (int x = 0; x < _width; x++) {
			delete[] _controlField[x];
		}
		delete[] _controlField;
		_controlField = nullptr;
	}

	_controlField = new bool*[width];
	for (int x = 0; x < width; x++) {
		_controlField[x] = new bool[height];
	}
	_width = width;
	_height = height;
}

//---------------------------------------------------------------------------------------------
//class MenuItemContainer definition

MenuItemContainer::MenuItemContainer() noexcept {
}

MenuItemContainer::~MenuItemContainer() {
	/*for (int i = 0; i < menuItems.size(); i++) {
		delete menuItems[i];
	}*/
}

void MenuItemContainer::addMenuItem(UserInterfaceItem* menuItem) {
	menuItems.push_back(menuItem);
}

//---------------------------------------------------------------------------------------------
//functions

void drawMainMenu() {

}

//---------------------------------------------------------------------------------------------
//callbacks

void callback_windowSize(GLFWwindow* window, int width, int height) {
	/*GLint data[4];
	glGetIntegerv(GL_VIEWPORT, data);*/

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, -1);

	ControlField::Instance().setWindowSize(width, height);

	//MenuItemContainer.setWindowSize(width, height);

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
		double cursor_x, cursor_y;
		glfwGetCursorPos(window, &cursor_x, &cursor_y);

		//cout << "GLFW_MOUSE_BUTTON_1:" << endl;
		cout << "cursor_x: " << cursor_x << "  cursor_y: " << cursor_y << endl;

		/*int ix = ((float)cursor_x / w_ratio);
		int iy = ((float)cursor_y / h_ratio);
		crear_square(ix, iy);*/
	}
}