#pragma once

#include "Entity.hpp"
#include "GUI/GUI.hpp"

class Camera : public Entity {
   public:
	Camera(glm::vec3 const &cameraPos, glm::vec3 const &cameraTarget);
	virtual ~Camera(void);

	glm::mat4 const &getViewMatrix(void) const;
	virtual void drawGUI(GUI * graphicUI);
	int getNewSceneIdx(void);

  protected:
	int _newSceneIdx;
	glm::mat4 _view;

	Camera(void);
	Camera(Camera const &src);

	Camera &operator=(Camera const &rhs);
};
