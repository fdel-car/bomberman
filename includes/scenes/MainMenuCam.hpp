#pragma once

#include "Camera.hpp"

class MainMenuCam : public Camera {
	public:
		MainMenuCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
		virtual ~MainMenuCam(void);

		virtual void drawGUI(GUI * graphicUI);
		virtual void configGUI(GUI *graphicUI);

	private:
		MainMenuCam(void);
		MainMenuCam(MainMenuCam const &src);
		MainMenuCam &operator=(MainMenuCam const &rhs);

		void _btnPlay(GUI *graphicUI);
		void _btnSettings(GUI *graphicUI);
		void _btnCredits(GUI *graphicUI);
		void _btnExit(GUI *graphicUI);
		void _settings(GUI *graphicUI);

		std::vector<std::string> _lvlChoice;
		int _myChoice;
		bool _changeSettings;

};
