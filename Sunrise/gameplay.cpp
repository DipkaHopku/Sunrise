#include "gameplay.h"

static ApplicationState applicationState = ApplicationState::APPLICATION_LAUNCH;
static ApplicationState applicationStateNext = ApplicationState::MAIN_MENU;

//---------------------------------------------------------------------------------------------
//functions

void gameplayProcessing() {
	UserInterface::Instance().clearControlField();

	int _windowWidth, _windowHeight;
	UserInterface::Instance().getWindowSize(&_windowWidth, &_windowHeight);

	switch (applicationState) {
	case ApplicationState::APPLICATION_LAUNCH:
		//UserInterface::Instance().setUIItemsContainer(createMainMenu());
		createMainMenu();

		applicationState = ApplicationState::MAIN_MENU;

	case ApplicationState::MAIN_MENU :
		drawScaledTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		break;

	case ApplicationState::BATTLE :
		drawTexture(10, 10, TextureName::TEST2);
		drawTexture(200, 100, TextureName::TEST2);
		drawTexture(150, 400, TextureName::TEST2);
		drawTexture(0, 0, TextureName::TEST3);
		drawTexture(0, 350, TextureName::WTF_CAT);

		/*for (int i = 400; i < 10400; i++) { //14-20 мс медиана - 15
			if (i % 2 == 0) drawTexture(i, i, TextureName::TEST2);
			else drawTexture(i, i, TextureName::TEST4);
		}*/

		break;

	case ApplicationState::LOAD_MENU :
		//TODO
		break;

	case ApplicationState::OPTIONS:
		//TODO
		break;
	}

	UserInterface::Instance().drawUserInterface();

	//debug
	//ControlField::Instance().__drawControlField();

	//Sleep(500);
	/*test_frameTime = (float)(clock() - start) / CLOCKS_PER_SEC;
	test_totalSeconds += test_frameTime;
	test_totalFrames++;*/
	//cout << "gameplayProcessing: " << (float) (clock() - start) / CLOCKS_PER_SEC << endl; //250 кадров/сек - 4 миллисекунды на кадр, 60 фпс - 16,5 мс
}