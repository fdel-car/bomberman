#pragma once

#include "Camera.hpp"

class MainMenuCam : public Camera {
	public:
		MainMenuCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
		virtual ~MainMenuCam(void);

		void drawGUI(GUI * graphicUI);

	private:
		MainMenuCam(void);
		MainMenuCam(MainMenuCam const &src);
		MainMenuCam &operator=(MainMenuCam const &rhs);

};
