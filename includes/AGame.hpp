#pragma once

#include "Camera.hpp"
#include "Entity.hpp"

class AGame {
   public:
	AGame(void);
	virtual ~AGame(void);

	virtual bool loadScene(size_t sceneIdx) = 0;

	std::vector<std::tuple<float, std::string, std::string>> &getNeededFont();
	std::vector<Entity *> const getEntities() const;
	Camera *getCamera() const;

	void unload(void);

   protected:
	size_t _sceneIdx;
	std::vector<Entity *> _entities;
	Camera *_camera;
	std::vector<std::tuple<float, std::string, std::string>> _neededFonts;
};
