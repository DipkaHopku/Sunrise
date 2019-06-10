#pragma once

#include <map>
#include <iostream>
#include <ctime>
#include <utility>
#include <cmath>
#include <algorithm>

#include "userInterface.h"
#include "textures.h"

using std::swap;
using std::max;

enum class BiomeType {
	GRASS,
	TREE,
	STONE,
	BUSH,
	PINE,
	SMALL_STONE,
	NONE
};

enum class AdjacentCellRelativePos {
	LEFT_BOTTOM,
	LEFT_TOP,
	TOP,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	BOTTOM
};

enum class UnitType {
	WIZARD,
	WARRIOR
};

class Unit;

class Cell;

class BattleField;

//---------------------------------------------------------------------------------------------
//class Battle declaration

class Battle {
private:
	//bool _isScrollingEnabled = false;

	BattleField* _battleField = nullptr;
	vector<Unit*> _units;

	Unit* _chosenUnit = nullptr;
	bool _actionMode = false;
	Cell* _cellUnderMouse = nullptr;
	Unit* _unitUnderMouse = nullptr;
	//Cell* _movementMarkCell = nullptr;

	int _currentTurn = 0;

	int _healthBarWidth;
	int _healthBarHeight;

	Battle();
	~Battle();
	Battle(const Battle&) = delete;
	Battle& operator=(const Battle&) = delete;

public:
	static Battle& Instance();

	void begin();

	void end();

	void draw();

	void endTurn();

	void getBattleFieldProperties(int*, int*) const;

	//void scrollBattleField(int xPixels, int yPixels);
	void switchBattleFieldScrolling(bool scrollingState);

	void updateBattleFieldAfterWindowResize();

	void focusVisionOnCell(Cell*);

	void focusVisionOnCell(int xCellPos, int yCellPos);

	/*Unit* const getUnitByIndex(int index) const {
		if (index < _units.size()) {
			return _units[index];
		}
		else {
			return nullptr;
		}
	}*/

	void sortUnitsByBacklightsDrawOrder();

	void drawMovementMarkBacklight();
	
	void drawUnitBacklights();

	void setChosenUnit(Unit*);

	Unit* getChosenUnit();

	void switchActionMode(bool);

	//void setChosenUnit(Unit*);

	bool getActionMode();

	//void setMovementMarkCell(Cell*);
	void setCellUnderMouse(Cell*);

	void setUnitUnderMouse(Unit*);

	//Cell* getMovementMarkCell();
	Cell* getCellUnderMouse();

	Unit* getUnitUnderMouse();

	bool getAdjacentCellPos(int*, int*, AdjacentCellRelativePos);

	/*
	arguments:
	#1 - xFirstCellPos
	#2 - yFirstCellPos
	#3 - xSecondCellPos
	#4 - ySecondCellPos
	*/
	int getDistanceBetweenCells(int, int, int, int);

	void spawnUnit(); //TODO spawnUnits?

	void getHealthBarSize(int*, int*);

	void killUnit(Unit*);

	void checkEndBattleCondition();
};