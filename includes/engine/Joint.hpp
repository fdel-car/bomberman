#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "engine/Engine.hpp"

struct PositionKey {
	double frameTime;
	glm::vec3 position;
};

struct RotationKey {
	double frameTime;
	glm::quat rotation;
};

struct ScalingKey {
	double frameTime;
	glm::vec3 scaling;
};

struct Animation {
	std::vector<PositionKey> positionKeys;
	std::vector<RotationKey> rotationKeys;
	std::vector<ScalingKey> scalingKeys;
};

class Joint final {
   public:
	Joint(std::string const &name, glm::mat4 const &offsetMatrix, int index);

	virtual ~Joint(void);

	std::string const name;
	glm::mat4 const offsetMatrix;
	int const index;
	Joint *parent = nullptr;
	glm::mat4 localTransform;
	glm::mat4 finalTransform;

	void applyAnimationTransform(double animTime, std::string const &animName);
	void updateFinalTransform(void);
	void setPositionKeys(std::string const &animName, aiVectorKey *positionKeys,
						 size_t nbr);
	void setRotationKeys(std::string const &animName, aiQuatKey *rotationKeys,
						 size_t nbr);
	void setScalingKeys(std::string const &animName, aiVectorKey *scalingKeys,
						size_t nbr);

   private:
	static glm::mat4 _toYAxisUp;
	std::map<std::string, Animation> _animations;

	Joint(void);
	Joint(Joint const &src);

	Joint &operator=(Joint const &rhs);
};