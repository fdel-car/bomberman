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

class Joint {
   public:
	Joint(std::string const &name, glm::mat4 const &offsetMatrix, int index);

	virtual ~Joint(void);

	std::string const name;
	glm::mat4 const offsetMatrix;
	int const index;
	Joint *parent;
	glm::mat4 localTransform;
	glm::mat4 finalTransform;

	// Animation
	std::vector<PositionKey> _positionKeys;
	std::vector<RotationKey> _rotationKeys;
	std::vector<ScalingKey> _scalingKeys;

	void applyAnimationTransform(double currentAnimTime);
	void updateFinalTransform(void);
	void setPositionKeys(aiVectorKey *positionKeys, size_t nbr);
	void setRotationKeys(aiQuatKey *rotationKeys, size_t nbr);
	void setScalingKeys(aiVectorKey *scalingKeys, size_t nbr);

   private:
	static glm::mat4 _toYAxisUp;

	Joint(void);
	Joint(Joint const &src);

	Joint &operator=(Joint const &rhs);
};