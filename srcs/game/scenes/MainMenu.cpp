#include "game/scenes/MainMenu.hpp"
#include "engine/GameEngine.hpp"

extern std::string _assetsDir;

MainMenu::MainMenu(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
				   std::vector<std::string> levelsName, Bomberman *bomberman)
	: SceneTools(0, 0, pos, eulerAngles, bomberman,
				 bomberman->getStartLevelName()),
	  _levelsName(levelsName) {
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/icons/rightAngleBracket.png"), "rightAngleBracket"));
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/icons/leftAngleBracket.png"), "leftAngleBracket"));
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/icons/settings.png"), "settings"));

	_updateVarsFromSave();
	_startMusic = _assetsDir + "Audio/Musics/MainMenu.wav";
}

MainMenu::~MainMenu(void) {}

void MainMenu::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImages(_neededImages);
	graphicUI->uiSetDefaultFont("22_BOMBERMAN");
	graphicUI->getDefaultStyle(THEME_RED, &defaultStyle);
	defaultStyle[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 0);
	defaultStyle[NK_COLOR_TEXT] = nk_rgba(215, 215, 215, 255);
	graphicUI->setStyle(defaultStyle);
	activeStyle = defaultStyle;

	_lvlIndex = 0;
	_changeSettings = false;
	_slowTitle = false;
}

void MainMenu::drawGUI(GUI *graphicUI) {
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
				// std::cout << _lvlIndex << std::endl;
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
					  (WINDOW_H / 5) * 2.7, 30, "_BOMBERMAN", &extraSizePlay,
					  10, &isPlayButtonHover, "Play"))
			_newSceneName = _levelsName[_lvlIndex];

		activeStyle = defaultStyle;
		graphicUI->setStyle(activeStyle);

		static int extraSizeSetting = 0;
		static bool isSettingButtonHover = false;
		if (_btnHover(graphicUI, (WINDOW_W / 5), 60,
					  (WINDOW_W / 5) - ((WINDOW_W / 5) / 2), (WINDOW_H / 5) * 4,
					  20, "_BOMBERMAN", &extraSizeSetting, 10,
					  &isSettingButtonHover, "Settings", "settings"))
			_changeSettings = true;

		static int extraSizeCredits = 0;
		static bool isCreditButtonHover = false;
		if (_btnHover(graphicUI, (WINDOW_W / 5), 60, (WINDOW_W / 5) * 2,
					  (WINDOW_H / 5) * 4, 20, "_BOMBERMAN", &extraSizeCredits,
					  10, &isCreditButtonHover, "Credits"))
			std::cout << "Hey hey, nothing happened. bad luck." << std::endl;

		static int extraSizeExit = 0;
		static bool isExitButtonHover = false;
		if (_btnHover(graphicUI, (WINDOW_W / 5), 60,
					  (WINDOW_W / 5) * 4 - ((WINDOW_W / 5) / 2),
					  (WINDOW_H / 5) * 4, 20, "_BOMBERMAN", &extraSizeExit, 10,
					  &isExitButtonHover, "Exit"))
			_isRunning = false;
	} else
		_settings(graphicUI);
}

void MainMenu::_settings(GUI *graphicUI) {
	activeStyle[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
	graphicUI->setStyle(activeStyle);
	if (graphicUI->uiStartBlock("SettingMenu", "Settings",
								nk_rect((WINDOW_W / 4), (WINDOW_H / 3) / 2,
										WINDOW_W / 2, (WINDOW_H / 3) * 2),
								NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		graphicUI->uiHeader("Options", NK_TEXT_CENTERED, 30, "28_BOMBERMAN");

		if (graphicUI->uiHorizontalSelection(
				WINDOW_W / 2, "Window Resolution",
				std::get<0>(Save::RESOLUTIONS[_resolutionsIdx]),
				&_resolutionsIdx, Save::RESOLUTIONS.size() - 1)) {
		}
		if (graphicUI->uiHorizontalSelection(
				WINDOW_W / 2, "Full Screen",
				std::get<0>(Save::FULL_SCREEN[_isFullScreen]), &_isFullScreen,
				Save::FULL_SCREEN.size() - 1)) {
		}
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Music Volume",
											 _levelsName[_lvlIndex], &_lvlIndex,
											 _levelsName.size() - 1)) {
		}
		if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Sounds Volume",
											 _levelsName[_lvlIndex], &_lvlIndex,
											 _levelsName.size() - 1)) {
		}
		// if (graphicUI->uiHorizontalSelection(WINDOW_W / 2, "Test5",
		// 									 _levelsName[_lvlIndex], &_lvlIndex,
		// 									 _levelsName.size() - 1)) {
		// }

		graphicUI->uiHeader("Keyboard Controls", NK_TEXT_CENTERED, 30,
							"28_BOMBERMAN");
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "Move Up",
										  NK_EDIT_FIELD, _upChoice, &len1, 2,
										  nk_filter_default);
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "Move Left",
										  NK_EDIT_FIELD, _leftChoice, &len2, 2,
										  nk_filter_default);
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "Move Down",
										  NK_EDIT_FIELD, _downChoice, &len3, 2,
										  nk_filter_default);
		graphicUI->uiHorizontalEditString(WINDOW_W / 2, "Move Right",
										  NK_EDIT_FIELD, _rightChoice, &len4, 2,
										  nk_filter_default);

		int btnWidth = (WINDOW_W / 6) - 11;
		graphicUI->uiRowMultipleElem(true, 60, 3);
		graphicUI->uiAddElemInRow(btnWidth);
		if (graphicUI->uiButton(btnWidth, 50, 0, "Back", "", "", false))
			_changeSettings = false;
		graphicUI->uiAddElemInRow(btnWidth);
		if (graphicUI->uiButton(btnWidth, 50, 0, "Default", "", "", false)) {
			_save.resetSettings();
			_updateVarsFromSave();
			_save.doSave();
			_changeSettings = false;
		}
		graphicUI->uiAddElemInRow(btnWidth);
		if (graphicUI->uiButton(btnWidth, 50, 0, "Save", "", "", false)) {
			_updateSaveFromVars();
			_save.doSave();
			_updateVarsFromSave();  // Avoid empty field bug
			_changeSettings = false;
		}
		graphicUI->uiRowMultipleElem(false);
	}
	graphicUI->uiEndBlock();
	activeStyle = defaultStyle;
	graphicUI->setStyle(activeStyle);
}

void MainMenu::_movingTitle(GUI *graphicUI) {
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

void MainMenu::_updateVarsFromSave(void) {
	len1 = 1;
	len2 = 1;
	len3 = 1;
	len4 = 1;
	_upChoice[0] = static_cast<char>(_save.upKey);
	_leftChoice[0] = static_cast<char>(_save.leftKey);
	_downChoice[0] = static_cast<char>(_save.downKey);
	_rightChoice[0] = static_cast<char>(_save.rightKey);
	_isFullScreen = _save.isFullScreen;
	_resolutionsIdx = _save.resolutionsIdx;
	_musicVolume = _save.musicVolume;
	_soundsVolume = _save.soundsVolume;
}

void MainMenu::_updateSaveFromVars(void) {
	_save.upKey = toupper(_upChoice[0]);
	_save.leftKey = toupper(_leftChoice[0]);
	_save.downKey = toupper(_downChoice[0]);
	_save.rightKey = toupper(_rightChoice[0]);
	_save.isFullScreen = _isFullScreen;
	_save.resolutionsIdx = _resolutionsIdx;
	_save.musicVolume = _musicVolume;
	_save.soundsVolume = _soundsVolume;
}
