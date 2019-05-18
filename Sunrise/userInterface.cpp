//#include <cstdlib>

#include "userInterface.h"

//---------------------------------------------------------------------------------------------
//local variables, local types and local function declarations

//bool** _controlField = nullptr;

static int ID = -1;

ButtonProperties::ButtonProperties(string title, void(*callback)()) {
	//this->title = title;
	_title = title;
	this->callback = callback;
}

string ButtonProperties::getTitle() const {
	return _title;
}

/*const map<const ButtonType, const string> buttonNames = {
	{ButtonType::START,"Start"},
	{ButtonType::LOAD,"Load"},
	{ButtonType::OPTIONS,"Options"},
	{ButtonType::EXIT,"Exit"}
};*/

void callback_startButton_onClick();

const map<const ButtonType, const ButtonProperties> buttonData = {
	{ButtonType::START, ButtonProperties("Start", callback_startButton_onClick)},
	{ButtonType::LOAD, ButtonProperties("Load", callback_startButton_onClick)},
	{ButtonType::OPTIONS, ButtonProperties("Options", callback_startButton_onClick)},
	{ButtonType::EXIT, ButtonProperties("Exit", callback_startButton_onClick)}
};

//---------------------------------------------------------------------------------------------
//class ControlField definition

ControlField::ControlField() = default;
ControlField::~ControlField() = default;

ControlField& ControlField::Instance() {
	static ControlField singleton;
	return singleton;
}

void ControlField::setDisplaySize(int displayWidth, int displayHeight) {
	if (_controlField == nullptr) {
		_controlField = new UserInterfaceItem**[displayWidth];
		for (int x = 0; x < displayWidth; x++) {
			_controlField[x] = new UserInterfaceItem*[displayHeight];
		}
		_displayWidth = displayWidth;
		_displayHeight = displayHeight;
	}
}

void ControlField::setWindowSize(int windowWidth, int windowHeight) {
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	/*if (_controlField != nullptr) { //TODO небольшая утечка памяти до 8мб. возможно она даже не моя. зависит от величины экрана. если открывать сразу большой экран, то утечки нет.
		for (int x = 0; x < _windowWidth; x++) {
			delete[] _controlField[x];
		}
		delete[] _controlField;
		_controlField = nullptr;
	}

	_controlField = new bool*[width];
	for (int x = 0; x < width; x++) {
		_controlField[x] = new bool[height];
	}
	_windowWidth = width;
	_windowHeight = height;*/
}

void ControlField::getWindowSize(int* width, int* height) const {
	*width = _windowWidth;
	*height = _windowHeight;
}

void ControlField::clearControlField() {
	for (int x = 0; x < _windowWidth; x++) {
		for (int y = 0; y < _windowHeight; y++) {
			_controlField[x][y] = nullptr;
		}
	}
}

//---------------------------------------------------------------------------------------------
//class MenuItemContainer definition

MenuItemContainer::MenuItemContainer(
	Orientation orientation, 
	VerticalPosition verticalPosition, 
	HorizontalPosition horizontalPosition
) noexcept {
	_orientation = orientation;
	_verticalPosition = verticalPosition;
	_horizontalPosition = horizontalPosition;
}

MenuItemContainer::~MenuItemContainer() {
	/*for (int i = 0; i < menuItems.size(); i++) {
		delete menuItems[i];
	}*/
}

void MenuItemContainer::addMenuItem(UserInterfaceItem* menuItem) {
	//menuItems.push_back(menuItem);
	/*vector<UserInterfaceItem*> temp;
	menuItems = temp;*/
}

void MenuItemContainer::drawUIElement() { //const {
}

void MenuItemContainer::onClick() const {
}

//---------------------------------------------------------------------------------------------
//class Label definition

Label::Label(string title, int textSize) noexcept {
	_title = title;
	_textSize = textSize;

}

Label::~Label() {
}

void Label::drawUIElement() { //const { TODO
	/*glColor3f(buttonColorRGB[0], buttonColorRGB[1], buttonColorRGB[2]);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);	glVertex2i(_xPos, _yPos);
	glTexCoord2i(1, 0); glVertex2i(_xPos + _width, _yPos);
	glTexCoord2i(1, 1); glVertex2i(_xPos + _width, _yPos + _height);
	glTexCoord2i(0, 1); glVertex2i(_xPos, _yPos + _height);
	glEnd();*/
}

void Label::onClick() const {
}

//---------------------------------------------------------------------------------------------
//class Button definition

Button::Button(const ButtonProperties buttonProperties, const int textSize, const int marginSize, const int paddingSize) noexcept {
	_title = buttonProperties.getTitle();
	_callback = buttonProperties.callback;
	_textSize = textSize;
	_marginSize = marginSize;
	_paddingSize = paddingSize;
	_height = _paddingSize * 2 + _textSize;
	_width = _paddingSize * 2 + _textSize; //TODO
}

Button::~Button() {
}

void Button::drawUIElement() { //const { TODO
	//_xPos = 20; _yPos = 20; //TODO
	#define _xPos xx
	#define _yPos yy
	int xx = 20, yy = 20;

	glColor3fv(buttonColorRGB);
	glBegin(GL_QUADS);
	glVertex2i(_xPos, _yPos);
	glVertex2i(_xPos + _width, _yPos);
	glVertex2i(_xPos + _width, _yPos + _height);
	glVertex2i(_xPos, _yPos + _height);
	glEnd();

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glColor3f(0.8, 1, 0.5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 6); //чем меньше колво айди - тем лучше

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	glBegin(GL_QUADS); //устаревший интерфейс, НО время выполнения функции всё ещё менее 1 миллисекунды
	glTexCoord2i(0, 0); glVertex2i(_xPos, _yPos); //i - это int
	glTexCoord2i(1, 0); glVertex2i(_xPos + 18, _yPos);
	glTexCoord2i(1, 1); glVertex2i(_xPos + 18, _yPos + 18); //74/73
	glTexCoord2i(0, 1); glVertex2i(_xPos, _yPos + 18);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	/*int width, height;
	const unsigned char* bitmapBuffer;
	getCharBitmapBuffer(FontName::TITLE, 'S', &bitmapBuffer, &width, &height);

	if (ID == -1) {
		//ID = SOIL_create_OGL_texture(bitmapBuffer, width, height, 1, SOIL_CREATE_NEW_ID,  SOIL_FLAG_MIPMAPS); //канал распознаётся как яркость(нет прозрачность. фон - чёрный квадрат, но цвет задаётся glColor //появляется сглаживание
		//ID = SOIL_create_OGL_texture(bitmapBuffer, width, height, 1, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT ); //перекошенный текст. чёрный фон //цвет текста glColor
		ID = SOIL_create_OGL_texture(bitmapBuffer, width, height, 1, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO); //сглаживание меньше чем при мипмапах, чёрный фон
	}
	glColor3f(0.8,1,0.5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID); //чем меньше колво айди - тем лучше

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//повторение текстуры

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//сглаживания при уменьшении
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//сглаживания при увеличении

	glBegin(GL_QUADS); //устаревший интерфейс, НО время выполнения функции всё ещё менее 1 миллисекунды
	glTexCoord2i(0, 0); glVertex2i(_xPos, _yPos); //i - это int
	glTexCoord2i(1, 0); glVertex2i(_xPos + width, _yPos);
	glTexCoord2i(1, 1); glVertex2i(_xPos + width, _yPos + height);
	glTexCoord2i(0, 1); glVertex2i(_xPos, _yPos + height);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);*/


	/*glBegin(GL_POINTS);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const int a = bitmapBuffer[y * width + x];
			float aa = (float)a / 255;
			glColor4f(0.8, 1.f, 0.8, aa);
			glVertex2i(_xPos+x, _yPos+y);
		}
	}
	glEnd();*/

	/*glColor3f(1.f, 1.f, 1.f);
	glPixelTransferi(GL_ALPHA_SCALE, 1);*/

	/*glPixelTransferi(GL_MAP_COLOR, 1);//использоваь карту
	float map[2] = { 1, 0 };//пусть...
	  //...все единицы закрашивают в чёрный, а все нули - в белый цвета...
	glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 2, map);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 2, map);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 2, map);
	//...и все нули будут прозрачными
	map[0] = 0, map[1] = 1;
	glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 2, map);

	glRasterPos2i(_xPos + 20, _yPos + 20);
	glPixelZoom(1.0, -1.0);
	glDrawPixels(width, height, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, bitmapBuffer);

	glPixelTransferi(GL_MAP_COLOR, 0);//не использовать карту*/

	//glPixelTransferi(GL_MAP_COLOR, 1);
	/*unsigned char arr[24] = {
		0, 255, 255, 0, 255, 127,
		0, 255, 0, 255, 255, 127,
		0, 0, 255, 255, 0, 0,
		127, 0, 127, 0, 255, 127
	};*/

	//glColor3f(255, 0, 255);

	/*unsigned char arr[96] = {
		255, 255, 255, 0,		255, 255, 255, 255,		255, 255, 255, 127,		255, 255, 255, 0,		255, 255, 255, 255,		255, 255, 255, 127,
		255, 255, 255, 255,		255, 255, 255, 255,		255, 255, 255, 127,		255, 255, 255, 255,		255, 255, 255, 255,		255, 255, 255, 127,
		255, 255, 255, 127,		255, 255, 255, 0,		255, 255, 255, 0,		255, 255, 255, 127,		255, 255, 255, 0,		255, 255, 255, 0,
		255, 255, 255, 127,		255, 255, 255, 0,		255, 255, 255, 0,		255, 255, 255, 127,		255, 255, 255, 0,		255, 255, 255, 0
	};*/

	/*glRasterPos2i(_xPos + 20, _yPos + 20);
	glPixelZoom(16.0, -16.0);
	glDrawPixels(6, 4, GL_ALPHA, GL_UNSIGNED_BYTE, arr);*/
	//glPixelTransferi(GL_MAP_COLOR, 0);

	/*xx = yy = 40;
	int ID = SOIL_create_OGL_texture(arr, 6, 4, 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);
	glEnable(GL_TEXTURE_2D);
	//auto start = clock();
	glBindTexture(GL_TEXTURE_2D, ID); //чем меньше колво айди - тем лучше

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//повторение текстуры
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//сглаживания при уменьшении
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//сглаживания при увеличении

	glBegin(GL_QUADS); //устаревший интерфейс, НО время выполнения функции всё ещё менее 1 миллисекунды
	glTexCoord2i(0, 0); glVertex2i(_xPos, _yPos); //i - это int
	glTexCoord2i(1, 0); glVertex2i(_xPos + 60, _yPos);
	glTexCoord2i(1, 1); glVertex2i(_xPos + 60, _yPos + 40);
	glTexCoord2i(0, 1); glVertex2i(_xPos, _yPos + 40);
	glEnd();
	glDisable(GL_TEXTURE_2D);*/

	/*// Вывод символа в консоли
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// Получение прозрачности точки (x, y)
			//const int a = glyph->bitmap.buffer[y * pitch + x];
			const int a = bitmapBuffer[y * width + x];

			if (a > 127) {
				cout << "*";
			//} else if (a > 63) {
				//cout << ".";
			} else {
				//cout << " ";
				cout << ".";
			}
		}
		cout << endl;
	}*/


	/*static int quantity = 0, sum = 0;
	if (quantity > 100) { quantity = sum = 0; }
	int start = std::clock();
	//cout << "start - " << std::clock();
	int w, h;
	ControlField::Instance().getWindowSize(&w, &h);
	for (int i = 0; i < 100000; i++) {	
	//glBegin(GL_QUADS);
	//glColor3fv(buttonColorRGB);
		glVertex2i(_xPos, _yPos);
		glVertex2i(_xPos + _width, _yPos);
		glVertex2i(_xPos + _width, _yPos + _height);
		glVertex2i(_xPos, _yPos + _height);
		//glEnd();
	}
	quantity++;
	sum += std::clock() - start;
	cout << "average time(q=" << quantity << "):\t" << (float) sum/ quantity << endl;
	glEnd();*/
}

void Button::onClick() const {
	_callback();
}

//---------------------------------------------------------------------------------------------
//functions

MenuItemContainer* createMainMenu() {
	MenuItemContainer* _menuItemContainer = new MenuItemContainer(
		Orientation::VERTICAL,
		VerticalPosition::MIDDLE,
		HorizontalPosition::MIDDLE
	);

	_menuItemContainer->addMenuItem(new Label(MAIN_MENU_TITLE, 10));
	_menuItemContainer->addMenuItem(new Button(buttonData.at(ButtonType::START),40, 5, 5));
	_menuItemContainer->addMenuItem(new Button(buttonData.at(ButtonType::LOAD), 40, 5, 5));
	_menuItemContainer->addMenuItem(new Button(buttonData.at(ButtonType::OPTIONS), 40, 5, 5));
	_menuItemContainer->addMenuItem(new Button(buttonData.at(ButtonType::EXIT), 40, 5, 5));

	return _menuItemContainer;
}

void drawMainMenu() {

}

//---------------------------------------------------------------------------------------------
//callbacks

void callback_startButton_onClick() {

}

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
		glfwSetWindowSize(window, height * 2, height);
	}


	ControlField::Instance().setWindowSize(width, height);

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

		//cout << "GLFW_MOUSE_BUTTON_1:" << endl;
		cout << "cursor_x: " << cursor_x << "  cursor_y: " << cursor_y << endl;
	}
}

