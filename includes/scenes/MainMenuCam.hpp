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

		bool _btnHover(GUI * graphicUI, int rectWidth, int rectHeight,
			int xRectPos, int yRectPos, int fontSize, std::string fontName, int *extraSizePlay,
			int maxSize, bool *isPlayButtonHover, std::string btnName,
			std::string btnImageHover = "", std::string btnImage = "");
		void _settings(GUI *graphicUI);
		void _movingTitle(GUI *graphicUI);

		std::vector<std::string> _lvlChoice;
		int _myChoice;
		bool _changeSettings;
		bool _slowTitle;
};
