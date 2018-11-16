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

};
