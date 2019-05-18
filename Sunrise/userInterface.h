#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

//#include <cstdlib>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "fonts.h"

using std::map;
using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

//---------------------------------------------------------------------------------------------
//global variables and types

#define MAIN_MENU_TITLE "Sunrise"

const float buttonColorRGB[3] = {180.f/255, 0.f/255, 255.f/255};

enum class ButtonType {
	START, //СОЗДАНИЕ ПОЛЯ, РАССТАНОВКА ПЕРСОВ
	LOAD,
	OPTIONS,
	EXIT
};

enum class Orientation {
	VERTICAL,
	HORIZONTAL
};

enum class VerticalPosition {
	TOP,
	MIDDLE,
	BOTTOM
};

enum class HorizontalPosition {
	LEFT,
	MIDDLE,
	RIGHT
};

class ButtonProperties {
/*public:
	string title;*/
private:
	string _title;

public:
	void(*callback)();

	ButtonProperties(string, void(*)());

	string getTitle() const;
};

class UserInterfaceItem;

//MenuItemContainer* menuItemContainer;

//---------------------------------------------------------------------------------------------
//class ControlField declaration

class ControlField {
private:
	int _displayWidth = 0;
	int _displayHeight = 0;
	UserInterfaceItem*** _controlField = nullptr;
	int _windowWidth = 0;
	int _windowHeight = 0;

	ControlField();
	~ControlField();
	ControlField(const ControlField&) = delete;
	ControlField& operator=(const ControlField&) = delete;
public:
	static ControlField& Instance();

	//установка размера дисплея в пикселях. Функция вызывается только в момент запуска приложения
	void setDisplaySize(int, int);

	//установка текущего размера окна приложения
	void setWindowSize(int, int);

	//получение текущего размера окна приложения
	//arguments:
	//#1 - width of window
	//#2 - height of window
	void getWindowSize(int*, int*) const;

	//подготовка к отрисовке следующего кадра. результат - нажатие кнопок мыши не провоцирует начала выполнения функций активных элементов интерфейса. на новом кадре положения элементов интрефейсов может меняться
	void clearControlField();
};

//---------------------------------------------------------------------------------------------
//class UserInterfaceItem declaration

class UserInterfaceItem {
protected:
	int _xPos, _yPos;
	int _width, _height;
	int _paddingSize;

public:
	virtual void drawUIElement() = 0; // const = 0; TODO
	virtual void onClick() const = 0;
};

//---------------------------------------------------------------------------------------------
//class MenuItemContainer declaration

class MenuItemContainer : public UserInterfaceItem {
private:
	Orientation _orientation;
	VerticalPosition _verticalPosition;
	HorizontalPosition _horizontalPosition;

	vector<UserInterfaceItem*> menuItems;

public:
	MenuItemContainer(Orientation, VerticalPosition, HorizontalPosition) noexcept;
	~MenuItemContainer();

	//void drawUIElement() const;
	void drawUIElement();
	void onClick() const;

	void addMenuItem(UserInterfaceItem* const menuItem);
};

//---------------------------------------------------------------------------------------------
//class Label declaration

class Label : public UserInterfaceItem {
private:
	string _title;
	int _textSize;
public:
	Label(string, int) noexcept;
	~Label();

	//void drawUIElement() const;
	void drawUIElement();
	void onClick() const;
};

//---------------------------------------------------------------------------------------------
//class Button declaration

class Button : public UserInterfaceItem {
private:
	string _title;
	void(*_callback)();
	int _marginSize;
	int _textSize;
public:

	/*
	arguments:
	#1 - ButtonProperties class with title and callback
	#3 - textSize
	#4 - marginSize
	#5 - paddingSize
	*/
	Button(ButtonProperties, int, int, int) noexcept;
	~Button();

	//void drawUIElement() const;
	void drawUIElement();
	void onClick() const;
};

//---------------------------------------------------------------------------------------------
//fuctions declaration

MenuItemContainer* createMainMenu();

//---------------------------------------------------------------------------------------------
//callbacks declaration

void callback_windowSize(GLFWwindow*, int, int);
void callback_key(GLFWwindow*, int, int, int, int);
void callback_mouseButton(GLFWwindow*, int, int, int);
