#include "game/scenes/MainMenu.hpp"
#include "engine/GameEngine.hpp"
#include "engine/GameRenderer.hpp"

extern std::string _assetsDir;

bool MainMenu::FIRST_LOAD = true;

MainMenu::MainMenu(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
				   std::vector<std::string> levelsName, Bomberman *bomberman)
	: SceneTools(0, 0, pos, eulerAngles, bomberman,
				 bomberman->getStartLevelName()),
	  _levelsName(levelsName) {
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/Icons/rightAngleBracket.png"), "rightAngleBracket"));
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/Icons/leftAngleBracket.png"), "leftAngleBracket"));
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/Icons/settings.png"), "settings"));
	_neededImages.push_back(std::tuple<std::string, std::string>(
		(_assetsDir + "GUI/Icons/MainMenuTitle.png"), "title"));

	_updateVarsFromSave();

	// Audio settings
	_startMusic = "Audio/Musics/MainMenu.wav";
	_neededSounds.insert("select");
	_neededSounds.insert("lateral_select");
	if (FIRST_LOAD) {
		_neededSounds.insert("first_load");
	}
	_neededSounds.insert("open_settings");
	_neededSounds.insert("back");
	_neededSounds.insert("reset_default");
	_neededSounds.insert("save");
}

MainMenu::~MainMenu(void) {}

void MainMenu::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImages(_neededImages);
	graphicUI->uiSetDefaultFont("22_slider");
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
	if (FIRST_LOAD) {
		FIRST_LOAD = false;
		_gameEngine->playSound("first_load");
	}

	if (!_changeSettings) {
		_movingTitle(graphicUI);

		if (graphicUI->uiStartBlock(
				"Levels", "",
				nk_rect((_gameEngine->getGameRenderer()->getWidth() / 5) * 2,
						(_gameEngine->getGameRenderer()->getHeight() / 5) * 2,
						(_gameEngine->getGameRenderer()->getWidth() / 5), 50),
				NK_WINDOW_NO_SCROLLBAR)) {
			if (graphicUI->uiHorizontalSelection(
					(_gameEngine->getGameRenderer()->getWidth() / 5), "",
					_levelsName[_lvlIndex], &_lvlIndex, _levelsName.size() - 1,
					30)) {
				_gameEngine->playSound("lateral_select");
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
		if (_btnHover(
				graphicUI, (_gameEngine->getGameRenderer()->getWidth() / 5), 60,
				(_gameEngine->getGameRenderer()->getWidth() / 5) * 2,
				(_gameEngine->getGameRenderer()->getHeight() / 5) * 2.7, 30,
				"_slider", &extraSizePlay, 10, &isPlayButtonHover, "Play")) {
			_newSceneName = _levelsName[_lvlIndex];
			_gameEngine->playSound("select");
		}

		activeStyle = defaultStyle;
		graphicUI->setStyle(activeStyle);

		static int extraSizeSetting = 0;
		static bool isSettingButtonHover = false;
		if (_btnHover(
				graphicUI, (_gameEngine->getGameRenderer()->getWidth() / 5), 60,
				(_gameEngine->getGameRenderer()->getWidth() / 5) -
					((_gameEngine->getGameRenderer()->getWidth() / 5) / 2),
				(_gameEngine->getGameRenderer()->getHeight() / 5) * 4, 20,
				"_slider", &extraSizeSetting, 10, &isSettingButtonHover,
				"Settings", "settings")) {
			_changeSettings = true;
			_gameEngine->playSound("open_settings");
		}

		static int extraSizeCredits = 0;
		static bool isCreditButtonHover = false;
		if (_btnHover(graphicUI,
					  (_gameEngine->getGameRenderer()->getWidth() / 5), 60,
					  (_gameEngine->getGameRenderer()->getWidth() / 5) * 2,
					  (_gameEngine->getGameRenderer()->getHeight() / 5) * 4, 20,
					  "_slider", &extraSizeCredits, 10, &isCreditButtonHover,
					  "Credits")) {
			// std::cout << "Hey hey, nothing happened. bad luck." << std::endl;
			_gameEngine->playSound("select");
			_newSceneName = "Credits";
		}

		static int extraSizeExit = 0;
		static bool isExitButtonHover = false;
		if (_btnHover(
				graphicUI, (_gameEngine->getGameRenderer()->getWidth() / 5), 60,
				(_gameEngine->getGameRenderer()->getWidth() / 5) * 4 -
					((_gameEngine->getGameRenderer()->getWidth() / 5) / 2),
				(_gameEngine->getGameRenderer()->getHeight() / 5) * 4, 20,
				"_slider", &extraSizeExit, 10, &isExitButtonHover, "Exit"))
			_isRunning = false;
	} else
		_settings(graphicUI);
}

void MainMenu::_settings(GUI *graphicUI) {
	activeStyle[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
	graphicUI->setStyle(activeStyle);
	if (graphicUI->uiStartBlock(
			"SettingMenu", "Settings",
			nk_rect((_gameEngine->getGameRenderer()->getWidth() / 4),
					(_gameEngine->getGameRenderer()->getHeight() / 6),
					_gameEngine->getGameRenderer()->getWidth() / 2,
					(_gameEngine->getGameRenderer()->getHeight() / 3) * 2),
			NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		int rowHeight =
			((_gameEngine->getGameRenderer()->getHeight() / 3) * 2) / 15;
		rowHeight = rowHeight < 30 ? 30 : rowHeight;
		graphicUI->uiHeader("Options", NK_TEXT_CENTERED, 30, "28_slider");

		if (graphicUI->uiHorizontalSelection(
				_gameEngine->getGameRenderer()->getWidth() / 2,
				"Window Resolution",
				std::get<0>(Save::RESOLUTIONS[_resolutionsIdx]),
				&_resolutionsIdx, Save::RESOLUTIONS.size() - 1, rowHeight)) {
			_gameEngine->playSound("lateral_select");
		}
		if (graphicUI->uiHorizontalSelection(
				_gameEngine->getGameRenderer()->getWidth() / 2, "Full Screen",
				std::get<0>(Save::FULL_SCREEN[_isFullScreen]), &_isFullScreen,
				Save::FULL_SCREEN.size() - 1, rowHeight)) {
			_gameEngine->playSound("lateral_select");
		}
		if (graphicUI->uiHorizontalSelection(
				_gameEngine->getGameRenderer()->getWidth() / 2, "Music Volume",
				std::get<0>(Save::VOLUMES[_musicVolume]), &_musicVolume,
				Save::VOLUMES.size() - 1, rowHeight)) {
			_gameEngine->updateMusicVolume(_musicVolume);
			_gameEngine->playSound("lateral_select");
		}
		if (graphicUI->uiHorizontalSelection(
				_gameEngine->getGameRenderer()->getWidth() / 2, "Sounds Volume",
				std::get<0>(Save::VOLUMES[_soundsVolume]), &_soundsVolume,
				Save::VOLUMES.size() - 1, rowHeight)) {
			_gameEngine->updateSoundsVolume(_soundsVolume);
			_gameEngine->playSound("lateral_select");
		}
		graphicUI->uiHeader("Keyboard Controls", NK_TEXT_CENTERED, 30,
							"28_slider");
		graphicUI->uiHorizontalEditString(
			_gameEngine->getGameRenderer()->getWidth() / 2, "Move Up",
			NK_EDIT_FIELD, _upChoice, &len1, 2, nk_filter_default, rowHeight);
		graphicUI->uiHorizontalEditString(
			_gameEngine->getGameRenderer()->getWidth() / 2, "Move Left",
			NK_EDIT_FIELD, _leftChoice, &len2, 2, nk_filter_default, rowHeight);
		graphicUI->uiHorizontalEditString(
			_gameEngine->getGameRenderer()->getWidth() / 2, "Move Down",
			NK_EDIT_FIELD, _downChoice, &len3, 2, nk_filter_default, rowHeight);
		graphicUI->uiHorizontalEditString(
			_gameEngine->getGameRenderer()->getWidth() / 2, "Move Right",
			NK_EDIT_FIELD, _rightChoice, &len4, 2, nk_filter_default,
			rowHeight);

		int btnWidth = (_gameEngine->getGameRenderer()->getWidth() / 6) - 12;
		graphicUI->uiRowMultipleElem(true, 60, 3);
		graphicUI->uiAddElemInRow(btnWidth);
		if (graphicUI->uiButton(btnWidth, 50, 0, "Back", "", "", false)) {
			_changeSettings = false;
			_gameEngine->playSound("back");
			_updateVarsFromSave();
			_gameEngine->updateMusicVolume(_musicVolume);
			_gameEngine->updateSoundsVolume(_soundsVolume);
		}
		graphicUI->uiAddElemInRow(btnWidth);
		if (graphicUI->uiButton(btnWidth, 50, 0, "Default", "", "", false)) {
			_save.resetSettings();
			_save.doSave();
			_updateVarsFromSave();
			_gameEngine->updateMusicVolume(_musicVolume);
			_gameEngine->updateSoundsVolume(_soundsVolume);
			_bomberman->needResolutionChange = true;
			_changeSettings = false;
			_gameEngine->playSound("reset_default");
		}
		graphicUI->uiAddElemInRow(btnWidth);
		if (graphicUI->uiButton(btnWidth, 50, 0, "Save", "", "", false)) {
			_updateSaveFromVars();
			_save.doSave();
			_updateVarsFromSave();
			_gameEngine->updateMusicVolume(_musicVolume);
			_gameEngine->updateSoundsVolume(_soundsVolume);
			_bomberman->needResolutionChange = true;
			_changeSettings = false;
			_gameEngine->playSound("save");
		}
		graphicUI->uiRowMultipleElem(false);
	}
	graphicUI->uiEndBlock();
	activeStyle = defaultStyle;
	graphicUI->setStyle(activeStyle);
}

void MainMenu::_movingTitle(GUI *graphicUI) {
	static int extraSizeTitle = 0;
	static bool titleIsGrowing = true;
	if (_slowTitle) {
		_slowTitle = !_slowTitle;
		if (extraSizeTitle < 40 && titleIsGrowing)
			extraSizeTitle += 2;
		else if (extraSizeTitle > 0 && !titleIsGrowing)
			extraSizeTitle -= 2;
		else
			titleIsGrowing = !titleIsGrowing;
	} else
		_slowTitle = !_slowTitle;
	size_t x =
		(_gameEngine->getGameRenderer()->getWidth() / 4) - (extraSizeTitle / 2);
	size_t y = (_gameEngine->getGameRenderer()->getHeight() / 15) -
			   (extraSizeTitle / 4);
	size_t width =
		(_gameEngine->getGameRenderer()->getWidth() / 2) + extraSizeTitle;
	size_t height = (_gameEngine->getGameRenderer()->getHeight() / 4) +
					(extraSizeTitle / 4);
	if (graphicUI->uiStartBlock("Title", "",
								nk_rect(x, y, width + 20, height + 10),
								NK_WINDOW_NO_SCROLLBAR)) {
		graphicUI->uiRowMultipleElem(true, height);
		graphicUI->uiAddElemInRow(width);
		graphicUI->uiSetImage("title");
		graphicUI->uiRowMultipleElem(false);
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
