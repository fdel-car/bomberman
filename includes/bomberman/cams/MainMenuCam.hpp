#pragma once

// #include "engine/Camera.hpp"
#include "bomberman/cams/Tools.hpp"

class MainMenuCam : public Tools {
   public:
	MainMenuCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
				std::vector<std::string> levelsName);
	virtual ~MainMenuCam(void);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);

   private:
	MainMenuCam(void);
	MainMenuCam(MainMenuCam const &src);
	MainMenuCam &operator=(MainMenuCam const &rhs);

	void _settings(GUI *graphicUI);
	void _movingTitle(GUI *graphicUI);

	int _lvlIndex;
	bool _changeSettings;
	bool _slowTitle;
	std::vector<std::string> _levelsName;
};
