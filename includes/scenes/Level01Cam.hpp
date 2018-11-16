#pragma once

#include "Camera.hpp"

class Level01Cam : public Camera {
	public:
		Level01Cam(glm::vec3 const &pos, glm::vec3 const &eulerAngles);
		virtual ~Level01Cam(void);

		void drawGUI(GUI * graphicUI);

	private:
		Level01Cam(void);
		Level01Cam(Level01Cam const &src);
		Level01Cam &operator=(Level01Cam const &rhs);

};
