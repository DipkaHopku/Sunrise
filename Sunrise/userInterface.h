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
#include "gameplay.h"

using std::map;
using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

//---------------------------------------------------------------------------------------------
//global variables and types

#define MAIN_MENU_TITLE "Sunrise"

const float BUTTON_COLOR_RGB[4] = { 0.f / 255, 0.f / 255, 0.f / 255, 255.f / 255 };
//const float BUTTON_COLOR_RGB[4] = {0.f/255, 0.f/255, 0.f/255, 63.f/255};
//const float BUTTON_COLOR_RGB[4] = { 0.f / 255, 0.f / 255, 0.f / 255, 0.f / 255 };

enum class ApplicationState {
	APPLICATION_LAUNCH,
	MAIN_MENU,
	BATTLE,
	LOAD_MENU,
	OPTIONS
};

enum class ButtonType {
	START,
	LOAD,
	OPTIONS,
	EXIT,
	BACK,
	APPLY
};

enum class CheckBoxType {
	FULLSCREEN
};

enum class LabelType {
	TITLE,
	OPTIONS,
	FULLSCREEN
};

enum class Orientation {
	VERTICAL,
	HORIZONTAL
};

enum class VerticalAlign {
	TOP,
	MIDDLE,
	BOTTOM
};

enum class HorizontalAlign {
	LEFT,
	MIDDLE,
	RIGHT
};

/*class ButtonProperties { //TODO мб лучше спрятать
//public:
	//string title;
private:
	string _title;
	FontName fontName;

public:
	void(*callback)();

	ButtonProperties(string, void(*)());

	string getTitle() const;
};*/

class UserInterface;

class UIItem;

class ActiveGraphicItem;

class UIItemsContainer;

//MenuItemContainer* menuItemContainer;



//---------------------------------------------------------------------------------------------
//class ApplicationStateController declaration

static class ApplicationStateChangingController { //ApplicationStateCurrentSettingController
	friend void gameplayProcessing();
	
private:
	static void setApplicationState(ApplicationState);	
};



//---------------------------------------------------------------------------------------------
//class ApplicationStatePlanningController declaration

static class ApplicationStatePlanningController {
	friend void gameplayProcessing();
	friend void callback_startButton_onClick();
	friend void callback_optionsButton_onClick();
	friend void callback_backButton_onClick();
	friend void callback_exitToMainMenuButton_onClick();
	
	friend void callback_key(GLFWwindow*, const int, const int, const int, const int);
	friend void callback_mouseButton(GLFWwindow*, const int, const int, const int);
	friend void callback_cursorEnter(GLFWwindow*, int);
	friend void callback_cursorPos(GLFWwindow*, double, double);

private:
	static ApplicationState getApplicationState();

	static void setApplicationStateNext(ApplicationState);

	static ApplicationState getApplicationStateNext();
};



//---------------------------------------------------------------------------------------------
//class UserInterface declaration

class UserInterface {
	friend class ApplicationStateChangingController;
	friend class ApplicationStatePlanningController;

private:
	ApplicationState _applicationState = ApplicationState::APPLICATION_LAUNCH;
	ApplicationState _applicationStateNext = ApplicationState::APPLICATION_LAUNCH;

	int _displayWidth = 0;
	int _displayHeight = 0;
	ActiveGraphicItem*** _controlField = nullptr;
	int _windowWidth = 0;
	int _windowHeight = 0;

	int _xCursorPos = -1;
	int _yCursorPos = -1;

	Orientation _orientation = Orientation::VERTICAL;
	HorizontalAlign _horizontalAlign = HorizontalAlign::MIDDLE;
	VerticalAlign _verticalAlign = VerticalAlign::MIDDLE;

	UIItemsContainer* _UIItemsContainer = nullptr;
	//int _windowWidth = 0;
	//int _windowHeight = 0;

	UserInterface();
	~UserInterface();
	UserInterface(const UserInterface&) = delete;
	UserInterface& operator=(const UserInterface&) = delete;

	void updateContainerProperties();

	//bool setApplicationState(ApplicationState);
	void setApplicationState(ApplicationState);

	ApplicationState getApplicationState() const;

	//возвращает true, если установить следующее состояние удалось, и false, если не удалось
	bool setApplicationStateNext(ApplicationState);

	ApplicationState getApplicationStateNext() const;
public:
	static UserInterface& Instance();	

	//установка размера дисплея в пикселях. Функция вызывается только в момент запуска приложения
	void setDisplaySize(int, int);

	//установка текущего размера окна приложения
	void setWindowSize(int, int);

	//получение текущего размера окна приложения
	//arguments:
	//#1 - width of window
	//#2 - height of window
	void getWindowSize(int*, int*) const;

	//установка активной зоны графического объекта на экране приложения
	//arguments:
	//#1 - xPos of active graphic item left side
	//#2 - width of active graphic item
	//#3 - yPos of active graphic item top side
	//#4 - height of active graphic item
	//#5 - active graphic item pointer
	void setActiveGraphicItemControlField(int, int, int, int, ActiveGraphicItem*);

	void triggerActiveGraphicItemOnClickCallbackAtPoint(int, int);

	void setCursorPos(int, int);

	void getCursorPos(int*, int*);

	//подготовка к отрисовке следующего кадра. результат - нажатие кнопок мыши не провоцирует начала выполнения функций активных элементов интерфейса. на новом кадре положения элементов интрефейсов может меняться
	void clearControlField();

	//debug
	//void __drawControlField();

	void setUIItemsContainer(UIItemsContainer*);

	//void updateContainerProperties(); //скрыт

	void drawUserInterface() const;

	void createDialogWindow() const;
};



//---------------------------------------------------------------------------------------------
//class GraphicItem declaration

class GraphicItem { //(visualObject/visualItem/graphicItem
protected:
	int _xPos = 0, _yPos = 0;
};



//---------------------------------------------------------------------------------------------
//class Drawer declaration

static class UIItemDrawer {
	friend class UserInterface;
	friend class UIItemsContainer;

private:
	static void drawUIItem(UIItem*);

	static void setPosition(UIItem* UIItem, int, int);

	static void updateContainerProperties(UIItemsContainer*);
};



//---------------------------------------------------------------------------------------------
//class UserInterfaceItem declaration

class UIItem : public virtual GraphicItem {
	friend class UIItemDrawer;

private: 
	virtual void drawUIItem() const = 0; //наследуется, т.к. интерфейс
protected:
	//int _xPos = 0, _yPos = 0;
	int _width = 0, _height = 0;
	int _paddingSize = 0;
	int _marginSize = 0; //работают только по оси ориентации контейнера

	//virtual void drawUIItem() const = 0;
	virtual void setPosition(int, int);
public:
	virtual ~UIItem() = default;

	//получение размеров элемента интерфейса
	//arguments:
	//#1 - width
	//#2 - height
	//#3 - marginSize
	//virtual void getUIItemSizes(int*, int*, int*) const = 0;

	//virtual void setPosition(int, int) = 0;

	void getUIItemSizes(int*, int*, int*) const;

	//virtual void drawUIItem() const = 0; // скрыл
};

//---------------------------------------------------------------------------------------------
//class ActiveUserInterfaceItem declaration

/*class ActiveUIItem : public UIItem {
protected:
	//унаследованные:
	//int _xPos = 0, _yPos = 0;
	//int _width = 0, _height = 0;
	//int _paddingSize = 0;
	//int _marginSize = 0;

	void(*_onClickCallback)();
public:
	virtual void onClick() const = 0;
};*/



//---------------------------------------------------------------------------------------------
//class ActiveGraphicItem declaration
//разделить на onClick, onMouseOver и scrolledItem, и добавить scrolledItem к BattleField?

class ActiveGraphicItem : public virtual GraphicItem {
protected:
	//унаследовано int _xPos = 0, _yPos = 0;

	bool _mouseOver = false;
	void(*_onClickCallback)();

public:
	virtual void onClick() const = 0;
	//virtual void onClick() = 0;
	//virtual void onMouseOver(bool) = 0;
	virtual void onMouseOver(bool);
};

//---------------------------------------------------------------------------------------------
//class MenuItemContainer declaration

//class UIItemsContainer : private UIItem { //нельзя преобразовывать указатель в UIItem
class UIItemsContainer : public UIItem {
	friend class UIItemDrawer;

private:
	//унаследованные:
	//int _xPos = 0, _yPos = 0;
	//int _width = 0, _height = 0;
	//int _paddingSize = 0;
	//int _marginSize = 0;

	Orientation _orientation;
	VerticalAlign _verticalAlign;
	HorizontalAlign _horizontalAlign;
	//bool _scrollable = false;

	//UIItemsContainer* _parentContainer = nullptr;
	vector<UIItem*> _UIItems;

	void drawUIItem() const;
	void updateContainerProperties();

	virtual void setPosition(int, int) override;
public:
	//UIItemsContainer(UIItemsContainer*, Orientation, VerticalAlign, HorizontalAlign) noexcept;

	UIItemsContainer(Orientation, VerticalAlign, HorizontalAlign) noexcept;

	/*
	arguments:
	#1 - orientation
	#2 - vertical Align
	#3 - horizontal Align
	#4 - margin size
	*/
	UIItemsContainer(Orientation, VerticalAlign, HorizontalAlign, int) noexcept;
	//UIItemsContainer(UIItemsContainer &UIItemsContainer);
	~UIItemsContainer();

	//void getUIItemSizes(int*, int*, int*) const; //наследуется

	//void setPosition(int, int); //наследуется

	//void drawUIItem() const; //скрыл
	//void drawUIElement();
	//void onClick() const;

	//UIItemsContainer* _parentContainer = nullptr;

	void addUIItem(UIItem* const menuItem);

	//void updateContainerProperties(); //скрыта
};



//---------------------------------------------------------------------------------------------
//fuctions declaration

void createMainMenu();
//UIItemsContainer* createMainMenu();

//void createLoadMenu();

void createOptionsMenu();


//---------------------------------------------------------------------------------------------
//callbacks declaration

void callback_windowSize(GLFWwindow*, int, int);
void callback_key(GLFWwindow*, int, int, int, int);
void callback_mouseButton(GLFWwindow*, int, int, int);
void callback_cursorPos(GLFWwindow*, double, double);
void callback_cursorEnter(GLFWwindow*, int);
