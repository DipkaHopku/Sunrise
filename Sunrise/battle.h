#pragma once

#include <map>
#include <iostream>
#include <ctime>
#include <utility>

#include "userInterface.h"
#include "textures.h"

using std::swap;

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
	WIZARD
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
	bool _movementMode = false;
	Cell* _cellUnderMouse = nullptr;
	//Cell* _movementMarkCell = nullptr;

	int _turn;

	Battle();
	~Battle();
	Battle(const Battle&) = delete;
	Battle& operator=(const Battle&) = delete;

public:
	static Battle& Instance();

	void begin();

	void end();

	void draw();

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

	void switchMovementMode(bool);

	//void setChosenUnit(Unit*);

	bool getMovementMode();

	//void setMovementMarkCell(Cell*);
	void setCellUnderMouse(Cell*);

	//Cell* getMovementMarkCell();
	Cell* getCellUnderMouse();

	bool getAdjacentCellPos(int*, int*, AdjacentCellRelativePos);

	void spawnUnit(); //TODO spawnUnits?
};