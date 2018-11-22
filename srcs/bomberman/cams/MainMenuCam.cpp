#include "bomberman/cams/MainMenuCam.hpp"
#include "engine/GameEngine.hpp"

extern std::string _assetsDir;

MainMenuCam::MainMenuCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
						 std::vector<std::string> levelsName)
	: Camera(pos, eulerAngles), GUITools(), _levelsName(levelsName) {
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/icons/rightAngleBracket.png"), "rightAngleBracket"));
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/icons/leftAngleBracket.png"), "leftAngleBracket"));
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/icons/settings.png"), "settings"));
}

MainMenuCam::~MainMenuCam(void) {}

void MainMenuCam::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImage(_neededImages);
	graphicUI->uiSetDefaultFont("18_BOMBERMAN");
	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	defaultStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 0);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;

	_lvlIndex = 0;
	_changeSettings = false;
	_slowTitle = false;
}

void MainMenuCam::drawGUI(GUI *graphicUI) {
	if (!_changeSettings) {
		_movingTitle(graphicUI);

		if (graphicUI->uiStartBlock(
				"Levels", "",
				nk_rect((WINDOW_W / 5) * 2, (WINDOW_H / 5) * 2, (WINDOW_W / 5),
						50),
				NK_WINDOW_NO_SCROLLBAR)) {
			if (graphicUI->uiHorizontalSelection(
					(WINDOW_W / 5), "", _levelsName[_lvlIndex], &_lvlIndex,
					_levelsName.size() - 1)) {
				std::cout << _lvlIndex << std::endl;
			}
		}
		graphicUI->uiEndBlock();

		activeStyle[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(51, 55, 67, 0);
		activeStyle[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 0);
		activeStyle[NK_COLOR_BUTTON] = nk_rgba(51, 55, 67, 0);
		activeStyle[NK_COLOR_BUTTON_HOVER] = nk_rgba(51, 55, 67, 0);
		graphicUI->setStyle(activeStyle);
		static int extraSizePlay = 0;
		static bool isPlayButtonHover = false;
		if (_btnHover(graphicUI, (WINDOW_W / 5), 60, (WINDOW_W / 5) * 2,
					  (WINDOW_H / 5) * 2.7, 20, "_BOMBERMAN", &extraSizePlay,
					  10, &isPlayButtonHover, "Play"))
			_newSceneName = _levelsName[_lvlIndex];

		activeStyle = defaultStyle;
		graphicUI->setStyle(activeStyle);

		static int extraSizeSetting = 0;
		static bool isSettingButtonHover = false;
		if (_btnHover(graphicUI, (WINDOW_W / 5), 60,
					  (WINDOW_W / 5) - ((WINDOW_W / 5) / 2), (WINDOW_H / 5) * 4,
					  14, "_BOMBERMAN", &extraSizeSetting, 10,
					  &isSettingButtonHover, "Settings", "settings"))
			_changeSettings = true;

		static int extraSizeCredits = 0;
		static bool isCreditButtonHover = false;
		if (_btnHover(graphicUI, (WINDOW_W / 5), 60, (WINDOW_W / 5) * 2,
					  (WINDOW_H / 5) * 4, 14, "_BOMBERMAN", &extraSizeCredits,
					  10, &isCreditButtonHover, "Credits"))
			std::cout << "Hey hey, nothing happened. bad luck." << std::endl;

		static int extraSizeExit = 0;
		static bool isExitButtonHover = false;
		if (_btnHover(graphicUI, (WINDOW_W / 5), 60,
					  (WINDOW_W / 5) * 4 - ((WINDOW_W / 5) / 2),
					  (WINDOW_H / 5) * 4, 14, "_BOMBERMAN", &extraSizeExit, 10,
					  &isExitButtonHover, "Exit"))
			std::cout << "Exits" << std::endl;
	} else
		_settings(graphicUI);
}

void MainMenuCam::_settings(GUI *graphicUI) {
	activeStyle[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
	graphicUI->setStyle(activeStyle);
	if (graphicUI->uiStartBlock("SettingMenu", "Settings",
								nk_rect((WINDOW_W / 4), (WINDOW_H / 3) / 2,
										WINDOW_W / 2, (WINDOW_H / 3) * 2),
								NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		graphicUI->uiHeader("Options", NK_TEXT_CENTERED, 30, "24_BOMBERMAN");
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test1",
											 _levelsName[_lvlIndex], &_lvlIndex,
											 _levelsName.size() - 1)) {
		}
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test2",
											 _levelsName[_lvlIndex], &_lvlIndex,
											 _levelsName.size() - 1)) {
		}
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test3",
											 _levelsName[_lvlIndex], &_lvlIndex,
											 _levelsName.size() - 1)) {
		}
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test4",
											 _levelsName[_lvlIndex], &_lvlIndex,
											 _levelsName.size() - 1)) {
		}
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test5",
											 _levelsName[_lvlIndex], &_lvlIndex,
											 _levelsName.size() - 1)) {
		}

		graphicUI->uiHeader("Controls", NK_TEXT_CENTERED, 30, "24_BOMBERMAN");
		static int len1 = 0;
		static char test1[1];
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button1",
										  NK_EDIT_FIELD, test1, &len1, 2,
										  nk_filter_default);

		static int len2 = 0;
		static char test2[1];
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button2",
										  NK_EDIT_FIELD, test2, &len2, 2,
										  nk_filter_default);

		static int len3 = 1;
		static char test3[1];
		test3[0] = 'p';
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button3",
										  NK_EDIT_FIELD, test3, &len3, 2,
										  nk_filter_default);

		static int len4 = 0;
		static char test4[1];
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button4",
										  NK_EDIT_FIELD, test4, &len4, 2,
										  nk_filter_default);

		static int len5 = 0;
		static char test5[1];
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "button5",
										  NK_EDIT_FIELD, test5, &len5, 2,
										  nk_filter_default);

		int btnWidth = (WINDOW_W / 4) - 18;
		graphicUI->uiRowMultipleElem(true, 60, 3);
		graphicUI->uiAddElemInRow(btnWidth);
		if (graphicUI->uiButton(btnWidth, 50, 0, "Default", "", "", false))
			_changeSettings = false;
		graphicUI->uiAddElemInRow(btnWidth);
		if (graphicUI->uiButton(btnWidth, 50, 0, "Save", "", "", false))
			_changeSettings = false;
		graphicUI->uiRowMultipleElem(false);
	}
	graphicUI->uiEndBlock();
	activeStyle = defaultStyle;
	graphicUI->setStyle(activeStyle);
}

bool MainMenuCam::_btnHover(GUI *graphicUI, int rectWidth, int rectHeight,
							int xRectPos, int yRectPos, int fontSize,
							std::string fontName, int *extraSize, int maxSize,
							bool *isButtonHover, std::string btnName,
							std::string btnImageHover, std::string btnImage) {
	bool ret = false;
	if (*extraSize < maxSize && *isButtonHover)
		*extraSize += 1;
	else if (*extraSize > 0 && !*isButtonHover)
		*extraSize -= 1;
	rectWidth += *extraSize;
	rectHeight += *extraSize;
	xRectPos -= *extraSize / 2;
	yRectPos -= *extraSize / 2;
	fontName = std::to_string(fontSize + *extraSize) + fontName;
	if (graphicUI->uiStartBlock(
			btnName.c_str(), "",
			nk_rect(xRectPos, yRectPos, rectWidth, rectHeight),
			NK_WINDOW_NO_SCROLLBAR)) {
		if (graphicUI->uiHover()) {
			*isButtonHover = true;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, btnName.c_str(),
									btnImageHover.c_str(), fontName)) {
				ret = true;
			}
		} else {
			*isButtonHover = false;
			if (graphicUI->uiButton(rectWidth, rectHeight, 0, btnName.c_str(),
									btnImage.c_str(), fontName)) {
				ret = true;
			}
		}
	}
	graphicUI->uiEndBlock();
	return ret;
}

void MainMenuCam::_movingTitle(GUI *graphicUI) {
	static int extraSizeTitle = 40;
	static bool titleIsGrowing = true;
	if (_slowTitle) {
		_slowTitle = !_slowTitle;
		if (extraSizeTitle < 48 && titleIsGrowing)
			extraSizeTitle++;
		else if (extraSizeTitle > 38 && !titleIsGrowing)
			extraSizeTitle--;
		else
			titleIsGrowing = !titleIsGrowing;
	} else
		_slowTitle = !_slowTitle;
	if (graphicUI->uiStartBlock("Title", "",
								nk_rect(0, (WINDOW_H / 5) * 1, WINDOW_W, 50),
								NK_WINDOW_NO_SCROLLBAR)) {
		graphicUI->uiHeader("Super Bomberman", NK_TEXT_CENTERED, 48,
							std::to_string(extraSizeTitle) + "_BOMBERMAN");
	}
	graphicUI->uiEndBlock();
}
