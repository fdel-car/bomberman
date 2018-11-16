#pragma once

#include "Camera.hpp"

class FactoryCam : public Camera {
	public:
		FactoryCam(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
		virtual ~FactoryCam(void);

		void drawGUI(GUI * graphicUI);

	private:
		FactoryCam(void);
		FactoryCam(FactoryCam const &src);
		FactoryCam &operator=(FactoryCam const &rhs);

};
