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
		drawTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, nullptr, 
			TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		//drawScaledTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		//drawScaledTexture(0, 0, TextureName::WTF_CAT, TextureScalingByHeightRatioType::MULTIPLYNG_FACTOR, 1);
		break;

	case ApplicationState::BATTLE :
		Battle::Instance().draw();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//_drawTextureWithDepth(0, 0, 10, TextureName::MAIN_MENU_BACKGROUND);
		/*_drawTextureWithDepth(20, 20, 3, TextureName::GRASS);
		_drawTextureWithDepth(80, 40, 3, TextureName::GRASS);
		_drawTextureWithDepth(40, 40, 2, TextureName::BUSH);
		_drawTextureWithDepth(50, 50, 1, TextureName::STONE);
		_drawTextureWithDepth(60, 60, 0, TextureName::WTF_CAT);*/

		/*drawTexture(10, 10, TextureName::STONE);
		drawTexture(200, 100, TextureName::STONE);
		drawTexture(150, 400, TextureName::STONE);
		drawTexture(0, 0, TextureName::STONE);
		drawTexture(0, 350, TextureName::WTF_CAT);*/

		/*for (int i = 400; i < 10400; i++) { //14-20 мс медиана - 15
			if (i % 2 == 0) drawTexture(i, i, TextureName::TEST2);
			else drawTexture(i, i, TextureName::TEST4);
		}*/

		break;
	
	case ApplicationState::WIN:
		drawTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, nullptr,
			TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		break;

	case ApplicationState::LOSE:
		drawTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, nullptr,
			TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		break;

	case ApplicationState::LOAD_MENU :
		//TODO
		break;

	case ApplicationState::OPTIONS:
		drawTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, nullptr,
			TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		//drawScaledTexture(0, 0, TextureName::MAIN_MENU_BACKGROUND, TextureScalingByHeightRatioType::PIXELS_NUMBER, _windowHeight);
		break;
	}

	UserInterface::Instance().drawUserInterface();

	//контент под курсор может меняться, хотя курсор может и не двигаться, так что вручную вызывает событие определения того что находится под курсором
	int	_xCursorPos, _yCursorPos;
	UserInterface::Instance().getCursorPos(&_xCursorPos, &_yCursorPos);
	UserInterface::Instance().setCursorPos(_xCursorPos, _yCursorPos);

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