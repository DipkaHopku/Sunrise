#include "gameplay.h"

//static ApplicationState applicationState = ApplicationState::APPLICATION_LAUNCH;
//static ApplicationState applicationStateNext = ApplicationState::APPLICATION_LAUNCH;

//---------------------------------------------------------------------------------------------
//functions

void gameplayProcessing() {
	UserInterface::Instance().clearControlField();

	//получаем то каким должнон было быть следующее состояние
	ApplicationState _applicationStateNext = ApplicationStatePlanningController::getApplicationStateNext();
	//следующее состояние наступило - устанавливаем его текущим
	ApplicationStateChangingController::setApplicationState(_applicationStateNext);
	ApplicationState _applicationState = ApplicationStatePlanningController::getApplicationStateNext();

	int _windowWidth, _windowHeight;
	UserInterface::Instance().getWindowSize(&_windowWidth, &_windowHeight);

	switch (_applicationState) {
	case ApplicationState::APPLICATION_LAUNCH:
		//загружаем все текстуры //перенесено из main
		loadTextures(); //>128 мс

		//UserInterface::Instance().setUIItemsContainer(createMainMenu());
		//createMainMenu();

		//applicationState = ApplicationState::MAIN_MENU;
		ApplicationStatePlanningController::setApplicationStateNext(ApplicationState::MAIN_MENU);
		//applicationStateNext = ApplicationState::MAIN_MENU;
		break;

	case ApplicationState::MAIN_MENU :
		drawScaledTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		//drawScaledTexture(0, 0, TextureName::WTF_CAT, TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1);
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
		drawScaledTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		break;
	}

	UserInterface::Instance().drawUserInterface();

	/*double cursor_x, cursor_y;
	glfwGetCursorPos(window, &cursor_x, &cursor_y);

	UserInterface::Instance().triggerActiveUIItemOnClickCallbackAtPoint(cursor_x, cursor_y);*/

	//debug
	//ControlField::Instance().__drawControlField();

	//Sleep(500);
	/*test_frameTime = (float)(clock() - start) / CLOCKS_PER_SEC;
	test_totalSeconds += test_frameTime;
	test_totalFrames++;*/
	//cout << "gameplayProcessing: " << (float) (clock() - start) / CLOCKS_PER_SEC << endl; //250 кадров/сек - 4 миллисекунды на кадр, 60 фпс - 16,5 мс
}