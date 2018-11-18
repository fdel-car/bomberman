#include "scenes/MainMenuCam.hpp"
#include "GameEngine.hpp"
#include "Player.hpp"

extern std::string _assetsDir;

MainMenuCam::MainMenuCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles)
	: Camera(pos, eulerAngles) {
	vNeededImage.push_back(std::tuple<std::string, std::string>(
	(_assetsDir + "GUI/icons/chevronDroit.png"), "chevronDroit"));
	vNeededImage.push_back(std::tuple<std::string, std::string>(
	(_assetsDir + "GUI/icons/chevronGauche.png"), "chevronGauche"));
	vNeededImage.push_back(std::tuple<std::string, std::string>(
	(_assetsDir + "GUI/icons/settings.png"), "settings"));
}

MainMenuCam::~MainMenuCam(void) {}

void MainMenuCam::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(vNeededImage);
	graphicUI->uiSetDefaultFont("18_BOMBERMA");
	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	defaultStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 0);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;

	_lvlChoice.push_back("Tutorial");
	_lvlChoice.push_back("Level One");
	_lvlChoice.push_back("Level Two");
	_lvlChoice.push_back("Level Three");
	_myChoice = 0;
	_changeSettings = false;
}

void MainMenuCam::drawGUI(GUI * graphicUI) {

	if (!_changeSettings) {
		static int extraSizeTitle = 40;
		static bool titleIsGrowing = true;
		if (extraSizeTitle < 48 && titleIsGrowing)
			extraSizeTitle++;
		else if (extraSizeTitle > 40 && !titleIsGrowing)
			extraSizeTitle--;
		else
			titleIsGrowing = !titleIsGrowing;
		if (graphicUI->uiStartBlock("Title", "",
									nk_rect(0, (WINDOW_H / 5) * 1, WINDOW_W, 50),
									NK_WINDOW_NO_SCROLLBAR)) {
			graphicUI->uiHeader("Super Bomberman", NK_TEXT_CENTERED, 48, std::to_string(extraSizeTitle) + "_BOMBERMA");
		}
		graphicUI->uiEndBlock();

		if (graphicUI->uiStartBlock(
				"Levels", "",
				nk_rect((WINDOW_W / 5) * 2, (WINDOW_H / 5) * 2, (WINDOW_W / 5), 50),
				NK_WINDOW_NO_SCROLLBAR)) {
			if (graphicUI->uiHorizontalSelection((WINDOW_W / 5), "",
				_lvlChoice[_myChoice], &_myChoice, _lvlChoice.size() - 1)) {
				std::cout << _myChoice << std::endl;
			}
		}
		graphicUI->uiEndBlock();

		_btnPlay(graphicUI);
		_btnSettings(graphicUI);
		_btnCredits(graphicUI);
		_btnExit(graphicUI);
	}
	else
		_settings(graphicUI);
}

void MainMenuCam::_settings(GUI *graphicUI) {
	activeStyle[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
	graphicUI->setStyle(activeStyle);
	if (graphicUI->uiStartBlock(
		"SettingMenu", "Settings",
		nk_rect((WINDOW_W / 4),
		(WINDOW_H / 3) / 2, WINDOW_W / 2, (WINDOW_H / 3) * 2),
		NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		graphicUI->uiHeader("Options", NK_TEXT_CENTERED, 30, "24_BOMBERMA");
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test1",
			_lvlChoice[_myChoice], &_myChoice, _lvlChoice.size() - 1)) { }
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test2",
			_lvlChoice[_myChoice], &_myChoice, _lvlChoice.size() - 1)) { }
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test3",
			_lvlChoice[_myChoice], &_myChoice, _lvlChoice.size() - 1)) { }
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test4",
			_lvlChoice[_myChoice], &_myChoice, _lvlChoice.size() - 1)) { }
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test5",
			_lvlChoice[_myChoice], &_myChoice, _lvlChoice.size() - 1)) { }

		graphicUI->uiHeader("Controls", NK_TEXT_CENTERED, 30, "24_BOMBERMA");
		static int len1 = 0;
		static char test1[1];
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button1", NK_EDIT_FIELD, test1, &len1, 2, nk_filter_default);

		static int len2 = 0;
		static char test2[1];
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button2", NK_EDIT_FIELD, test2, &len2, 2, nk_filter_default);

		static int len3 = 1;
		static char test3[1];
		test3[0] = 'p';
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button3", NK_EDIT_FIELD, test3, &len3, 2, nk_filter_default);

		static int len4 = 0;
		static char test4[1];
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button4", NK_EDIT_FIELD, test4, &len4, 2, nk_filter_default);

		static int len5 = 0;
		static char test5[1];
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button5", NK_EDIT_FIELD, test5, &len5, 2, nk_filter_default);

		if (graphicUI->uiButton((WINDOW_W / 2) - 11, 50, 0, "Back"))
			_changeSettings = false;
	}
	graphicUI->uiEndBlock();
	activeStyle = defaultStyle;
	graphicUI->setStyle(activeStyle);
}

void MainMenuCam::_btnPlay(GUI * graphicUI) {
	static int extraSizePlay = 0;
	static bool isPlayButtonHover = false;
	int rectWidth = (WINDOW_W / 5);
	int rectHeight = 60;
	int xRectPos = (WINDOW_W / 5) * 2;
	int yRectPos = (WINDOW_H / 5) * 2.7;
	int fontSize = 20;
	std::string fontName = "_BOMBERMA";
	if (extraSizePlay < 10 && isPlayButtonHover)
		extraSizePlay++;
	else if (extraSizePlay > 0 && !isPlayButtonHover)
		extraSizePlay--;
	rectWidth += extraSizePlay;
	rectHeight += extraSizePlay;
	xRectPos -= extraSizePlay / 2;
	yRectPos -= extraSizePlay / 2;
	fontName = std::to_string(fontSize + extraSizePlay) + fontName;
	activeStyle[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(51, 55, 67, 0);
	activeStyle[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 0);
	activeStyle[NK_COLOR_BUTTON] = nk_rgba(51, 55, 67, 0);
	activeStyle[NK_COLOR_BUTTON_HOVER] = nk_rgba(51, 55, 67, 0);
	graphicUI->setStyle(activeStyle);
	if (graphicUI->uiStartBlock(
			"Play", "",
			nk_rect(xRectPos, yRectPos, rectWidth, rectHeight),
			NK_WINDOW_NO_SCROLLBAR)) {
		if (graphicUI->uiHover()) {
			isPlayButtonHover = true;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, "Play", "", fontName)) {
				std::cout << "3" << std::endl;
			}
		} else {
			isPlayButtonHover = false;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, "Play", "", fontName)) { }
		}
	}
	graphicUI->uiEndBlock();
	activeStyle = defaultStyle;
	graphicUI->setStyle(activeStyle);
}

void MainMenuCam::_btnSettings(GUI * graphicUI) {
	static int extraSizeSetting = 0;
	static bool isSettingButtonHover = false;
	int rectWidth = (WINDOW_W / 5);
	int rectHeight = 60;
	int xRectPos = (WINDOW_W / 5) - ((WINDOW_W / 5) / 2);
	int yRectPos = (WINDOW_H / 5) * 4;
	int fontSize = 14;
	std::string fontName = "_BOMBERMA";
	if (extraSizeSetting < 10 && isSettingButtonHover)
		extraSizeSetting++;
	else if (extraSizeSetting > 0 && !isSettingButtonHover)
		extraSizeSetting--;
	rectWidth += extraSizeSetting;
	rectHeight += extraSizeSetting;
	xRectPos -= extraSizeSetting / 2;
	yRectPos -= extraSizeSetting / 2;
	fontName = std::to_string(fontSize + extraSizeSetting) + fontName;
	if (graphicUI->uiStartBlock(
			"Setting", "",
			nk_rect(xRectPos, yRectPos, rectWidth, rectHeight),
			NK_WINDOW_NO_SCROLLBAR)) {
		if (graphicUI->uiHover()) {
			isSettingButtonHover = true;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, "Settings", "settings", fontName)) {
				_changeSettings = true;
			}
		} else {
			isSettingButtonHover = false;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, "Settings", "", fontName)) { }
		}
	}
	graphicUI->uiEndBlock();
}

void MainMenuCam::_btnCredits(GUI * graphicUI) {
	static int extraSizeCredits = 0;
	static bool isCreditButtonHover = false;
	int rectWidth = (WINDOW_W / 5);
	int rectHeight = 60;
	int xRectPos = (WINDOW_W / 5) * 2;
	int yRectPos = (WINDOW_H / 5) * 4;
	int fontSize = 14;
	std::string fontName = "_BOMBERMA";
	if (extraSizeCredits < 10 && isCreditButtonHover)
		extraSizeCredits++;
	else if (extraSizeCredits > 0 && !isCreditButtonHover)
		extraSizeCredits--;
	rectWidth += extraSizeCredits;
	rectHeight += extraSizeCredits;
	xRectPos -= extraSizeCredits / 2;
	yRectPos -= extraSizeCredits / 2;
	fontName = std::to_string(fontSize + extraSizeCredits) + fontName;
	if (graphicUI->uiStartBlock(
			"Credits", "",
			nk_rect(xRectPos, yRectPos, rectWidth, rectHeight),
			NK_WINDOW_NO_SCROLLBAR)) {
		if (graphicUI->uiHover()) {
			isCreditButtonHover = true;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, "Credits", "", fontName)) {
				std::cout << "2" << std::endl;
				_newSceneIdx = 1;
			}
		}
		else {
			isCreditButtonHover = false;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, "Credits", "", fontName)) { }
		}
	}
	graphicUI->uiEndBlock();
}

void MainMenuCam::_btnExit(GUI * graphicUI) {
	static int extraSizeExit = 0;
	static bool isExitButtonHover = false;
	int rectWidth = (WINDOW_W / 5);
	int rectHeight = 60;
	int xRectPos = (WINDOW_W / 5) * 4 - ((WINDOW_W / 5) / 2);
	int yRectPos = (WINDOW_H / 5) * 4;
	int fontSize = 14;
	std::string fontName = "_BOMBERMA";
	if (extraSizeExit < 10 && isExitButtonHover)
		extraSizeExit++;
	else if (extraSizeExit > 0 && !isExitButtonHover)
		extraSizeExit--;
	rectWidth += extraSizeExit;
	rectHeight += extraSizeExit;
	xRectPos -= extraSizeExit / 2;
	yRectPos -= extraSizeExit / 2;
	fontName = std::to_string(fontSize + extraSizeExit) + fontName;
	if (graphicUI->uiStartBlock(
			"Exit", "",
			nk_rect(xRectPos, yRectPos, rectWidth, rectHeight),
			NK_WINDOW_NO_SCROLLBAR)) {
		if (graphicUI->uiHover()) {
			isExitButtonHover = true;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, "Exit", "", fontName)) {
				std::cout << "2" << std::endl;
			}
		}
		else {
			isExitButtonHover = false;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, "Exit", "", fontName)) {
				std::cout << "2" << std::endl;
			}
		}
	}
	graphicUI->uiEndBlock();
}
