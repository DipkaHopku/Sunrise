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

enum class UnitType {
	WIZARD
};

class Unit;

class BattleField;

//---------------------------------------------------------------------------------------------
//class Battle declaration

class Battle {
private:
	//bool _isScrollingEnabled = false;

	BattleField* _battleField = nullptr;
	vector<Unit*> _units;

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

	/*Unit* const getUnitByIndex(int index) const {
		if (index < _units.size()) {
			return _units[index];
		}
		else {
			return nullptr;
		}
	}*/

	void sortUnitsByBacklightsDrawOrder();

	void drawUnitBacklights();

	void spawnUnit();
};