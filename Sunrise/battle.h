#pragma once

#include <map>
#include <iostream>
#include <ctime>

#include "userInterface.h"
#include "textures.h"

enum class BiomeType {
	GRASS,
	TREE,
	STONE,
	BUSH,
	PINE,
	SMALL_STONE,
	NONE
};

class BattleField;

//---------------------------------------------------------------------------------------------
//class Battle declaration

class Battle {
private:
	//bool _isScrollingEnabled = false;

	BattleField* _battleField = nullptr;

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
};