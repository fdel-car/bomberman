#pragma once

#include "bomberman/GUITools.hpp"
#include "bomberman/Save.hpp"
#include "engine/Camera.hpp"

class MainMenuCam : public Camera, public GUITools {
   public:
	MainMenuCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
				std::vector<std::string> levelsName, Save &save);
	virtual ~MainMenuCam(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);

   private:
	MainMenuCam(void);
	MainMenuCam(MainMenuCam const &src);
	MainMenuCam &operator=(MainMenuCam const &rhs);

	bool _btnHover(GUI *graphicUI, int rectWidth, int rectHeight, int xRectPos,
				   int yRectPos, int fontSize, std::string fontName,
				   int *extraSizePlay, int maxSize, bool *isPlayButtonHover,
				   std::string btnName, std::string btnImageHover = "",
				   std::string btnImage = "");
	void _settings(GUI *graphicUI);
	void _movingTitle(GUI *graphicUI);

	void _updateVarsFromSave(void);
	void _updateSaveFromVars(void);

	int _lvlIndex;
	bool _changeSettings;
	bool _slowTitle;
	std::vector<std::string> _levelsName;

	// Vars for settings
	Save &_save;
	char _upChoice[1];
	int len1 = 1;
	char _leftChoice[1];
	int len2 = 1;
	char _downChoice[1];
	int len3 = 1;
	char _rightChoice[1];
	int len4 = 1;
	int _isFullScreen;
	int _resolutionsIdx;
	int _musicVolume;
	int _soundsVolume;
};
