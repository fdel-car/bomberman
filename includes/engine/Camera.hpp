#pragma once

#include "GUI/GUI.hpp"
#include "engine/Entity.hpp"

class Camera : public Entity {
   public:
	Camera(glm::vec3 const &cameraPos, glm::vec3 const &cameraTarget);
	virtual ~Camera(void);

	glm::mat4 const &getViewMatrix(void) const;
	glm::mat4 const &getProjectionMatrix(void) const;
	std::vector<std::tuple<std::string, std::string>> const &getNeededImages()
		const;
	int getNewSceneIdx(void) const;
	std::string getNewSceneName(void) const;

	virtual void drawGUI(GUI *graphicUI);
	virtual void update(void);
	virtual void configGUI(GUI *graphicUI);

	void initEntity(GameEngine *gameEngine);

   protected:
	int _newSceneIdx;
	std::string _newSceneName;
	std::map<int, nk_color> defaultStyle;
	std::map<int, nk_color> activeStyle;
	std::vector<std::tuple<std::string, std::string>> _neededImages;

   private:
	float _speed;
	float _aspectRatio;
	glm::mat4 _view;
	glm::mat4 _projection;

	glm::vec3 _front;

	Camera(void);
	Camera(Camera const &src);

	void _updateData(void);

	Camera &operator=(Camera const &rhs);
};
