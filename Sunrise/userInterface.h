#pragma once

#include <GLFW/glfw3.h>
//#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

enum class ButtonType {
	START, //—Œ«ƒ¿Õ»≈ œŒÀﬂ, –¿——“¿ÕŒ¬ ¿ œ≈–—Œ¬
	LOAD,
	OPTIONS,
	EXIT
};

//---------------------------------------------------------------------------------------------
//class ControlField definition

class ControlField {
private:
	bool** _controlField = nullptr;
	int _width = 0;
	int _height = 0;

	ControlField();
	~ControlField();
	ControlField(const ControlField&) = delete;
	ControlField& operator=(const ControlField&) = delete;
public:
	static ControlField& Instance();

	void setWindowSize(int, int);
};

//MenuItemContainer* menuItemContainer;

//---------------------------------------------------------------------------------------------
//class UserInterfaceItem definition

class UserInterfaceItem {
private:
	int xPos, yPos;
	int width, height;
	
public:
	virtual void drawUIElement() = 0;
	virtual void onClick() = 0;
};

class MenuItemContainer {
private:
	vector<UserInterfaceItem*> menuItems;

public:
	MenuItemContainer() noexcept;
	~MenuItemContainer();

	void addMenuItem(UserInterfaceItem* menuItem);
};

class Label : UserInterfaceItem {

};

class Button : UserInterfaceItem {
private:
	ButtonType _buttonType;
public:
	void drawUIElement() {

	}
};

void callback_windowSize(GLFWwindow*, int, int);
void callback_key(GLFWwindow*, int, int, int, int);
void callback_mouseButton(GLFWwindow*, int, int, int);
