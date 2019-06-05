#include "battle.h"

const map<const TextureName, const TextureName> textureBorders = {
	{TextureName::WIZARD, TextureName::WIZARD_BORDER}
};



//������ �� ��� �������� ���� �������� �� �������������� ��������, ��... ��
const int CELL_WIDTH = 128;
const int CELL_HEIGHT = 64;

const int TOP_LAYER_THICKNESS_DECORATING_CELLS = 4;
const int BOTTOM_LAYER_THICKNESS_DECORATING_CELLS = 2;
const int LEFT_LAYER_THICKNESS_DECORATING_CELLS = 2;
const int RIGHT_LAYER_THICKNESS_DECORATING_CELLS = 2;

const float PLAYER_COLOR_RGB[4] = { 0.235, 0.75, 0.235, 1 };
const float ENEMY_COLOR_RGB[4] = { 0.75, 0.235, 0.235, 1 };

//class Cell;



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
		bool isTransparent = true, //����������
		bool isPassable = true, //����������
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

	bool getPassability() const {
		return _isPassable;
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



const map<const UnitType, const TextureName> unitTypesData = {
	{UnitType::WIZARD, TextureName::WIZARD}
};



//---------------------------------------------------------------------------------------------
//class Unit definition

class Unit : public ActiveGraphicItem {
private:
	//��������������
	//bool _mouseOver = false;
	//void(*_onClickCallback)();
	UnitType _unitType;
	bool _isLookRight;
	unsigned int _player;
	//bool _isChoosen = false;

	//Cell* _cell;
	//const _textureName;

protected:
	//��������������
	//int _xPos = 0, _yPos = 0;

public:
	//��������������
	//virtual void onClick() const = 0;
	//virtual void onMouseOver(bool);

	Unit(UnitType unitType, int player) {
		_unitType = unitType;
		_player = player;
	}

	/*Unit(int xPos, int yPos, bool isLookRight, int player) {
		_xPos = xPos;
		_yPos = yPos;
		_isLookRight = isLookRight;
		_player = player;
	}*/

	//������� ���������� �������� ������ ���� ������, � ������� ���� �������� ����
	void draw(int xPixelPos, int yPixelPos) const {
		auto _unitTypeData = unitTypesData.find(_unitType); //�������� ������ �� ��������
		if (_unitTypeData != unitTypesData.end()) {
			TextureName _textureName = _unitTypeData->second;

			int _textureWidth, _textureHeight;
			getTextureProperties(_textureName, &_textureWidth, &_textureHeight);

			int _xPixelPos = xPixelPos + (CELL_WIDTH - _textureWidth) / 2;
			int _yPixelPos = yPixelPos - _textureHeight + CELL_HEIGHT * 0.625;

			drawTexture(_xPixelPos, _yPixelPos,
				_textureName, (ActiveGraphicItem*)this,
				TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1,
				!_isLookRight
			);

			//UserInterface::Instance().setActiveGraphicItemTextureControlField(_xPixelPos, _yPixelPos, _textureName, (ActiveGraphicItem*)this);
		}
	}

	//������� ���������� �������� ������ ���� battleField
	void drawBacklight(const int xPixelPos, const int yPixelPos) const {
		auto _unitTypeData = unitTypesData.find(_unitType);
		if (_unitTypeData != unitTypesData.end()) {
			//��������� ������� ������
			int _xPixelPos = 0, _yPixelPos = 0;
			_xPixelPos = xPixelPos + _xPos * (CELL_WIDTH * 0.75);
			if (_xPos % 2 == 0) { //������ �������
				_yPixelPos = yPixelPos + (CELL_HEIGHT * 0.5) + _yPos * CELL_HEIGHT;
			}
			else { //�������� ������
				_yPixelPos = yPixelPos + _yPos * CELL_HEIGHT;
			}

			//��������� ������� �����
			TextureName _textureName = _unitTypeData->second;

			int _textureWidth, _textureHeight;
			getTextureProperties(_textureName, &_textureWidth, &_textureHeight);

			_xPixelPos += (CELL_WIDTH - _textureWidth) / 2;
			_yPixelPos += - _textureHeight + CELL_HEIGHT * 0.625;

			/*//��������� ��� ���������/���������
			int _borderWidth = 4;
			if (_mouseOver) {
				float _widthFactor = (float)(_textureWidth + _borderWidth) / _textureWidth;
				float _heightFactor = (float)(_textureHeight + _borderWidth) / _textureHeight;

				//glEnable(GL_ALPHA_TEST);
				//glAlphaFunc(GL_NOTEQUAL, 0);

				//glEnable(GL_STENCIL_TEST);

				//������ ������ � ���� �������
				glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
				glStencilFunc(GL_NEVER, 2, 255);

				//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);

				//������� ������
				drawTexture(_xPixelPos, _yPixelPos - ((float)_textureHeight * _widthFactor - _textureHeight) / 2,
					_textureName, nullptr,
					TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, _widthFactor,
					!_isLookRight
				);

				//������� �����
				drawTexture(_xPixelPos - _borderWidth, _yPixelPos - ((float)_textureHeight * _widthFactor - _textureHeight) / 2,
					_textureName, nullptr,
					TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, _widthFactor,
					!_isLookRight
				);

				//������� �����
				drawTexture(_xPixelPos - ((float)_textureWidth * _heightFactor - _textureWidth) / 2, _yPixelPos ,
					_textureName, nullptr,
					TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, _heightFactor,
					!_isLookRight
				);

				//������� ������
				drawTexture(_xPixelPos - ((float)_textureWidth * _heightFactor - _textureWidth) / 2, _yPixelPos - _borderWidth,
					_textureName, nullptr,
					TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, _heightFactor,
					!_isLookRight
				);

				//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

				//�������� ��� �������
				//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				//glStencilFunc(GL_ALWAYS, 3, 255);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				glStencilFunc(GL_NOTEQUAL, 1, 255);
			}*/

			/*glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_NOTEQUAL, 0);

			glEnable(GL_STENCIL_TEST);

			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_NOTEQUAL, 1, 255);

			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA); //�������� � ���������� �������
			drawTexture(_xPixelPos, _yPixelPos,
				_textureName, nullptr,
				TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1,
				!_isLookRight, false, 0.45
			);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glDisable(GL_STENCIL_TEST);

			glDisable(GL_ALPHA_TEST);*/

			/*if (_mouseOver) {
				//����������� ���������� �������
				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				glStencilFunc(GL_EQUAL, 2, 255);

				glColor4fv(BUTTON_FONT_COLOR_RGB);
				glBegin(GL_QUADS);
				glVertex2i(_xPixelPos - _borderWidth, _yPixelPos - _borderWidth);
				glVertex2i(_xPixelPos + _borderWidth + _textureWidth, _yPixelPos - _borderWidth);
				glVertex2i(_xPixelPos + _borderWidth + _textureWidth, _yPixelPos + _borderWidth + _textureHeight);
				glVertex2i(_xPixelPos - _borderWidth, _yPixelPos + _borderWidth + _textureHeight);
				glEnd();

				//glDisable(GL_STENCIL_TEST);

				//glDisable(GL_ALPHA_TEST);
			}*/
			
			const bool _isChosen = this == Battle::Instance().getChosenUnit();

			//��������� ��� ���������/���������
			if (_isChosen || _mouseOver) {
				auto textureBorder = textureBorders.find(_textureName);
				if (textureBorder != textureBorders.end()) {
					TextureName _textureBorderName = textureBorder->second;

					int _textureBorderWidth, _textureBorderHeight;
					getTextureProperties(_textureBorderName, &_textureBorderWidth, &_textureBorderHeight);

					int _xBorderPixelPos = _xPixelPos - (_textureBorderWidth - _textureWidth) / 2;
					int _yBorderPixelPos = _yPixelPos - (_textureBorderHeight - _textureHeight) / 2;

					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_NOTEQUAL, 0);

					glEnable(GL_STENCIL_TEST);

					glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
					glStencilFunc(GL_NOTEQUAL, 255, 255);

					//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
					//glStencilFunc(GL_ALWAYS, 2, 255);

					/*if (_isChosen) {
						glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
						glStencilFunc(GL_ALWAYS, 127, 255);
					}
					else { //_mouseOver
						glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
						glStencilFunc(GL_NOTEQUAL, 255, 255);
					}*/

					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
					//����� �������
					drawTexture(_xBorderPixelPos, _yBorderPixelPos,
						_textureBorderName, nullptr,
						TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1,
						!_isLookRight, false, 1
					);
					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

					glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
					if (_isChosen) {
						//glStencilFunc(GL_EQUAL, 127, 255);
						glStencilFunc(GL_EQUAL, 1, 255);
						glColor4f(1, 1, 1, 1);
					}
					else {
						glStencilFunc(GL_EQUAL, 1, 255);
						//glStencilFunc(GL_EQUAL, 1, 0);
						//if (_player == 0) glColor4f(0.235, 0.75, 0.235, 1);
						//else glColor4f(0.75, 0.235, 0.235, 1);
						if (_player == 0) glColor4fv(PLAYER_COLOR_RGB);
						else glColor4fv(ENEMY_COLOR_RGB);
					}
					
					//��������� ������
					glBegin(GL_QUADS);
					glVertex2i(_xBorderPixelPos - 100,						_yBorderPixelPos);
					glVertex2i(_xBorderPixelPos + _textureBorderWidth,	_yBorderPixelPos);
					glVertex2i(_xBorderPixelPos + _textureBorderWidth,	_yBorderPixelPos + _textureBorderHeight);
					glVertex2i(_xBorderPixelPos - 100,						_yBorderPixelPos + _textureBorderHeight);
					glEnd();

					//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
					//glStencilFunc(GL_EQUAL, 2, 255);
					//glStencilFunc(GL_NOTEQUAL, 1, 255);

					//����� ���� ��� ��� ������ �����
					drawTexture(_xPixelPos, _yPixelPos,
						_textureName, nullptr,
						TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1,
						!_isLookRight, false, 1
					);

					//��� backlight`�� ������ ������, ���� ��� �� �������� �� ��������
					//glEnable(GL_ALPHA_TEST);
					//glAlphaFunc(GL_NOTEQUAL, 0);

					//glEnable(GL_STENCIL_TEST);

					glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
					glStencilFunc(GL_NEVER, 255, 255); //�� ��������� 0, 0? //������ �������� ��� ���� ����� 0000000?
					//glStencilFunc(GL_NEVER, 1, 0);
					//glStencilFunc(GL_NEVER, 0, 255);
					//glStencilFunc(GL_NEVER, 1, 1);

					//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
					drawTexture(_xBorderPixelPos, _yBorderPixelPos,
						_textureBorderName, nullptr,
						TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1,
						!_isLookRight, false, 1
					);
					//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

					glDisable(GL_STENCIL_TEST);

					glDisable(GL_ALPHA_TEST);
				}
			}
			else {
				//�������������� ���������
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_NOTEQUAL, 0);

				glEnable(GL_STENCIL_TEST);

				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				//glStencilFunc(GL_NOTEQUAL, 1, 255);
				glStencilFunc(GL_NOTEQUAL, 255, 255);

				glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
				drawTexture(_xPixelPos, _yPixelPos,
					_textureName, nullptr,
					TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1,
					!_isLookRight, false, 0.45
				);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glDisable(GL_STENCIL_TEST);

				glDisable(GL_ALPHA_TEST);
			}
		}
	}

	void onClick() {
		//_onClickCallback();
		//_isChoosen = true;
		Battle::Instance().setChosenUnit(this);
	}

	void setPosition(int xPos, int yPos, bool isLookRight) {
		_xPos = xPos;
		_yPos = yPos;
		_isLookRight = isLookRight;
	}

	//void getPosition(int* xPos, int* yPos) const {
	void getPosition(int* xPos, int* yPos, bool* isLookRight) const {
		if (xPos != nullptr) *xPos = _xPos;
		if (yPos != nullptr) *yPos = _yPos;
		if (isLookRight != nullptr) *isLookRight = _isLookRight;
	}

	unsigned int getPlayer() const {
		return _player;
	}

	/*void move(Cell* cell) { //TODO //������� �������? ������ ������������ ���������? (������ �����)
		if (biomesData.at(cell->getBiomeType()).getPassability() && cell->getUnit() == nullptr) {
			//VVVVVV ��� �� �� ���� ������ ������ ���� ������� VVVVVV

			//������ � ������� ������ ������ �� ����


			//������������� � ����� ����� �������
			int _xCellPos, _yCellPos;
			cell->getPosition(&_xCellPos, &_yCellPos);
			_xPos = _xCellPos;
			_yPos = _yCellPos;

			//������������� ������ �� ���� ������� �����
			cell->setUnit(this);
		}
		//_battleField[xPos][yPos].setUnit(nullptr);
		//_battleField[newxPos][newyPos].setUnit(this);
	}*/
};



//---------------------------------------------------------------------------------------------
//class Cell definition

class Cell : public ActiveGraphicItem {
private:
	const int _xPos, _yPos;
	//Unit* _unit = nullptr;
	BiomeType _biome;

	//bool hasUnit = false;
	Unit* _unit = nullptr;
public:
	Cell(int xPos, int yPos, BiomeType biome) : _xPos(xPos), _yPos(yPos) {
		_biome = biome;
	}

	void drawSurface(const int xPixels, const int yPixels) {
		int _xPixelPos = 0, _yPixelPos = 0;

		_xPixelPos = xPixels + _xPos * (CELL_WIDTH * 0.75);

		if (_xPos % 2 == 0) { //������ �������
			_yPixelPos = yPixels + (CELL_HEIGHT * 0.5) + _yPos * CELL_HEIGHT;
		} else { //�������� ������
			_yPixelPos = yPixels + _yPos * CELL_HEIGHT; //�� ��� ������ ���� �� ������
		}
		
		const TextureName _textureName = biomesData.at(_biome).getTextureName();

		drawTexture(_xPixelPos, _yPixelPos, _textureName, (ActiveGraphicItem*) this);
		//drawTexture(_xPixelPos, _yPixelPos, _textureName);

		int _battleFieldWidth, _battleFieldHeight;
		Battle::Instance().getBattleFieldProperties(&_battleFieldWidth, &_battleFieldHeight);

		glEnable(GL_LINE_SMOOTH);
		glColor4f(0, 0, 0, 0.5);
		glLineWidth(1);

		//glDisable(GL_LINE_SMOOTH); //���� �������������� �����

		if (_xPos >= LEFT_LAYER_THICKNESS_DECORATING_CELLS
			&& _xPos < _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS
			&& _yPos < _battleFieldHeight - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS
			&& _yPos >= TOP_LAYER_THICKNESS_DECORATING_CELLS
		) {
			//������ ��������������� ������� �����
			//�����
			if (_xPos == LEFT_LAYER_THICKNESS_DECORATING_CELLS) {
				glBegin(GL_LINES);
				glVertex2i(_xPixelPos + (CELL_WIDTH * 0.25),	_yPixelPos);
				glVertex2i(_xPixelPos,							_yPixelPos + (CELL_HEIGHT * 0.5));
				glEnd();
			}
			//������
			else if (_xPos == _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS - 1) {
				glBegin(GL_LINES);
				glVertex2i(_xPixelPos + (CELL_WIDTH * 0.75),	_yPixelPos);
				glVertex2i(_xPixelPos + CELL_WIDTH,			_yPixelPos + (CELL_HEIGHT * 0.5));
				glEnd();
			}

			//������� �������
			if (_yPos == TOP_LAYER_THICKNESS_DECORATING_CELLS) {
				if (_xPos % 2 == 0) { //������ �������
					if (_xPos == LEFT_LAYER_THICKNESS_DECORATING_CELLS
						|| _xPos == _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS - 1
						) { //����� ��� ������
						glBegin(GL_LINES);
						glVertex2i(_xPixelPos + (CELL_WIDTH * 0.25), _yPixelPos);
						glVertex2i(_xPixelPos + (CELL_WIDTH * 0.75), _yPixelPos);
						glEnd();

						if (_xPos == LEFT_LAYER_THICKNESS_DECORATING_CELLS) { //����� �������
							glBegin(GL_LINES);
							glVertex2i(_xPixelPos + (CELL_WIDTH * 0.75), _yPixelPos);
							glVertex2i(_xPixelPos + CELL_WIDTH, _yPixelPos - (CELL_HEIGHT * 0.5));
							glEnd();
						}
						else { //������
							glBegin(GL_LINES);
							glVertex2i(_xPixelPos + (CELL_WIDTH * 0.25), _yPixelPos);
							glVertex2i(_xPixelPos, _yPixelPos - (CELL_HEIGHT * 0.5));
							glEnd();
						}
					}
					else { //������, �� ������� ������
						//"�������" ������ �������
						glBegin(GL_LINE_STRIP);
						glVertex2i(_xPixelPos, _yPixelPos - (CELL_HEIGHT * 0.5));
						glVertex2i(_xPixelPos + (CELL_WIDTH * 0.25), _yPixelPos);
						glVertex2i(_xPixelPos + (CELL_WIDTH * 0.75), _yPixelPos);
						glVertex2i(_xPixelPos + CELL_WIDTH, _yPixelPos - (CELL_HEIGHT * 0.5));
						glEnd();
					}
				}
				else { //�������� �������
					//����� ������
					glBegin(GL_LINES);
					glVertex2i(_xPixelPos + (CELL_WIDTH * 0.25), _yPixelPos);
					glVertex2i(_xPixelPos + (CELL_WIDTH * 0.75), _yPixelPos);
					glEnd();
				}
			}

			//glEnable(GL_LINE_SMOOTH);

			//������ �������
			glBegin(GL_LINE_STRIP);
			glVertex2i(_xPixelPos, _yPixelPos + (CELL_HEIGHT * 0.5));
			glVertex2i(_xPixelPos + (CELL_WIDTH * 0.25), _yPixelPos + CELL_HEIGHT);
			glVertex2i(_xPixelPos + (CELL_WIDTH * 0.75), _yPixelPos + CELL_HEIGHT);
			glVertex2i(_xPixelPos + CELL_WIDTH, _yPixelPos + (CELL_HEIGHT * 0.5));
			glEnd();
		}
	}

	void drawUnitAndObject(const int xPixels, const int yPixels) const {
		const TextureName _objectTextureName = biomesData.at(_biome).getObjectTextureName();

		int _battleFieldWidth, _battleFieldHeight;
		Battle::Instance().getBattleFieldProperties(&_battleFieldWidth, &_battleFieldHeight);

		bool _drawMovementMark = _mouseOver && Battle::Instance().getMovementMode()
			&& _xPos >= LEFT_LAYER_THICKNESS_DECORATING_CELLS
			&& _xPos < _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS
			&& _yPos < _battleFieldHeight - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS
			&& _yPos >= TOP_LAYER_THICKNESS_DECORATING_CELLS;

		//�� ������ ��� �� ����
		if (_objectTextureName != TextureName::NONE || _unit != nullptr) {

			//int _textureWidth = 0, _textureHeight = 0;
			//getTextureProperties(_objectTextureName, &_textureWidth, &_textureHeight);

			int _xPixelPos = 0, _yPixelPos = 0;
			_xPixelPos = xPixels + _xPos * (CELL_WIDTH * 0.75);
			if (_xPos % 2 == 0) { //������ �������
				_yPixelPos = yPixels + (CELL_HEIGHT * 0.5) + _yPos * CELL_HEIGHT;
			}
			else { //�������� ������
				_yPixelPos = yPixels + _yPos * CELL_HEIGHT;
			}

			if (_drawMovementMark) {
				if (biomesData.at(_biome).getPassability() && _unit == nullptr) {
					int _textureWidth = 0, _textureHeight = 0;
					getTextureProperties(TextureName::AVAILABLE_MOVEMENT_MARK, &_textureWidth, &_textureHeight);
					drawTexture(_xPixelPos + (CELL_WIDTH - _textureWidth) / 2, _yPixelPos + (CELL_HEIGHT - _textureHeight) / 2, TextureName::AVAILABLE_MOVEMENT_MARK);
				}
				else {
					int _textureWidth = 0, _textureHeight = 0;
					getTextureProperties(TextureName::NOT_AVAILABLE_MOVEMENT_MARK, &_textureWidth, &_textureHeight);
					drawTexture(_xPixelPos + (CELL_WIDTH - _textureWidth) / 2, _yPixelPos + (CELL_HEIGHT - _textureHeight) / 2, TextureName::NOT_AVAILABLE_MOVEMENT_MARK);
				}
			}

			//����
			drawTexture(_xPixelPos, _yPixelPos, TextureName::SHADOW);

			if (_unit != nullptr) {
				_unit->draw(_xPixelPos, _yPixelPos);
			}

			if (_objectTextureName != TextureName::NONE) {
				int _textureWidth = 0, _textureHeight = 0;
				getTextureProperties(_objectTextureName, &_textureWidth, &_textureHeight);
				//srand(_xPixelsPos * _yPixelsPos) //��� ��������� �����
				drawTexture(_xPixelPos + (CELL_WIDTH - _textureWidth) / 2, _yPixelPos - _textureHeight + CELL_HEIGHT * 0.75, _objectTextureName);
			}
		}

		//�������� ������������ � ������ ������ ������ ����
		else if (_drawMovementMark) {
			int _xPixelPos = 0, _yPixelPos = 0;
			_xPixelPos = xPixels + _xPos * (CELL_WIDTH * 0.75);
			if (_xPos % 2 == 0) { //������ �������
				_yPixelPos = yPixels + (CELL_HEIGHT * 0.5) + _yPos * CELL_HEIGHT;
			}
			else { //�������� ������
				_yPixelPos = yPixels + _yPos * CELL_HEIGHT;
			}

			int _textureWidth = 0, _textureHeight = 0;
			getTextureProperties(TextureName::AVAILABLE_MOVEMENT_MARK, &_textureWidth, &_textureHeight);
			drawTexture(_xPixelPos + (CELL_WIDTH - _textureWidth) / 2, _yPixelPos + (CELL_HEIGHT - _textureHeight) / 2, TextureName::AVAILABLE_MOVEMENT_MARK);
		}
	}

	void getPosition(int* xPos, int* yPos) const {
		if (xPos != nullptr) *xPos = _xPos;
		if (yPos != nullptr) *yPos = _yPos;
	}

	const BiomeType getBiomeType() const {
		return _biome;
	}

	void setUnit(Unit* unit) {
		_unit = unit;
	}

	Unit* getUnit() const {
		return _unit;
	}

	virtual void onMouseOver(bool mouseOver) override {
		//_mouseOver = mouseOver;
		ActiveGraphicItem::onMouseOver(mouseOver);
		
		if (mouseOver) {
			Battle::Instance().setCellUnderMouse(this);
		}
		else {
			Battle::Instance().setCellUnderMouse(nullptr);
		}

		/*if (Battle::Instance().getMovementMode() == true) {
			if (mouseOver) {
				Battle::Instance().setMovementMarkCell(this);
			}
			else {
				Battle::Instance().setMovementMarkCell(nullptr);
			}
		}*/
	}

	void onClick() {
		Battle::Instance().switchBattleFieldScrolling(true);
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

	//��������� ������������
	void drawCellSurfaces() const {
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				_battlefield[x][y]->drawSurface(_xPixelPos, _yPixelPos);
			}
		}
	}
	
	/*//��������� ������
	void drawCellUnits() const {
		//while(true) {
		for (int i = 0;;i++) {
			Unit* const _unit = Battle::Instance().getUnitByIndex(i);
			if (_unit == nullptr) break;

			_unit->draw(_xPixelPos, _yPixelPos);
		}
	}*/

	//��������� ��������
	void drawCellUnitsAndObjects() const {
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				int _x;
				x < _width / 2 ? _x = x * 2 + 1 : _x = (x - _width / 2) * 2;
				_battlefield[_x][y]->drawUnitAndObject(_xPixelPos, _yPixelPos);
			}
		}
	}

	/*//��������� ������ ������ ��� � ������������� ��� �� ��������� ������ �����������
	void drawCellUnitBacklights() const {
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				//_battlefield[x][y]->drawUnit(_xPixels, _yPixels);
			}
		}
	}*/

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

				//�����, ������ � ������� ���������
				if (x < LEFT_LAYER_THICKNESS_DECORATING_CELLS 
					|| x >= _width - RIGHT_LAYER_THICKNESS_DECORATING_CELLS
					|| y < TOP_LAYER_THICKNESS_DECORATING_CELLS
				) {
					//�������
					if (0 <= _biomeValue && _biomeValue < 50) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::PINE);
					}

					//������� �������
					else if (50 <= _biomeValue && _biomeValue < 70) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::STONE);
					}

					//������
					else if (70 <= _biomeValue && _biomeValue < 90) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::BUSH);
					}

					//��������� �������
					else if (90 <= _biomeValue && _biomeValue < 100) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::SMALL_STONE);
					}
				}

				//������ ���������
				else if (y >= _height - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS) {
					//������
					if (0 <= _biomeValue && _biomeValue < 60) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::BUSH);
					}

					//��������� �������
					else if (60 <= _biomeValue && _biomeValue < 100) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::SMALL_STONE);
					}
				}

				//������� �����
				else {
					//������-�������
					if (0 <= _biomeValue && _biomeValue < 80) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::GRASS);
					}

					//������
					else if (75 <= _biomeValue && _biomeValue < 90) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::BUSH);
					}

					//��������� �������
					else if (90 <= _biomeValue && _biomeValue < 95) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::SMALL_STONE);
					}

					//������� �������
					else if (95 <= _biomeValue && _biomeValue < 97) {
						_battlefield[x][y] = new Cell(x, y, BiomeType::STONE);
					}

					//�������
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

			//����������� ��������� �����
			if (_xPixelPos > -CELL_WIDTH * 0.25) {
				//���������� ��������� ��� �������� � ������ ����� ����������� ���������
				_xScrollingStartCursorPos += _xPixelPos + CELL_WIDTH * 0.25;

				//��������� � ������� �����
				_xPixelPos = -CELL_WIDTH * 0.25;
			}

			//����������� ��������� ������
			else if (_xPixelPos < - (_width * (CELL_WIDTH * 0.75) - _windowWidth)) {
				//���������� ��������� ��� �������� � ������ ����� ����������� ���������
				_xScrollingStartCursorPos += _xPixelPos + (_width * (CELL_WIDTH * 0.75) - _windowWidth);

				//��������� � ������� �����
				_xPixelPos = -(_width * (CELL_WIDTH * 0.75) - _windowWidth);
			}

			//����������� ��������� �����
			if (_yPixelPos > -CELL_HEIGHT * 0.5) {
				//���������� ��������� ��� �������� � ������ ����� ����������� ���������
				_yScrollingStartCursorPos += _yPixelPos + CELL_HEIGHT * 0.5;

				//��������� � ������� �����
				_yPixelPos = -CELL_HEIGHT * 0.5;
			}

			//����������� ��������� ����
			else if (_yPixelPos < - (_height * CELL_HEIGHT - _windowHeight)) {
				//���������� ��������� ��� �������� � ������ ����� ����������� ���������
				_yScrollingStartCursorPos += _yPixelPos + (_height * CELL_HEIGHT - _windowHeight);

				//��������� � ������� �����
				_yPixelPos = -(_height * CELL_HEIGHT - _windowHeight);
			}
		}

		//��������� ���� �����
		drawCellSurfaces();

		//��������� ��������(��������, ������...) � ������
		drawCellUnitsAndObjects();

		//��������� movementMark
		Battle::Instance().drawMovementMarkBacklight();

		//��������� ��������� (������ ��� � �������������) ������, ����� �� ���� ����� � �����, �� ������� ��� ������
		//drawCellUnitBacklights();
		Battle::Instance().drawUnitBacklights();
		//��������� ������ ������ ��� � ������������� ��� �� ��������� ������ �����������
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

	void getSize(int* width, int* height) const {
		if (width != nullptr) *width = _width;
		if (height != nullptr) *height = _height;
	}

	void getPosition(int* xPixelPos, int* yPixelPos) const {
		if (xPixelPos != nullptr) *xPixelPos = _xPixelPos;
		if (yPixelPos != nullptr) *yPixelPos = _yPixelPos;
	}
	
	void setPosition(int xPixelPos, int yPixelPos) {
		_xPixelPos = xPixelPos;
		_yPixelPos = yPixelPos;
		updatePos();
	}

	//���������� ������� ������ ����� ����� ��������� battlefield ������������ ����� ����
	void updatePos() {
		int _windowWidth = 0, _windowHeight = 0;
		UserInterface::Instance().getWindowSize(&_windowWidth, &_windowHeight);

		//����������� ��������� �����
		if (_xPixelPos > -CELL_WIDTH * 0.25) {
			//��������� � ������� �����
			_xPixelPos = -CELL_WIDTH * 0.25;
		}

		//����������� ��������� ������
		else if (_xPixelPos < -(_width * (CELL_WIDTH * 0.75) - _windowWidth)) {
			//��������� � ������� �����
			_xPixelPos = -(_width * (CELL_WIDTH * 0.75) - _windowWidth);
		}

		//����������� ��������� �����
		if (_yPixelPos > -CELL_HEIGHT * 0.5) {
			//��������� � ������� �����
			_yPixelPos = -CELL_HEIGHT * 0.5;
		}

		//����������� ��������� ����
		else if (_yPixelPos < -(_height * CELL_HEIGHT - _windowHeight)) {
			//��������� � ������� �����
			_yPixelPos = -(_height * CELL_HEIGHT - _windowHeight);
		}
	}

	void spawnUnit(Unit* unit, const int xPos, const int yPos, bool isLookRight) {
		//biomesData.at(_biome).getPassability() && _unit == nullptr
		bool _cellIsFree = false;
		Cell* _cell = _battlefield[xPos][yPos];
		_cellIsFree = biomesData.at(_cell->getBiomeType()).getPassability() && _cell->getUnit() == nullptr;

		int _xPos = xPos, _yPos = yPos;
		int _searchRadius = 1;
		while (!_cellIsFree) {
			bool noFreeCells = true;

			for (int i = 1; i <= 6 * _searchRadius; i++) {
				AdjacentCellRelativePos adjacentCellRelativePos;

				int _stage = (i + _searchRadius - 1) / _searchRadius;

				if (i == 1) {
					adjacentCellRelativePos = AdjacentCellRelativePos::TOP; //������ 1 ���
				}
				else if (_stage == 1) {
					adjacentCellRelativePos = AdjacentCellRelativePos::RIGHT_TOP; //(_searchRadius - 1) ���
				}
				else if (_stage == 2) {
					adjacentCellRelativePos = AdjacentCellRelativePos::RIGHT_BOTTOM; //_searchRadius ���
				}
				else if (_stage == 3) {
					adjacentCellRelativePos = AdjacentCellRelativePos::BOTTOM; //_searchRadius ���
				}
				else if (_stage == 4) {
					adjacentCellRelativePos = AdjacentCellRelativePos::LEFT_BOTTOM; //_searchRadius ���
				}
				else if (_stage == 5) {
					adjacentCellRelativePos = AdjacentCellRelativePos::LEFT_TOP; //_searchRadius ���
				}
				else if (_stage == 6) {
					adjacentCellRelativePos = AdjacentCellRelativePos::TOP; //_searchRadius ���
				}

				//�������� ���������� �������� ������ � ���� ������ ���� ��� ��������� 
				_cell = getAdjacentCell(&_xPos, &_yPos, adjacentCellRelativePos);

				if (_cell != nullptr) {
					noFreeCells = false; //���� ������� ���� �� ���� ������ ����, �� ������ ������ � ��������� ������� ����� �����
					_cellIsFree = biomesData.at(_cell->getBiomeType()).getPassability() && _cell->getUnit() == nullptr;
				}

				if (_cellIsFree) break;
			}

			//�������� �� �� ��� �� �� ����� �� ������� ���� //�������� ���������
			if (noFreeCells) {
				cerr << "no free cells for unit spawn" << endl;
				return;
			}

			_searchRadius++;
		}

		if (_cellIsFree) {
			unit->setPosition(_xPos, _yPos, isLookRight);
			_cell->setUnit(unit);
			//_battlefield[xPos][yPos]->setUnit(unit);
		}
	}



	//�������� �������� ������ � � ����������, ���� ���� ������ ��� ����. ������ ������������ ������(��������) ��������� �� ��������� ������
	Cell* getAdjacentCell(int* xPos, int* yPos, AdjacentCellRelativePos adjacentCellRelativePos) {
		if (!Battle::Instance().getAdjacentCellPos(xPos, yPos, adjacentCellRelativePos)) return nullptr;

		if (*xPos < LEFT_LAYER_THICKNESS_DECORATING_CELLS
			|| *xPos >= _width - RIGHT_LAYER_THICKNESS_DECORATING_CELLS
			|| *yPos < TOP_LAYER_THICKNESS_DECORATING_CELLS
			|| *yPos >= _height - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS
			) {
			return nullptr;
		}
		else {
			return _battlefield[*xPos][*yPos];
		}
	}



	//�������������, ��� ������������� ���� ������ ������������� �� ������������ ������
	void moveUnit(Unit* unit, Cell* destinationCell) {
		if (unit != nullptr && destinationCell != nullptr
			&& biomesData.at(destinationCell->getBiomeType()).getPassability() 
			&& destinationCell->getUnit() == nullptr
		) {
			//�������� ���������� ������ ����������
			int _xDestinationCellPos, _yDestinationCellPos;
			destinationCell->getPosition(&_xDestinationCellPos, &_yDestinationCellPos);

			int _battleFieldWidth, _battleFieldHeight;
			Battle::Instance().getBattleFieldProperties(&_battleFieldWidth, &_battleFieldHeight);

			bool _moveUnit = 
				_xDestinationCellPos >= LEFT_LAYER_THICKNESS_DECORATING_CELLS
				&& _xDestinationCellPos < _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS
				&& _yDestinationCellPos < _battleFieldHeight - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS
				&& _yDestinationCellPos >= TOP_LAYER_THICKNESS_DECORATING_CELLS;

			if (_moveUnit) {
				//������ � ������� ������ ������ �� ����
				int _xUnitPos, _yUnitPos;
				bool _unitIsLookRight;
				unit->getPosition(&_xUnitPos, &_yUnitPos, &_unitIsLookRight);
				_battlefield[_xUnitPos][_yUnitPos]->setUnit(nullptr);

				//������������� ������ ���������� ����
				destinationCell->setUnit(unit);

				//������������� � ����� ����� �������
				if (_xUnitPos < _xDestinationCellPos) _unitIsLookRight = true;
				else if (_xUnitPos > _xDestinationCellPos) _unitIsLookRight = false;

				unit->setPosition(_xDestinationCellPos, _yDestinationCellPos, _unitIsLookRight);

				//��������� ����� ��� �������
				Battle::Instance().sortUnitsByBacklightsDrawOrder();
			}
		}
		//_battleField[xPos][yPos].setUnit(nullptr);
		//_battleField[newxPos][newyPos].setUnit(this);
	}
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
		_turn = 0;

		spawnUnit(); //TODO DEBUG
	}
}

void Battle::end() {
	//������� ����
	if (_battleField != nullptr) { //TODO
		delete _battleField;
		_battleField = nullptr;
		switchBattleFieldScrolling(false);
	}

	_movementMode = false;
	//_movementMarkCell = nullptr;
	_cellUnderMouse = nullptr;

	//������� ������
	for (int i = 0; i < _units.size(); i++) {
		delete _units[i];
	}
	_units.clear();
	_chosenUnit = nullptr;
}

void Battle::draw() {
	if (_battleField != nullptr) {
		_battleField->draw();
		//switchBattleFieldScrolling(false);
	}
}

void Battle::getBattleFieldProperties(int* width, int* height) const {
	if (_battleField != nullptr) {
		_battleField->getSize(width, height);
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
		//UserInterface::Instance().switchOnMouseEventHandlingMode(!scrollingState);
	}
}

void Battle::updateBattleFieldAfterWindowResize() {
	if (_battleField != nullptr) {
		_battleField->updatePos();
	}
}

void Battle::focusVisionOnCell(Cell* cell) {
	int _xCellPos, _yCellPos;
	cell->getPosition(&_xCellPos, &_yCellPos);

	int _xPixelVisionPos, _yPixelVisionPos;

	//�������� ���������� ������ ������������ battleField
	_xPixelVisionPos = _xCellPos * (CELL_WIDTH * 0.75);

	if (_xCellPos % 2 == 0) { //������ �������
		_yPixelVisionPos = (CELL_HEIGHT * 0.5) + _yCellPos * CELL_HEIGHT;
	}
	else { //�������� ������
		_yPixelVisionPos = _yCellPos * CELL_HEIGHT; //�� ��� ������ ���� �� ������
	}

	//�������� ���������� ������ ������ ������������ battleField
	_xPixelVisionPos += CELL_WIDTH * 0.25;
	_yPixelVisionPos += CELL_HEIGHT * 0.5;

	int _windowWidth, _windowHeight;
	UserInterface::Instance().getWindowSize(&_windowWidth, &_windowHeight);

	//�������� ���������� battleField ������������ ���� � ������� � ������
	_xPixelVisionPos = _windowWidth / 2 - _xPixelVisionPos;
	_yPixelVisionPos = _windowHeight / 2 - _yPixelVisionPos;

	_battleField->setPosition(_xPixelVisionPos, _yPixelVisionPos);
}

void Battle::focusVisionOnCell(const int xCellPos, const int yCellPos) {
	int _battleFieldWidth, _battleFieldHeight;
	_battleField->getSize(&_battleFieldWidth, &_battleFieldHeight);

	if (0 > xCellPos || xCellPos >= _battleFieldWidth
		|| 0 > yCellPos || yCellPos >= _battleFieldHeight
	) {
		return;
	}

	int _xPixelVisionPos, _yPixelVisionPos;

	//�������� ���������� ������ ������������ battleField
	_xPixelVisionPos = xCellPos * (CELL_WIDTH * 0.75);

	if (xCellPos % 2 == 0) { //������ �������
		_yPixelVisionPos = (CELL_HEIGHT * 0.5) + yCellPos * CELL_HEIGHT;
	}
	else { //�������� ������
		_yPixelVisionPos = yCellPos * CELL_HEIGHT; //�� ��� ������ ���� �� ������
	}

	//�������� ���������� ������ ������ ������������ battleField
	_xPixelVisionPos += CELL_WIDTH * 0.25;
	_yPixelVisionPos += CELL_HEIGHT * 0.5;

	int _windowWidth, _windowHeight;
	UserInterface::Instance().getWindowSize(&_windowWidth, &_windowHeight);

	//�������� ���������� battleField ������������ ���� � ������� � ������
	_xPixelVisionPos = _windowWidth / 2 - _xPixelVisionPos;
	_yPixelVisionPos = _windowHeight / 2 - _yPixelVisionPos;

	_battleField->setPosition(_xPixelVisionPos, _yPixelVisionPos);
}

//������ �������� � ������� ����������� ���������, ������(���������� ������� �� ��������) � �������, �� ������ ����� ������������ //������������ ��� ���������� ��� ������ �� ������� - �������� ����� �������, �� ���� ����������� ��� ��������� ������� �����
void Battle::sortUnitsByBacklightsDrawOrder() {
	//��������� ������� �� yPos
	for (int i = 0; i < _units.size() - 1; i++) {
		int priority_yPos;
		_units[i]->getPosition(nullptr, &priority_yPos, nullptr);
		int priority_index = i;

		for (int j = i + 1; j < _units.size(); j++) {
			int yPos;
			_units[j]->getPosition(nullptr, &yPos, nullptr);

			//���� ����� ��������� ������� ����� yPos ������
			if (priority_yPos < yPos) {
				priority_yPos = yPos;
				priority_index = j;
			}

			//���� ����� ������� ����� ����� �� yPos ��� � ������ ����� ��������� �� xPos
			else if (priority_yPos == yPos) {

				//xPos �������� ������������� ��� ��������� �����
				int priority_xPos;
				_units[priority_index]->getPosition(&priority_xPos, nullptr, nullptr);

				//xPos ��������� �� ����� ������� ���������
				int xPos;
				_units[j]->getPosition(&xPos, nullptr, nullptr);

				//������ xPos ����� ���������
				if (priority_xPos % 2 == 1 && xPos % 2 == 0) {
					priority_yPos = yPos;
					priority_index = j;
				}

				//���� �� �������� ���������, �� ����� ���� ������� ���������� � ������� ������� �� ����� ��������
				//���� ������� ������������ ���� ����� ������ xPos, � �������� ��������, �� ������ ������ �� ����
			}
		}

		if (priority_index != i) {
			swap(_units[priority_index], _units[i]);
		}
	}
}

void Battle::drawMovementMarkBacklight() {
	//if (_movementMarkCell != nullptr) {
	if (_movementMode && _cellUnderMouse != nullptr) {
		int _xPos, _yPos;
		//_movementMarkCell->getPosition(&_xPos, &_yPos);
		_cellUnderMouse->getPosition(&_xPos, &_yPos);

		int _battleFieldWidth, _battleFieldHeight;
		Battle::Instance().getBattleFieldProperties(&_battleFieldWidth, &_battleFieldHeight);

		bool _drawMovementMark = _movementMode
			&& _xPos >= LEFT_LAYER_THICKNESS_DECORATING_CELLS
			&& _xPos < _battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS
			&& _yPos < _battleFieldHeight - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS
			&& _yPos >= TOP_LAYER_THICKNESS_DECORATING_CELLS;

		if (_drawMovementMark) {
			int _xPixelBattleFieldPos, _yPixelBattleFieldPos;
			_battleField->getPosition(&_xPixelBattleFieldPos, &_yPixelBattleFieldPos);

			//�������� ������� ������, �� ������� ����� ���������� ����� ������������ ������ � ��������
			int _xPixelPos = 0, _yPixelPos = 0;
			_xPixelPos = _xPixelBattleFieldPos + _xPos * (CELL_WIDTH * 0.75);
			if (_xPos % 2 == 0) { //������ �������
				_yPixelPos = _yPixelBattleFieldPos + (CELL_HEIGHT * 0.5) + _yPos * CELL_HEIGHT;
			}
			else { //�������� ������
				_yPixelPos = _yPixelBattleFieldPos + _yPos * CELL_HEIGHT;
			}

			//�������� ��� ����� ������� ��� ���� ��������
			TextureName _movementMarkTextureName;
			//if (biomesData.at(_movementMarkCell->getBiomeType()).getPassability() && _movementMarkCell->getUnit() == nullptr) {
			if (biomesData.at(_cellUnderMouse->getBiomeType()).getPassability() && _cellUnderMouse->getUnit() == nullptr) {
				_movementMarkTextureName = TextureName::AVAILABLE_MOVEMENT_MARK;
			}
			else {
				_movementMarkTextureName = TextureName::NOT_AVAILABLE_MOVEMENT_MARK;
			}

			int _textureWidth = 0, _textureHeight = 0;
			getTextureProperties(_movementMarkTextureName, &_textureWidth, &_textureHeight);

			//������ ����� ��������������
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_NOTEQUAL, 0);

			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
			drawTexture(
				_xPixelPos + (CELL_WIDTH - _textureWidth) / 2,
				_yPixelPos + (CELL_HEIGHT - _textureHeight) / 2,
				_movementMarkTextureName,
				nullptr,
				TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1,
				false, false, 0.6
			);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glDisable(GL_ALPHA_TEST);
		}
	}
}

void Battle::drawUnitBacklights() {
	int _xPixelBattleFieldPos, _yPixelBattleFieldPos;
	_battleField->getPosition(&_xPixelBattleFieldPos, &_yPixelBattleFieldPos);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_NOTEQUAL, 0);

	//glEnable(GL_STENCIL_TEST);

	//glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA); //������ ����������� ������

	for (int i = 0; i < _units.size(); i++) {
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glStencilFunc(GL_NOTEQUAL, 1, 255);

		_units[i]->drawBacklight(_xPixelBattleFieldPos, _yPixelBattleFieldPos);
	}

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//����� ���� �� ��� ��� ����� ���������������� ������ ������� ������ �������� ������, �������� �����, �� ��� �� �� ����� ������� � ��� ���� ������ �������
	/*glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_EQUAL, 1, 255);

	int _xPos = 0, _yPos = 0;
	int _width = 800, _height = 600;
	glColor4f(1, 1, 1, 0.1);
	glBegin(GL_QUADS);
	glVertex2i(_xPos, _yPos);
	glVertex2i(_xPos + _width, _yPos);
	glVertex2i(_xPos + _width, _yPos + _height);
	glVertex2i(_xPos, _yPos + _height);
	glEnd();*/

	//glDisable(GL_STENCIL_TEST);

	//glDisable(GL_ALPHA_TEST);
}

void Battle::setChosenUnit(Unit* unit) {
	if (unit->getPlayer() == 0) {
		_chosenUnit = unit;
	}
}

Unit * Battle::getChosenUnit() {
	return _chosenUnit;
}

void Battle::switchMovementMode(bool movementMode) {
	if (_chosenUnit != nullptr) {
		_movementMode = movementMode;

		if (movementMode == false) {
			_battleField->moveUnit(_chosenUnit, _cellUnderMouse);
		}
		//cout << "changeMoveMode on:" << movementMode << endl;//debug
		/*if (movementMode == false) {
			//_chosenUnit->move(_movementMarkCell);
			//_battleField->move(_chosenUnit);
			setMovementMarkCell(nullptr);
		}*/
	}
}

bool Battle::getMovementMode() {
	return _movementMode;
}

/*void Battle::setMovementMarkCell(Cell* movementMarkCell) {
	_movementMarkCell = movementMarkCell;
}*/

void Battle::setCellUnderMouse(Cell* cellUnderMouse) {
	_cellUnderMouse = cellUnderMouse;
}

/*Cell * Battle::getMovementMarkCell() {
	return _movementMarkCell;
}*/

Cell * Battle::getCellUnderMouse() {
	return _cellUnderMouse;
}

bool Battle::getAdjacentCellPos(int* xPos, int* yPos, AdjacentCellRelativePos adjacentCellRelativePos) {
	if (xPos == nullptr || yPos == nullptr) return false;

	const int _xPos = *xPos, _yPos = *yPos;

	switch (adjacentCellRelativePos) {
	case AdjacentCellRelativePos::LEFT_BOTTOM:
		*xPos = _xPos - 1;
		if (_xPos % 2 == 0) *yPos = _yPos + 1;
		break;

	case AdjacentCellRelativePos::LEFT_TOP:
		*xPos = _xPos - 1;
		if (_xPos % 2 == 1) *yPos = _yPos - 1;
		break;

	case AdjacentCellRelativePos::TOP:
		*yPos = _yPos - 1;
		break;

	case AdjacentCellRelativePos::RIGHT_TOP:
		*xPos = _xPos + 1;
		if (_xPos % 2 == 1) *yPos = _yPos - 1;
		break;

	case AdjacentCellRelativePos::RIGHT_BOTTOM:
		*xPos = _xPos + 1;
		if (_xPos % 2 == 0) *yPos = _yPos + 1;
		break;

	case AdjacentCellRelativePos::BOTTOM:
		*yPos = _yPos + 1;
		break;
	}

	return true;
}

void Battle::spawnUnit() { //TODO
	int battleFieldWidth, battleFieldHeight;
	_battleField->getSize(&battleFieldWidth, &battleFieldHeight);

	//Unit* _unit = new Unit(4, 10, true, 0);
	Unit* _unit = new Unit(UnitType::WIZARD, 0);
	_units.push_back(_unit);
	_battleField->spawnUnit(
		_unit, 
		LEFT_LAYER_THICKNESS_DECORATING_CELLS, 
		(battleFieldHeight - TOP_LAYER_THICKNESS_DECORATING_CELLS - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS) / 2 + TOP_LAYER_THICKNESS_DECORATING_CELLS, 
		true
	);

	//_unit = new Unit(5, 10, false, 0);
	_unit = new Unit(UnitType::WIZARD, 0);
	_units.push_back(_unit);
	_battleField->spawnUnit(
		_unit, 
		LEFT_LAYER_THICKNESS_DECORATING_CELLS, 
		(battleFieldHeight - TOP_LAYER_THICKNESS_DECORATING_CELLS - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS) / 2 + TOP_LAYER_THICKNESS_DECORATING_CELLS,
		true
	);

	//_unit = new Unit(4, 11, true, 0);
	_unit = new Unit(UnitType::WIZARD, 0);
	_units.push_back(_unit);
	_battleField->spawnUnit(
		_unit, 
		LEFT_LAYER_THICKNESS_DECORATING_CELLS, 
		(battleFieldHeight - TOP_LAYER_THICKNESS_DECORATING_CELLS - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS) / 2 + TOP_LAYER_THICKNESS_DECORATING_CELLS,
		true
	);

	//_unit = new Unit(15, 19, true, 1);
	_unit = new Unit(UnitType::WIZARD, 1);
	_units.push_back(_unit);
	_battleField->spawnUnit(
		_unit, 
		battleFieldWidth - RIGHT_LAYER_THICKNESS_DECORATING_CELLS - 1, 
		(battleFieldHeight - TOP_LAYER_THICKNESS_DECORATING_CELLS - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS) / 2 + TOP_LAYER_THICKNESS_DECORATING_CELLS,
		false
	);

	sortUnitsByBacklightsDrawOrder();

	//if (_battleField[])
	focusVisionOnCell(
		LEFT_LAYER_THICKNESS_DECORATING_CELLS,
		(battleFieldHeight - TOP_LAYER_THICKNESS_DECORATING_CELLS - BOTTOM_LAYER_THICKNESS_DECORATING_CELLS) / 2 + TOP_LAYER_THICKNESS_DECORATING_CELLS
	);
}



//---------------------------------------------------------------------------------------------
//functions

//void loadBiomes;

/*void getTextureBorderMask() {
	//�������� ����������� 6 ���
	//������������ ����� ��� ����� ������ ��������� ��� ������� ��� �������� �� ����� ����� "�����" � ����� �������� �� ������ �������� ������� ��������
	//��� ����� ������������ ��� ��������� ������������ ������, �� ������ ���� ��������� � �������� (���� ���) � ��������� �������� ��� ��������� �������� ������ (��� ����������� ������?)
	bool** const _textureControlField = getTextureControlField(_textureName, TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1, !_isLookRight, false);

	//�������������� ����� ������� ��������
	int _borderSize = 5;
	bool** _textureBorderMask = new bool*[_textureWidth + _borderSize * 2];
	for (int x = 0; x < _textureWidth + _borderSize * 2; x++) {
		_textureBorderMask[x] = new bool[_textureHeight + _borderSize * 2];
		for (int y = 0; y < _textureHeight + _borderSize * 2; y++) {
			_textureBorderMask[x][y] = false;
		}
	}

	//������ ����� ������� ��������
	for (int x = _borderSize; x < _textureWidth + _borderSize; x++) {
		for (int y = _borderSize; y < _textureHeight + _borderSize; y++) {
			if (_textureControlField[x - _borderSize][y - _borderSize]) { // == true
				//"������" � ����� ����
				for (int _x = x - _borderSize, ySize = 0; _x <= x + _borderSize; _x++) {
					for (int _y = y - ySize; _y <= y + ySize; _y++) {
						if (!_textureControlField[_x - _borderSize][_y - _borderSize]) { // == false
							_textureBorderMask[_x][_y] == true;
						}
					}

					if (ySize < _borderSize) ySize++;
					else ySize--;
				}
			}
		}
	}

	return _textureBorderMask;

	////������ ����� ������� //TODO ���
	//for (int x = 0; x < _textureWidth + _borderSize * 2; x++) {
	//	delete[] _textureBorderMask[x];
	//}
	//delete[] _textureBorderMask;
}*/