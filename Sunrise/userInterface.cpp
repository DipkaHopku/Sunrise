//#include <cstdlib>

#include "userInterface.h"

//---------------------------------------------------------------------------------------------
//local variables, local types and local function declarations

//bool** _controlField = nullptr;
//static int ID = -1;

class ButtonProperties {
private:
	string _title;
	FontName _fontName;

public:
	void(*callback)();

	ButtonProperties(string title, FontName fontName, void(*callback)()) {
		//this->title = title;
		_title = title;
		_fontName = fontName;
		this->callback = callback;
	}

	string getTitle() const {
		return _title;
	}

	FontName getFontName() const {
		return _fontName;
	}
};

/*const map<const ButtonType, const string> buttonNames = {
	{ButtonType::START,"Start"},
	{ButtonType::LOAD,"Load"},
	{ButtonType::OPTIONS,"Options"},
	{ButtonType::EXIT,"Exit"}
};*/

void callback_startButton_onClick();
void callback_loadButton_onClick();
void callback_optionsButton_onClick();
void callback_exitButton_onClick();

const map<const ButtonType, const ButtonProperties> buttonsData = {
	{ButtonType::START, ButtonProperties(	"Start",	FontName::BUTTON,	callback_startButton_onClick)},
	{ButtonType::LOAD, ButtonProperties(	"Load",		FontName::BUTTON,	callback_loadButton_onClick)},
	{ButtonType::OPTIONS, ButtonProperties(	"Options",	FontName::BUTTON,	callback_optionsButton_onClick)},
	{ButtonType::EXIT, ButtonProperties(	"Exit",		FontName::BUTTON,	callback_exitButton_onClick)}
};

class LabelProperties {
private:
	string _title;
	FontName _fontName;

public:
	void(*callback)();

	LabelProperties(string title, FontName fontName) {
		_title = title;
		_fontName = fontName;
	}

	string getTitle() const {
		return _title;
	}

	FontName getFontName() const {
		return _fontName;
	}
};

const map<const LabelType, const LabelProperties> labelsData = {
	{LabelType::TITLE, LabelProperties(		"Sunrise",		FontName::TITLE)},
	{LabelType::FULLSCREEN, LabelProperties("Fullscreen:",	FontName::SIMPLE_TEXT)}
};

//---------------------------------------------------------------------------------------------
//class Drawer definition

void UIItemDrawer::drawUIItem(UIItem* UIItem) {
	UIItem->drawUIItem();
}

void UIItemDrawer::setPosition(UIItem* UIItem, int xPos, int yPos) {
	UIItem->setPosition(xPos, yPos);
}

//нужно для UserInterface
void UIItemDrawer::updateContainerProperties(UIItemsContainer* UIItemsContainer) {
	UIItemsContainer->updateContainerProperties();
}

//---------------------------------------------------------------------------------------------
//class UserInterface definition

UserInterface::UserInterface() = default;
UserInterface::~UserInterface() = default;

UserInterface& UserInterface::Instance() {
	static UserInterface singleton;
	return singleton;
}

void UserInterface::setDisplaySize(int displayWidth, int displayHeight) {
	if (_controlField == nullptr) {
		_controlField = new ActiveUIItem**[displayWidth];
		for (int x = 0; x < displayWidth; x++) {
			_controlField[x] = new ActiveUIItem*[displayHeight];
		}
		_displayWidth = displayWidth;
		_displayHeight = displayHeight;
	}
}

void UserInterface::setWindowSize(int windowWidth, int windowHeight) {
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	updateContainerProperties();
}

void UserInterface::getWindowSize(int* width, int* height) const {
	*width = _windowWidth;
	*height = _windowHeight;
}

void UserInterface::setActiveUIItemControlField(int xPos, int width, int yPos, int height, ActiveUIItem* activeUIItem) {
	//костыль изза отсутсвия появления скрола при размерах контейнера больше размеров окна
	//работает замечательно. пиксель в пиксель
	for (int x = xPos; (x < xPos + width) && (x < _windowWidth); x++) {
		for (int y = yPos; (y < yPos + height) && (y < _windowHeight); y++) {
			_controlField[x][y] = activeUIItem;
		}
	}
	/*for (int x = 0; x < xPos + width; x++) {
		for (int y = 0; y < yPos + height; y++) {
			_controlField[x][y] = activeUIItem;
		}
	}*/
}

void UserInterface::triggerActiveUIItemOnClickCallbackAtPoint(int xPos, int yPos) {
	ActiveUIItem* activeUIItem = _controlField[xPos][yPos];
	if (activeUIItem != nullptr) {
		activeUIItem->onClick();
	}
}

void UserInterface::clearControlField() {
	for (int x = 0; x < _windowWidth; x++) {
		for (int y = 0; y < _windowHeight; y++) {
			_controlField[x][y] = nullptr;
		}
	}
}

/*
После передачи контейнера в эту функцию контейнер трогать нельзя от слова совсем. костыль такой
*/
void UserInterface::setUIItemsContainer(UIItemsContainer* UIItemsContainer) {
	delete _UIItemsContainer;

	_UIItemsContainer = UIItemsContainer; //возможно нужно копировать объект, а не ссылку
	//_UIItemContainer = new UIItemsContainer(*uIItemsContainer); //TODO

	updateContainerProperties();
}

void UserInterface::updateContainerProperties() { //существует, потому что рассчитывать параметры контейнеров надо не только при добавлении элементов в контейнер, но и при изменении размеров окна //как минимум это задел на появление скрола при малых и недостаточных размерах окна
	if (_UIItemsContainer != nullptr) {
		//снизу доверха рассчитываем размеры контейнеров, а потом сверху вниз позиции
		UIItemDrawer::updateContainerProperties(_UIItemsContainer);

		/*int _widthUIItem, _heightUIItem;
		_UIItemContainer->getUIItemSizes(&_widthUIItem, &_heightUIItem, nullptr);*/

		//UserInterface единственный контейнер, которого волнуют внешние отступы одиночных вложенных элементов интерфейса. святой костыль
		int _widthUIItem, _heightUIItem, _marginSizeUIItem;
		_UIItemsContainer->getUIItemSizes(&_widthUIItem, &_heightUIItem, &_marginSizeUIItem);

		int _xPosUIItem = 0, _yPosUIItem = 0;

		switch (_horizontalAlign) {
		case(HorizontalAlign::LEFT):
			_xPosUIItem = 0;
			break;
		case(HorizontalAlign::MIDDLE):
			_xPosUIItem = (_windowWidth - _widthUIItem) / 2; //изза int`а копится небольшая ошибка. максимум 1 пиксель на контейнер, впрочем это не влияет на controlField изза полукостыля в нём
			break;
		case(HorizontalAlign::RIGHT):
			_xPosUIItem = _windowWidth - _widthUIItem;
			break;
		}
		if (_marginSizeUIItem > _xPosUIItem) _xPosUIItem = _marginSizeUIItem;

		switch (_verticalAlign) {
		case(VerticalAlign::TOP) :
			_yPosUIItem = 0;
			break;
		case(VerticalAlign::MIDDLE):
			_yPosUIItem = (_windowHeight - _heightUIItem) / 2;
			break;
		case(VerticalAlign::BOTTOM):
			_yPosUIItem = _windowHeight - _heightUIItem;
			break;
		}
		if (_marginSizeUIItem > _yPosUIItem) _yPosUIItem = _marginSizeUIItem;
		

		UIItemDrawer::setPosition(_UIItemsContainer, _xPosUIItem, _yPosUIItem);
	}
}

void UserInterface::drawUserInterface() const {
	if (_UIItemsContainer != nullptr) {
		UIItemDrawer::drawUIItem(_UIItemsContainer);
	}
}

//---------------------------------------------------------------------------------------------
//class UIItem definition

void UIItem::getUIItemSizes(int* width, int* height, int* marginSize) const {
	if (width != nullptr) *width = _width;
	if (height != nullptr) *height = _height;
	if (marginSize != nullptr) *marginSize = _marginSize;
}

void UIItem::setPosition(int xPos, int yPos) {
	_xPos = xPos;
	_yPos = yPos;
}

//---------------------------------------------------------------------------------------------
//class UIItemsContainer definition

void UIItemsContainer::drawUIItem() const { //const {
	for (int i = 0; i < _UIItems.size(); i++) {
		//Drawer d;
		UIItemDrawer::drawUIItem(_UIItems[i]);
	}
}

UIItemsContainer::UIItemsContainer(
	Orientation orientation, 
	VerticalAlign verticalAlign, 
	HorizontalAlign horizontalAlign
) noexcept {
	_orientation = orientation;
	_verticalAlign = verticalAlign;
	_horizontalAlign = horizontalAlign;
}

UIItemsContainer::~UIItemsContainer() {
	for (int i = 0; i < _UIItems.size(); i++) {
		delete _UIItems[i];
	}
}

/*void UIItemsContainer::getUIItemSizes(int* width, int* height, int* marginSize) const {
	if (width != nullptr) *width = _width;
	if (height != nullptr) *height = _height;
	//*paddingSize = _paddingSize;
	if (marginSize != nullptr) *marginSize = _marginSize;
}*/

//установка размеров контейнеров снизу вверх
//установка позиций сверху вниз
void UIItemsContainer::updateContainerProperties() { //!WARNING! сверху вниз, иначе бесконечная петля!!!
	int _newWidth = 0, _newHeight = 0;
	int _marginSizePrevUIItem = 0, _marginSizeBetWeenUIItems = 0;

	for (int i = 0; i < _UIItems.size(); i++) {
		if (UIItemsContainer* _UIItemsContainer = dynamic_cast<UIItemsContainer*>(_UIItems[i])) {
			_UIItemsContainer->updateContainerProperties();
		}

		//к этому моменту у _UIItems точно есть размеры
		int _widthUIItem, _heightUIItem, _marginSizeUIItem;
		_UIItems[i]->getUIItemSizes(&_widthUIItem, &_heightUIItem, &_marginSizeUIItem);

		if (i != 0) _marginSizeUIItem > _marginSizePrevUIItem ? _marginSizeBetWeenUIItems = _marginSizeUIItem : _marginSizeBetWeenUIItems = _marginSizePrevUIItem;
		if (_orientation == Orientation::VERTICAL) {			
			_newHeight += _marginSizeBetWeenUIItems + _heightUIItem;
			
			if (_widthUIItem > _newWidth) _newWidth = _widthUIItem;
		} else {
			_newWidth += _marginSizeBetWeenUIItems + _widthUIItem;

			if (_heightUIItem > _newHeight) _newHeight = _heightUIItem;
		}

		_marginSizePrevUIItem = _marginSizeUIItem;
	}

	_newWidth += 2 * _paddingSize;
	_newHeight += 2 * _paddingSize;

	_width = _newWidth;
	_height = _newHeight;
}

void UIItemsContainer::setPosition(int xPos, int yPos) {
	_xPos = xPos;
	_yPos = yPos;

	int _xPosPrevUIItem = _xPos, _yPosPrevUIItem = _yPos; //на самом деле можно было обойтись 1 переменной, но так нагляднее и безопаснее
	int _widthPrevUIItem = 0, _heightPrevUIItem = 0; //таже история, что и строчкой выше
	int _marginSizePrevUIItem = 0, _marginSizeBetWeenUIItems = 0;

	for (int i = 0; i < _UIItems.size(); i++) {
		int _widthUIItem, _heightUIItem, _marginSizeUIItem;
		_UIItems[i]->getUIItemSizes(&_widthUIItem, &_heightUIItem, &_marginSizeUIItem);

		int _xPosUIItem = _xPos + _paddingSize, _yPosUIItem = _yPos + _paddingSize; //инициализирую по фану //на самом деле чтоб в случае ЧЕГОТО в _xPos не было каких нибудь -85******, хотя такой баг быстро обнаруживал бы себя. хм

		if (i != 0) _marginSizeUIItem > _marginSizePrevUIItem ? _marginSizeBetWeenUIItems = _marginSizeUIItem : _marginSizeBetWeenUIItems = _marginSizePrevUIItem;
		if (_orientation == Orientation::VERTICAL) {
			switch (_horizontalAlign) {
			case(HorizontalAlign::LEFT):
				_xPosUIItem = _xPos + _paddingSize;
				break;
			case(HorizontalAlign::MIDDLE):
				_xPosUIItem = _xPos + _paddingSize + (_width - _widthUIItem) / 2;
				break;
			case(HorizontalAlign::RIGHT):
				_xPosUIItem = _xPos + _paddingSize + _width - _widthUIItem;
				break;
			}

			_yPosUIItem = _yPosPrevUIItem + _heightPrevUIItem + _marginSizeBetWeenUIItems;
			//if (_marginSizeUIItem > _xPosUIItem) _xPosUIItem = _marginSizeUIItem; //не обсчитываются во второй оси
		}
		else {
			switch (_verticalAlign) {
			case(VerticalAlign::TOP):
				_yPosUIItem = _yPos;
				break;
			case(VerticalAlign::MIDDLE):
				_yPosUIItem = _yPos + (_height - _heightUIItem) / 2;
				break;
			case(VerticalAlign::BOTTOM):
				_yPosUIItem = _yPos + _height - _heightUIItem;
				break;
			}

			_xPosUIItem = _xPosPrevUIItem + _widthPrevUIItem + _marginSizeBetWeenUIItems;
			//if (_marginSizeUIItem > _yPosUIItem) _yPosUIItem = _marginSizeUIItem; //не обсчитываются во второй оси
		}

		UIItemDrawer::setPosition(_UIItems[i], _xPosUIItem, _yPosUIItem);

		_xPosPrevUIItem = _xPosUIItem;
		_yPosPrevUIItem = _yPosUIItem;
		_widthPrevUIItem = _widthUIItem;
		_heightPrevUIItem = _heightUIItem;
		_marginSizePrevUIItem = _marginSizeUIItem;
	}
}

void UIItemsContainer::addUIItem(UIItem* UIItem) {
	_UIItems.push_back(UIItem);
	//это делает UserInterface и только тогда когда есть необходимость
	/*if (UIItemsContainer* _UIItemsContainer = dynamic_cast<UIItemsContainer*>(UIItem)) {
		_UIItemsContainer->updateContainerProperties(); //TODO
	}*/
}

//---------------------------------------------------------------------------------------------
//class Label definition

class Label : public UIItem {
private:
	//унаследованные:
	//int _xPos = 0, _yPos = 0;
	//int _width = 0, _height = 0;
	//int _paddingSize = 0;
	//int _marginSize = 0;

	string _title;
	FontName _fontName;

	void drawUIItem() const {
		drawString(
			_xPos + _paddingSize,
			_yPos + _paddingSize,
			_fontName,
			_title,
			BUTTON_FONT_COLOR_RGB);
	}

public:
	/*
	arguments:
	#1 - labelType
	#2 - paddingSize in pixels
	#3 - marginSize in pixels
	*/
	//устанавливается всё кроме позиции кнопки, её устанавливает контейнер
	Label(const LabelType labelType, const int paddingSize, const int marginSize) { //noexcept {
		auto _labelData = labelsData.find(labelType);
		if (_labelData != labelsData.end()) {
			_title = _labelData->second.getTitle();
			_fontName = _labelData->second.getFontName();
		}
		_paddingSize = paddingSize;
		_marginSize = marginSize;

		int stringWidth, stringHeight;
		//int** charslocation;
		getStringProperties(_fontName, _title, &stringWidth, &stringHeight); // , &_charsLocation);
		_width = _paddingSize * 2 + stringWidth;
		_height = _paddingSize * 2 + stringHeight;
	}

	~Label() {
	}

	/*void getUIItemSizes(int* width, int* height, int* marginSize) const {
		if (width != nullptr) *width = _width;
		if (height != nullptr) *height = _height;
		//*paddingSize = _paddingSize;
		if (marginSize != nullptr) *marginSize = _marginSize;
	}*/

	//void drawUIElement() const;
	/*void drawUIItem() const {
		drawString(
			_xPos + _paddingSize,
			_yPos + _paddingSize,
			_fontName,
			_title,
			BUTTON_FONT_COLOR_RGB);
	}*/
};

//---------------------------------------------------------------------------------------------
//class Button definition
class Button : public ActiveUIItem {
private:
	//унаследованные:
	//int _xPos = 0, _yPos = 0;
	//int _width = 0, _height = 0;
	//int _paddingSize = 0;
	//int _marginSize = 0;
	//void(*_callback)();

	string _title;
	FontName _fontName;

	//int _textSize;
	//int** _charsLocation;

	void drawUIItem() const {
		UserInterface::Instance().setActiveUIItemControlField(_xPos, _width, _yPos, _height, (ActiveUIItem*)this);

		glColor4fv(BUTTON_COLOR_RGB);
		glBegin(GL_QUADS);
		glVertex2i(_xPos, _yPos); //самый левый верхний пиксель квардрата будет _xPos, _yPos, а самый левый верхний пиксель окна в системе координат опенгл это (0,0)
		glVertex2i(_xPos + _width, _yPos);
		glVertex2i(_xPos + _width, _yPos + _height); //самый правый нижний пиксель квадрата будет _xPos + _width - 1, _yPos + _height - 1, а самый правый нижний пиксель окна в системе координат опенгл это (window width -1, window height -1)
		glVertex2i(_xPos, _yPos + _height);
		glEnd();

		drawString(
			_xPos + _paddingSize,
			_yPos + _paddingSize,
			_fontName,
			_title,
			BUTTON_FONT_COLOR_RGB);
	}

public:
	//устанавливается всё кроме позиции кнопки, её устанавливает контейнер
	Button(const ButtonType buttonType, const int paddingSize, const int marginSize) { //noexcept {
		auto _buttonData = buttonsData.find(buttonType);
		if (_buttonData != buttonsData.end()) {
			_title = _buttonData->second.getTitle();
			_fontName = _buttonData->second.getFontName();
			_onClickCallback = _buttonData->second.callback;
		}
		_paddingSize = paddingSize;
		_marginSize = marginSize;

		int stringWidth, stringHeight;
		//int** charslocation;
		getStringProperties(_fontName, _title, &stringWidth, &stringHeight); // , &_charsLocation);
		_width = _paddingSize * 2 + stringWidth;
		_height = _paddingSize * 2 + stringHeight;
	}

	~Button() {
		/*for (int i = 0; i < _title.length(); i++) {
			delete[] _charsLocation[i];
		}
		delete[] _charsLocation;*/
	}

	/*void getUIItemSizes(int* width, int* height, int* marginSize) const {
		if (width != nullptr) *width = _width;
		if (height != nullptr) *height = _height;
		//*paddingSize = _paddingSize;
		if (marginSize != nullptr) *marginSize = _marginSize;
	}*/

	/*void setPosition(int xPos, int yPos) {
		_xPos = xPos;
		_yPos = yPos;
	}*/

	/*void drawUIItem() const {
		ControlField::Instance().setActiveUIItemControlField(_xPos, _width, _yPos, _height, (ActiveUIItem*)this);

		glColor4fv(BUTTON_COLOR_RGB);
		glBegin(GL_QUADS);
		glVertex2i(_xPos, _yPos);
		glVertex2i(_xPos + _width, _yPos);
		glVertex2i(_xPos + _width, _yPos + _height);
		glVertex2i(_xPos, _yPos + _height);
		glEnd();

		drawString(
			_xPos + _paddingSize,
			_yPos + _paddingSize,
			_fontName,
			_title,
			BUTTON_FONT_COLOR_RGB);
	}*/

	void onClick() const {
		_onClickCallback();
	}

	//debug
	//string __getTitle() {return _title;}
};

//---------------------------------------------------------------------------------------------
//functions

void createMainMenu() {
//UIItemsContainer* createMainMenu() {
	UIItemsContainer* _mainMenuItemsContainer = new UIItemsContainer(
		//nullptr,
		Orientation::VERTICAL,
		VerticalAlign::MIDDLE,
		HorizontalAlign::MIDDLE
	);

	_mainMenuItemsContainer->addUIItem(new Label(LabelType::TITLE, 0, 40));

	UIItemsContainer* _menuButtonsContainer = new UIItemsContainer(
		//_menuItemsContainer,
		Orientation::VERTICAL,
		VerticalAlign::MIDDLE,
		HorizontalAlign::MIDDLE
	);

	_menuButtonsContainer->addUIItem(new Button(ButtonType::START, 10, 10));
	_menuButtonsContainer->addUIItem(new Button(ButtonType::LOAD, 10, 10));
	_menuButtonsContainer->addUIItem(new Button(ButtonType::OPTIONS, 10, 10));
	_menuButtonsContainer->addUIItem(new Button(ButtonType::EXIT, 10, 10));

	_mainMenuItemsContainer->addUIItem(_menuButtonsContainer);

	UserInterface::Instance().setUIItemsContainer(_mainMenuItemsContainer);

	//чтобы не париться по поводу дальнейшнего изменения _mainMenuItemsContainer можно создать класс-переборку, берущего создание контейнера, работу с классом UserInterface и удаление контейнера на себя, но это время

	//delete _mainMenuItemsContainer;
	//return _menuItemsContainer;
}

//---------------------------------------------------------------------------------------------
//buttons callbacks

void callback_startButton_onClick() {
	cout << "start button is pressed" << endl;
}

void callback_loadButton_onClick() {
	cout << "load button is pressed" << endl;
}

void callback_optionsButton_onClick() {
	cout << "options button is pressed" << endl;
}

void callback_exitButton_onClick() {
	cout << "exit button is pressed" << endl;
}

//---------------------------------------------------------------------------------------------
//window callbacks

void callback_windowSize(GLFWwindow* window, int width, int height) {
	/*GLint data[4];
	glGetIntegerv(GL_VIEWPORT, data);*/

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, -1);

	/*if ((float)width / height >= 2) {
		glfwSetWindowSizeLimits(window, 640, 480, height * 2, 1024);
	}
	else {
		glfwSetWindowSizeLimits(window, 640, 480, 1280, 1024);
	}*/

	if ((float)width / height >= 2) {
		//int _windowWidth, _windowHeight;
		//ControlField::Instance().getWindowSize(&_windowWidth, &_windowHeight);
		width = height * 2;
		glfwSetWindowSize(window, width, height);
	}

	UserInterface::Instance().setWindowSize(width, height);

	//MenuItemContainer.setWindowSize(width, height);
}

void callback_key(GLFWwindow *window, const int key, const int scancode, const int action, const int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void callback_mouseButton(GLFWwindow *window, const int button, const int action, const int mods) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		double cursor_x, cursor_y;
		glfwGetCursorPos(window, &cursor_x, &cursor_y);

		UserInterface::Instance().triggerActiveUIItemOnClickCallbackAtPoint(cursor_x, cursor_y);
		//cout << "cursor_x: " << cursor_x << "  cursor_y: " << cursor_y << endl;
	}
}



/*//debug
void ControlField::__drawControlField() {
	glColor4f(1, 0, 0, 1);
	int left, top, right, bot;
	left = 0;
	right = 1;
	top = 0;
	bot = 1;
	glBegin(GL_QUADS);
	glVertex2i(left, top);
	glVertex2i(right, top);
	glVertex2i(right, bot); //на самом деле right - 1, bot - 1
	glVertex2i(left, bot);


	//cerr << "WARNING: debug function __drawControlField is enabled" << endl;
	glDisable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	for (int x = 0; x < _windowWidth; x++) {
		for (int y = 0; y < _windowHeight; y++) {
			if (_controlField[x][y] != nullptr) {
				//if (((Button*)_controlField[x][y])->__getTitle() == "Start") {}
				string str = ((Button*)_controlField[x][y])->__getTitle();
				if (str == "Start") glColor4f(1, 0, 0, 0.25);
				else if (str == "Load") glColor4f(0, 1, 0, 0.25);
				else if (str == "Options") glColor4f(0, 0, 1, 0.25);
				else glColor4f(0, 1, 1, 0.25);
				//glColor4f(1, 0, 0, 0.25);
				glVertex2i(x, y);
			}
		}
		//glColor4f(1, 0, 0, 1);
		//glVertex2i(x, 1);
	}
	glEnd();
}*/