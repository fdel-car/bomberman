#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "engine/Engine.hpp"

class Animation {
   public:
	Animation(aiNodeAnim *jointAnimation);
	virtual ~Animation(void);

	aiVectorKey *getPosition(void);
	aiVectorKey *getScale(void);
	aiQuatKey *getRotation(void);
	std::string getName(void);

   private:
	Animation(void);
	Animation(Animation const &src);

	Animation &operator=(Animation const &rhs);

	void _initTransformationVar(void);

	aiNodeAnim *_jointAnimation;
	std::string _name;
	aiVectorKey *_position;
	aiVectorKey *_scale;
	aiQuatKey *_rotation;
};
