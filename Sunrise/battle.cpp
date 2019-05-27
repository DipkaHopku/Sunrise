#include "battle.h"

const float CELL_WIDTH = 128;
const float CELL_HEIGHT = 64;



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
		/*if (_xPos % 2 == 0) {
			_xPixelsPos = xPixels + _xPos * (CELL_WIDTH * 1.5);
		}*/
		_xPixelsPos = xPixels + _xPos * (CELL_WIDTH * 0.75);
		if (_xPos % 2 == 0) { //чётный столбец
			_yPixelsPos = yPixels + (CELL_HEIGHT * 0.5) + _yPos * CELL_HEIGHT;
		} else { //нечётный стобец
			_yPixelsPos = yPixels + _yPos * CELL_HEIGHT;
		}
		
		const TextureName _textureName = biomesData.at(_biome).getTextureName();

		drawTexture(_xPixelsPos, _yPixelsPos, _textureName);

		glEnable(GL_LINE_SMOOTH);
		glColor4f(0, 0, 0, 0.5);
		glLineWidth(1);
		glBegin(GL_LINE_STRIP);
		glVertex2i(_xPixelsPos,							_yPixelsPos + (CELL_HEIGHT * 0.5));
		glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.25),	_yPixelsPos + CELL_HEIGHT);
		glVertex2i(_xPixelsPos + (CELL_WIDTH * 0.75),	_yPixelsPos + CELL_HEIGHT);
		glVertex2i(_xPixelsPos + CELL_WIDTH,			_yPixelsPos + (CELL_HEIGHT * 0.5));
		glEnd();
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
	int _xPixels = 0, _yPixels = 0;
	int _width, _height;
	Cell*** _battlefield = nullptr;

	//отрисовка поверхностей
	void drawCellSurfaces() const {
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				_battlefield[x][y]->drawSurface(_xPixels, _yPixels);
			}
		}
	}
	
	//отрисовка объектов и юнитов(первый раз)
	void drawCellObjects() const {
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				int _y;
				y < _height / 2 ? _y = y * 2 : _y = (y - _height / 2) * 2 + 1;
				_battlefield[x][y]->drawObject(_xPixels, _yPixels);
			}
		}
	}

	//отрисовка юнитов второй раз с прозрачностью для их видимости сквозь препятствия
	void drawCellUnitBacklight() const {
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				//_battlefield[x][y]->draw(_xPixels, _yPixels);
			}
		}
	}

public:
	BattleField() {
		srand(time(0));
		_width = rand() % 10 + 15;
		_height = rand() % 10 + 15;

		_battlefield = new Cell**[_width];
		for (int x = 0; x < _width; x++) {
			_battlefield[x] = new Cell*[_height];

			for (int y = 0; y < _height; y++) {
				int _biomeValue = rand() % 100;
				//BiomeType _biomeType = BiomeType::GRASS;

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

	void draw() const {
		drawCellSurfaces();
		drawCellObjects();
	}
	//прокрутка карты
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
	}
}

void Battle::draw() {
	if (_battleField != nullptr) {
		_battleField->draw();
	}
}

void Battle::scrollBattleField(int xPixels, int yPixels) {
}



//---------------------------------------------------------------------------------------------
//class Unit definition

class Unit {
	int _xPos = 0, _yPos = 0;
};



//---------------------------------------------------------------------------------------------
//functions

//void loadBiomes;