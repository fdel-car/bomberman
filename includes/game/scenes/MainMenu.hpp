#pragma once

#include "game/Save.hpp"
#include "game/scenes/SceneTools.hpp"

class MainMenu : public SceneTools {
   public:
	MainMenu(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
			 std::vector<std::string> levelsName, Save &save);
	virtual ~MainMenu(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);

   private:
	MainMenu(void);
	MainMenu(MainMenu const &src);
	MainMenu &operator=(MainMenu const &rhs);

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
