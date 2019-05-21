//#include <cstdlib>

#include "userInterface.h"

//---------------------------------------------------------------------------------------------
//local variables, local types and local function declarations

//bool** _controlField = nullptr;

static int ID = -1;

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
	{ButtonType::LOAD, ButtonProperties(	"Load",		FontName::BUTTON,	callback_startButton_onClick)},
	{ButtonType::OPTIONS, ButtonProperties(	"Options",	FontName::BUTTON,	callback_startButton_onClick)},
	{ButtonType::EXIT, ButtonProperties(	"Exit",		FontName::BUTTON,	callback_startButton_onClick)}
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
//class ControlField definition

ControlField::ControlField() = default;
ControlField::~ControlField() = default;

ControlField& ControlField::Instance() {
	static ControlField singleton;
	return singleton;
}

void ControlField::setDisplaySize(int displayWidth, int displayHeight) {
	if (_controlField == nullptr) {
		_controlField = new ActiveUIItem**[displayWidth];
		for (int x = 0; x < displayWidth; x++) {
			_controlField[x] = new ActiveUIItem*[displayHeight];
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

void ControlField::setActiveUIItemControlField(int xPos, int width, int yPos, int height, ActiveUIItem* activeUIItem) {
	for (int x = 0; x <= xPos + width; x++) {
		for (int y = 0; y <= yPos + height; y++) {
			_controlField[x][y] = activeUIItem;
		}
	}
}

void ControlField::triggerActiveUIItemOnClickCallbackAtPoint(int xPos, int yPos) {
	ActiveUIItem* activeUIItem = _controlField[xPos][yPos];
	if (activeUIItem != nullptr) {
		activeUIItem->onClick();
	}
}

void ControlField::clearControlField() {
	for (int x = 0; x < _windowWidth; x++) {
		for (int y = 0; y < _windowHeight; y++) {
			_controlField[x][y] = nullptr;
		}
	}
}

//---------------------------------------------------------------------------------------------
//class UserInterface definition

UserInterface::UserInterface() = default;
UserInterface::~UserInterface() = default;

UserInterface& UserInterface::Instance() {
	static UserInterface singleton;
	return singleton;
}

//---------------------------------------------------------------------------------------------
//class MenuItemContainer definition

UIItemsContainer::UIItemsContainer(
	//UIItemsContainer* parentContainer,
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

void UIItemsContainer::getUIItemSizes(int* width, int* height, int* marginSize) const {
	*width = _width;
	*height = _height;
	//*paddingSize = _paddingSize;
	*marginSize = _marginSize;
}

void UIItemsContainer::drawUIItem() const { //const {
}

void UIItemsContainer::addUIItem(UIItem* UIItem) {
	/*if (UIItem->isUIItemsContainer()) {
		UIItemsContainer* container = (UIItemsContainer*)UIItem;
		container->setParentUIItemsContainer();
	}*/
	_UIItems.push_back(UIItem);
	//updateContainerProperties();
}

void UIItemsContainer::updateContainerProperties() { //!WARNING! сверху вниз, иначе бесконечная петля!!!
	for (int i = 0; i < _UIItems.size(); i++) {
		int _widthUIItem, _heightUIItem, _marginSizeUIItem;
		_UIItems[i]->getUIItemSizes(&_widthUIItem, &_heightUIItem, &_marginSizeUIItem);

		int _marginSizePrevUIItem = 0;
		if (_orientation == Orientation::VERTICAL) {
			_height += _heightUIItem;
			TODO
		} else {

		}
	}

	_width += 2 * _paddingSize;
	_height += 2 * _paddingSize;

	/*if (_parentContainer != nullptr) {
		_parentContainer->updateContainerProperties();
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

public:
	/*
	arguments:
	#1 - labelType
	#2 - paddingSize in pixels
	#3 - marginSize in pixels
	*/
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

	void getUIItemSizes(int* width, int* height, int* marginSize) const {
		*width = _width;
		*height = _height;
		//*paddingSize = _paddingSize;
		*marginSize = _marginSize;
	}

	//void drawUIElement() const;
	void drawUIItem() const {
		drawString(
			_xPos + _paddingSize,
			_yPos + _paddingSize,
			_fontName,
			_title,
			BUTTON_FONT_COLOR_RGB);
	}
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

	void getUIItemSizes(int* width, int* height, int* marginSize) const {
		*width = _width;
		*height = _height;
		//*paddingSize = _paddingSize;
		*marginSize = _marginSize;
	}

	void setPosition(int xPos, int yPos) {
		_xPos = xPos;
		_yPos = yPos;
	}

	void drawUIItem() const {
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

	void onClick() const {
		_onClickCallback();
	}
};

//---------------------------------------------------------------------------------------------
//functions

UIItemsContainer* createMainMenu() {
	UIItemsContainer* _menuItemsContainer = new UIItemsContainer(
		//nullptr,
		Orientation::VERTICAL,
		VerticalAlign::MIDDLE,
		HorizontalAlign::MIDDLE
	);

	_menuItemsContainer->addUIItem(new Label(LabelType::TITLE, 0, 40));

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

	_menuItemsContainer->addUIItem(_menuButtonsContainer);

	return _menuItemsContainer;
}

//---------------------------------------------------------------------------------------------
//buttons callbacks

void callback_startButton_onClick() {

}

void callback_loadButton_onClick() {

}

void callback_optionsButton_onClick() {

}

void callback_exitButton_onClick() {

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

		//cout << "cursor_x: " << cursor_x << "  cursor_y: " << cursor_y << endl;
	}
}

