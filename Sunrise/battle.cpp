#include "battle.h"

//вообще то эти значения надо получать из соответсвующей текстуры, но... ок
const int CELL_WIDTH = 128;
const int CELL_HEIGHT = 64;

const int TOP_LAYER_THICKNESS_DECORATING_CELLS = 4;
const int BOTTOM_LAYER_THICKNESS_DECORATING_CELLS = 2;
const int LEFT_LAYER_THICKNESS_DECORATING_CELLS = 2;
const int RIGHT_LAYER_THICKNESS_DECORATING_CELLS = 2;



//---------------------------------------------------------------------------------------------
//class BiomeProperties definition

class BiomeProperties {
private:
	const TextureName _textureName;
	const bool _isTransparent;
	const bool _isPassable;
	//const BiomeType _baseBiome;
	const TextureName _objectTextureName;
	//whiteList
	//blakList
public:
	/*
	argumets:
	#1 - texture name
	#2 - isTransparent (default value - true)
	#3 - isPassable (default value - true)
	#4 - base biome type (default value - BiomeType::NONE)
	*/
	BiomeProperties(TextureName textureName,
		bool isTransparent = true, //прозрачный
		bool isPassable = true, //проходимый
		//BiomeType baseBiome = BiomeType::NONE
		TextureName objectTextureName = TextureName::NONE

	) noexcept
		//: _textureName(textureName), _isTransparent(isTransparent), _isPassable(isPassable), _baseBiome(baseBiome)
		: _textureName(textureName), _isTransparent(isTransparent), _isPassable(isPassable), _objectTextureName(objectTextureName)
	{
		//_textureName = textureName;
	}

	TextureName getTextureName() const {
		return _textureName;
	}

	TextureName getObjectTextureName() const {
		return _objectTextureName;
	}

	/*BiomeType getBaseBiome() const {
		return _baseBiome;
	}*/
};



const map<const BiomeType, const BiomeProperties> biomesData = {
	{BiomeType::GRASS, BiomeProperties(TextureName::GRASS)},
	{BiomeType::STONE, BiomeProperties(TextureName::GRASS, false, false, TextureName::STONE)},
	{BiomeType::SMALL_STONE, BiomeProperties(TextureName::GRASS, true, false, TextureName::SMALL_STONE)},
	{BiomeType::BUSH, BiomeProperties(TextureName::GRASS, false, true, TextureName::BUSH)},
	{BiomeType::PINE, BiomeProperties(TextureName::GRASS, true, true, TextureName::PINE)}
};



/*const map<const BiomeType, const BiomeProperties> biomesData = {
	{BiomeType::GRASS, BiomeProperties(TextureName::GRASS)},
	{BiomeType::STONE, BiomeProperties(TextureName::STONE, false, false, BiomeType::GRASS)},
	{BiomeType::BUSH, BiomeProperties(TextureName::BUSH, true, false, BiomeType::GRASS)},
	{BiomeType::TREE, BiomeProperties(TextureName::STONE, false, false, BiomeType::GRASS)},
};*/



//---------------------------------------------------------------------------------------------
//class Cell definition

class Cell {
private:
	const int _xPos, _yPos;
	//Unit* _unit = nullptr;
	BiomeType _biome;
public:
	Cell(int xPos, int yPos, BiomeType biome) : _xPos(xPos), _yPos(yPos) {
		_biome = biome;
	}

	void drawSurface(const int xPixels, const int yPixels) const {
		int _xPixelsPos = 0, _yPixelsPos = 0;

		_xPixelsPos = xPixels + _xPos * (CELL_WIDTH * 0.75);

		if (_xPos % 2 == 0) { //чётный столбец
			_yPixelsPos = yPixels + (CELL_HEIGHT * 0.5) + _yPos * CELL_HEIGHT;
		} else { //нечётный стобец
			_yPixelsPos = yPixels + _yPos * CELL_HEIGHT; //на пол клетки выше на экране
		}
		
		const TextureName _textureName = biomesData.at(_biome).getTextureName();

		drawTexture(_xPixelsPos, _yPixelsPos, _textureName);

		int _battleFieldWidth, _battleFieldHeight;
		Battle::Instance().getBattleFieldProperties(&_battleFieldWidth, &_battleFieldHeight);

		glEnable(GL_LINE_SMOOTH);
		glColor4f(0, 0, 0, 0.5);
		glLineWidth(1);

		//glDisable(GL_LINE_SMOOTH); //изза горизонтальнйо линии

		if (_xPos >= LEFT_LAYER_THICKNESS_DECORATING_CELLS
			&& _xPos < _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS
			&& _yPos < _battleFieldHeight - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS
			&& _yPos >= TOP_LAYER_THICKNESS_DECORATING_CELLS
		) {
			//рисуем допоплнительные боковые линии
			//слева
			if (_xPos == LEFT_LAYER_THICKNESS_DECORATING_CELLS) {
				glBegin(GL_LINES);
				glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.25),	_yPixelsPos);
				glVertex2i(_xPixelsPos,							_yPixelsPos + (CELL_HEIGHT * 0.5));
				glEnd();
			}
			//справо
			else if (_xPos == _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS - 1) {
				glBegin(GL_LINES);
				glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.75),	_yPixelsPos);
				glVertex2i(_xPixelsPos + CELL_WIDTH,			_yPixelsPos + (CELL_HEIGHT * 0.5));
				glEnd();
			}

			//верхний уровень
			if (_yPos == TOP_LAYER_THICKNESS_DECORATING_CELLS) {
				if (_xPos % 2 == 0) { //чётные столбцы
					if (_xPos == LEFT_LAYER_THICKNESS_DECORATING_CELLS
						|| _xPos == _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS - 1
						) { //левый или правый
						glBegin(GL_LINES);
						glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.25), _yPixelsPos);
						glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.75), _yPixelsPos);
						glEnd();

						if (_xPos == LEFT_LAYER_THICKNESS_DECORATING_CELLS) { //левый столбец
							glBegin(GL_LINES);
							glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.75), _yPixelsPos);
							glVertex2i(_xPixelsPos + CELL_WIDTH, _yPixelsPos - (CELL_HEIGHT * 0.5));
							glEnd();
						}
						else { //правый
							glBegin(GL_LINES);
							glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.25), _yPixelsPos);
							glVertex2i(_xPixelsPos, _yPixelsPos - (CELL_HEIGHT * 0.5));
							glEnd();
						}
					}
					else { //чётные, не крайние стобцы
						//"верхняя" нижняя обводка
						glBegin(GL_LINE_STRIP);
						glVertex2i(_xPixelsPos, _yPixelsPos - (CELL_HEIGHT * 0.5));
						glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.25), _yPixelsPos);
						glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.75), _yPixelsPos);
						glVertex2i(_xPixelsPos + CELL_WIDTH, _yPixelsPos - (CELL_HEIGHT * 0.5));
						glEnd();
					}
				}
				else { //нечётные столбцы
					//линия сверху
					glBegin(GL_LINES);
					glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.25), _yPixelsPos);
					glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.75), _yPixelsPos);
					glEnd();
				}
			}

			//glEnable(GL_LINE_SMOOTH);

			//нижняя обводка
			glBegin(GL_LINE_STRIP);
			glVertex2i(_xPixelsPos, _yPixelsPos + (CELL_HEIGHT * 0.5));
			glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.25), _yPixelsPos + CELL_HEIGHT);
			glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.75), _yPixelsPos + CELL_HEIGHT);
			glVertex2i(_xPixelsPos + CELL_WIDTH, _yPixelsPos + (CELL_HEIGHT * 0.5));
			glEnd();
		}
	}

	void drawObject(const int xPixels, const int yPixels) const {
		const TextureName _objectTextureName = biomesData.at(_biome).getObjectTextureName();
		if (_objectTextureName != TextureName::NONE) {

			int _textureWidth = 0, _textureHeight = 0;
			getTextureProperties(_objectTextureName, &_textureWidth, &_textureHeight);

			int _xPixelsPos = 0, _yPixelsPos = 0;
			_xPixelsPos = xPixels + _xPos * (CELL_WIDTH * 0.75);
			if (_xPos % 2 == 0) { //чётный столбец
				_yPixelsPos = yPixels + (CELL_HEIGHT * 0.5) + _yPos * CELL_HEIGHT;
			}
			else { //нечётный стобец
				_yPixelsPos = yPixels + _yPos * CELL_HEIGHT;
			}

			//тень
			drawTexture(_xPixelsPos, _yPixelsPos, TextureName::SHADOW);

			//srand(_xPixelsPos * _yPixelsPos) //дял красивого хаоса
			drawTexture(_xPixelsPos + (CELL_WIDTH - _textureWidth) / 2, _yPixelsPos - _textureHeight + CELL_HEIGHT * 0.75, _objectTextureName);
		}	
	}

	const BiomeType getBiomeType() const {
		return _biome;
	}
};



/*//---------------------------------------------------------------------------------------------
//class Biome definition

class Biome {
	int textureId = 0;
	Biome** _biomesWhiteList = nullptr;
	Biome** _biomesBlackList = nullptr;

};*/



//---------------------------------------------------------------------------------------------
//class BattleField definition

class BattleField {
private:
	int _xPixelPos =  - CELL_WIDTH * 0.25, _yPixelPos = - CELL_HEIGHT * 0.5;
	int _width, _height;

	bool _isScrollingEnabled = false;
	int _xScrollingStartCursorPos = 0, _yScrollingStartCursorPos = 0;
	int _xScrollingStartPixelPos = _xPixelPos, _yScrollingStartPixelPos = _yPixelPos;

	Cell*** _battlefield = nullptr;

	//отрисовка поверхностей
	void drawCellSurfaces() const {
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				_battlefield[x][y]->drawSurface(_xPixelPos, _yPixelPos);
			}
		}
	}
	
	//отрисовка объектов и юнитов(первый раз)
	void drawCellObjects() const {
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				/*int _y;
				y < _height / 2 ? _y = y * 2 : _y = (y - _height / 2) * 2 + 1;*/
				int _x;
				x < _width / 2 ? _x = x * 2 + 1 : _x = (x - _width / 2) * 2;
				_battlefield[_x][y]->drawObject(_xPixelPos, _yPixelPos);
				//_battlefield[x][y]->drawUnit(_xPixels, _yPixels);
			}
		}
	}

	//отрисовка юнитов второй раз с прозрачностью для их видимости сквозь препятствия
	void drawCellUnitBacklight() const {
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA); //или отдельную функцию для Cell? типа Cell->drawUnitBacklight();
				//_battlefield[x][y]->drawUnit(_xPixels, _yPixels);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
		}
	}

public:
	BattleField() {
		srand(time(0));
		//_width = rand() % 10 + 15;
		//_height = rand() % 10 + 15;
		_width = rand() % 10 + 16 + LEFT_LAYER_THICKNESS_DECORATING_CELLS + RIGHT_LAYER_THICKNESS_DECORATING_CELLS;
		_height = rand() % 10 + 16 + TOP_LAYER_THICKNESS_DECORATING_CELLS + BOTTOM_LAYER_THICKNESS_DECORATING_CELLS;

		_battlefield = new Cell**[_width];
		for (int x = 0; x < _width; x++) {
			_battlefield[x] = new Cell*[_height];

			for (int y = 0; y < _height; y++) {
				int _biomeValue = rand() % 100;
				//BiomeType _biomeType = BiomeType::GRASS;

				//левые, правые и верхние декорации
				if (x < LEFT_LAYER_THICKNESS_DECORATING_CELLS 
					|| x >= _width - RIGHT_LAYER_THICKNESS_DECORATING_CELLS
					|| y < TOP_LAYER_THICKNESS_DECORATING_CELLS
				) {
					//сосенка
					if (0 <= _biomeValue && _biomeValue < 50) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::PINE);
					}

					//большой камушек
					else if (50 <= _biomeValue && _biomeValue < 70) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::STONE);
					}

					//кустик
					else if (70 <= _biomeValue && _biomeValue < 90) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::BUSH);
					}

					//маленький камушек
					else if (90 <= _biomeValue && _biomeValue < 100) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::SMALL_STONE);
					}
				}

				//нижние декорации
				else if (y >= _height - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS) {
					//кустик
					if (0 <= _biomeValue && _biomeValue < 60) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::BUSH);
					}

					//маленький камушек
					else if (60 <= _biomeValue && _biomeValue < 100) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::SMALL_STONE);
					}
				}

				//обычная карта
				else {
					//травка-муравка
					if (0 <= _biomeValue && _biomeValue < 80) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::GRASS);
					}

					//кустик
					else if (75 <= _biomeValue && _biomeValue < 90) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::BUSH);
					}

					//маленький камушек
					else if (90 <= _biomeValue && _biomeValue < 95) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::SMALL_STONE);
					}

					//большой камушек
					else if (95 <= _biomeValue && _biomeValue < 97) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::STONE);
					}

					//сосенка
					else if (97 <= _biomeValue && _biomeValue < 100) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::PINE);
					}
				}
			}
		}
	}

	~BattleField() {
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				delete _battlefield[x][y];
			}
			delete[] _battlefield[x];
		}
		delete[] _battlefield;
		_battlefield = nullptr;
	}

	void draw() {
		if (_isScrollingEnabled) {
			int _xCursorPos = 0, _yCursorPos = 0;
			UserInterface::Instance().getCursorPos(&_xCursorPos, &_yCursorPos);

			//cout << "_xScrollingStartPosition: " << _xScrollingStartCursorPos << endl;
			_xPixelPos = _xScrollingStartPixelPos + (_xCursorPos - _xScrollingStartCursorPos);
			_yPixelPos = _yScrollingStartPixelPos + (_yCursorPos - _yScrollingStartCursorPos);

			int _windowWidth = 0, _windowHeight = 0;
			UserInterface::Instance().getWindowSize(&_windowWidth, &_windowHeight);

			//ограничение прокрутки влево
			if (_xPixelPos > -CELL_WIDTH * 0.25) {
				//поправляем прокрутку для удобства в случае смены направления прокрутки
				_xScrollingStartCursorPos += _xPixelPos + CELL_WIDTH * 0.25;

				//упираемся в границу карты
				_xPixelPos = -CELL_WIDTH * 0.25;
			}

			//ограничение прокрутки вправо
			else if (_xPixelPos < - (_width * (CELL_WIDTH * 0.75) - _windowWidth)) {
				//поправляем прокрутку для удобства в случае смены направления прокрутки
				_xScrollingStartCursorPos += _xPixelPos + (_width * (CELL_WIDTH * 0.75) - _windowWidth);

				//упираемся в границу карты
				_xPixelPos = -(_width * (CELL_WIDTH * 0.75) - _windowWidth);
			}

			//ограничение прокрутки вверх
			if (_yPixelPos > -CELL_HEIGHT * 0.5) {
				//поправляем прокрутку для удобства в случае смены направления прокрутки
				_yScrollingStartCursorPos += _yPixelPos + CELL_HEIGHT * 0.5;

				//упираемся в границу карты
				_yPixelPos = -CELL_HEIGHT * 0.5;
			}

			//ограничение прокрутки вниз
			else if (_yPixelPos < - (_height * CELL_HEIGHT - _windowHeight)) {
				//поправляем прокрутку для удобства в случае смены направления прокрутки
				_yScrollingStartCursorPos += _yPixelPos + (_height * CELL_HEIGHT - _windowHeight);

				//упираемся в границу карты
				_yPixelPos = -(_height * CELL_HEIGHT - _windowHeight);
			}
		}

		//отрисовка поля битвы
		drawCellSurfaces();
		drawCellObjects();
	}

	void switchScrolling(bool scrollingState) {
		if (_isScrollingEnabled == false && scrollingState == true) {
			_isScrollingEnabled = true;

			int _xCursorPos, _yCursorPos;
			UserInterface::Instance().getCursorPos(&_xCursorPos, &_yCursorPos);

			_xScrollingStartCursorPos = _xCursorPos;
			_yScrollingStartCursorPos = _yCursorPos;

			_xScrollingStartPixelPos = _xPixelPos;
			_yScrollingStartPixelPos = _yPixelPos;
		}

		else if (_isScrollingEnabled == true && scrollingState == false) {
			_isScrollingEnabled = false;

			_xScrollingStartCursorPos = 0;
			_yScrollingStartCursorPos = 0;
		}
	}

	//void getProperties(int* xPixelPos, int* yPixelPos, int* width, int* height) const {
	void getProperties(int* width, int* height) const {
		//if (xPixelPos != nullptr) *xPixelPos = _xPixelPos;
		//if (yPixelPos != nullptr) *yPixelPos = _yPixelPos;

		if (width != nullptr) *width = _width;
		if (height != nullptr) *height = _height;
	}
	
	void updateAfterWindowResize() {
		int _windowWidth = 0, _windowHeight = 0;
		UserInterface::Instance().getWindowSize(&_windowWidth, &_windowHeight);

		//ограничение прокрутки влево
		if (_xPixelPos > -CELL_WIDTH * 0.25) {
			//упираемся в границу карты
			_xPixelPos = -CELL_WIDTH * 0.25;
		}

		//ограничение прокрутки вправо
		else if (_xPixelPos < -(_width * (CELL_WIDTH * 0.75) - _windowWidth)) {
			//упираемся в границу карты
			_xPixelPos = -(_width * (CELL_WIDTH * 0.75) - _windowWidth);
		}

		//ограничение прокрутки вверх
		if (_yPixelPos > -CELL_HEIGHT * 0.5) {
			//упираемся в границу карты
			_yPixelPos = -CELL_HEIGHT * 0.5;
		}

		//ограничение прокрутки вниз
		else if (_yPixelPos < -(_height * CELL_HEIGHT - _windowHeight)) {
			//упираемся в границу карты
			_yPixelPos = -(_height * CELL_HEIGHT - _windowHeight);
		}
	}

	//передвижение
};



//---------------------------------------------------------------------------------------------
//class Battle definition

Battle::Battle() = default;
Battle::~Battle() = default;

Battle& Battle::Instance() {
	static Battle singleton;
	return singleton;
}

void Battle::begin() {
	if (_battleField == nullptr) { //TODO
		_battleField = new BattleField();
	}
}

void Battle::end() {
	if (_battleField != nullptr) { //TODO
		delete _battleField;
		_battleField = nullptr;
		switchBattleFieldScrolling(false);
	}
}

void Battle::draw() {
	if (_battleField != nullptr) {
		_battleField->draw();
		//switchBattleFieldScrolling(false);
	}
}

void Battle::getBattleFieldProperties(int* width, int* height) const {
	if (_battleField != nullptr) {
		_battleField->getProperties(width, height);
	}
	else {
		if (width != nullptr) *width = 0;
		if (height != nullptr) *height = 0;
	}
}

//void Battle::scrollBattleField(int xPixels, int yPixels) {}

void Battle::switchBattleFieldScrolling(bool scrollingState) {
	//cout << "scrollingState: " << scrollingState << endl;
	if (_battleField != nullptr) {
		_battleField->switchScrolling(scrollingState);
	}
}

void Battle::updateBattleFieldAfterWindowResize() {
	if (_battleField != nullptr) {
		_battleField->updateAfterWindowResize();
	}
}



//---------------------------------------------------------------------------------------------
//class Unit definition

class Unit : ActiveGraphicItem {
	int _xPos = 0, _yPos = 0;
};



//---------------------------------------------------------------------------------------------
//functions

//void loadBiomes;