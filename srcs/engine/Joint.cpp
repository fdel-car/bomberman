#include "engine/Joint.hpp"

Joint::Joint(std::string const &name, glm::mat4 const &offsetMatrix, int index)
	: name(name),
	  offsetMatrix(offsetMatrix),
	  index(index),
	  _positionKeys(std::vector<PositionKey>()),
	  _rotationKeys(std::vector<RotationKey>()),
	  _scalingKeys(std::vector<ScalingKey>()) {}

Joint::~Joint(void) {}

void Joint::applyAnimationTransform(double currentAnimTime) {
	PositionKey prevPosition, nextPosition;
	RotationKey prevRotation, nextRotation;
	float mixRatio;

	// Scale not taken into account for now
	for (size_t i = 0; i < _positionKeys.size(); i++) {
		if (currentAnimTime < _positionKeys[i].frameTime) {
			prevPosition = _positionKeys[i - 1];
			nextPosition = _positionKeys[i];
			break;
		}
	}
	mixRatio = (currentAnimTime - prevPosition.frameTime) /
			   (nextPosition.frameTime - prevPosition.frameTime);

	glm::vec3 position = prevPosition.position * mixRatio +
						 nextPosition.position * (1.0f - mixRatio);

	localTransform[3][0] = position.x;
	localTransform[3][1] = position.y;
	localTransform[3][2] = position.z;
	for (size_t i = 0; i < _rotationKeys.size(); i++) {
		if (currentAnimTime < _rotationKeys[i].frameTime) {
			prevRotation = _rotationKeys[i - 1];
			nextRotation = _rotationKeys[i];
			break;
		}
	}
	mixRatio = (currentAnimTime - prevRotation.frameTime) /
			   (nextRotation.frameTime - prevRotation.frameTime);

	glm::quat interpolated =
		glm::slerp(prevRotation.rotation, nextRotation.rotation, mixRatio);
	glm::mat4 tmp = glm::mat4(interpolated);
	localTransform[0][0] = tmp[0][0];
	localTransform[0][1] = tmp[0][1];
	localTransform[0][2] = tmp[0][2];
	localTransform[1][0] = tmp[1][0];
	localTransform[1][1] = tmp[1][1];
	localTransform[1][2] = tmp[1][2];
	localTransform[2][0] = tmp[2][0];
	localTransform[2][1] = tmp[2][1];
	localTransform[2][2] = tmp[2][2];
}

void Joint::updateFinalTransform(void) {
	Joint *tmp = parent;
	std::vector<glm::mat4> transforms;

	finalTransform = _toYAxisUp;
	while (tmp != nullptr) {
		transforms.push_back(tmp->localTransform);
		tmp = tmp->parent;
	}
	for (int i = transforms.size() - 1; i >= 0; i--)
		finalTransform *= transforms[i];
	finalTransform *= localTransform * offsetMatrix;
}

void Joint::setPositionKeys(aiVectorKey *positionKeys, size_t nbr) {
	_positionKeys.resize(nbr);
	for (size_t i = 0; i < nbr; i++) {
		_positionKeys[i].frameTime = positionKeys[i].mTime;
		_positionKeys[i].position =
			glm::vec3(positionKeys[i].mValue.x, positionKeys[i].mValue.y,
					  positionKeys[i].mValue.z);
	}
}

void Joint::setRotationKeys(aiQuatKey *rotationKeys, size_t nbr) {
	_rotationKeys.resize(nbr);
	for (size_t i = 0; i < nbr; i++) {
		_rotationKeys[i].frameTime = rotationKeys[i].mTime;
		_rotationKeys[i].rotation =
			glm::quat(rotationKeys[i].mValue.w, rotationKeys[i].mValue.x,
					  rotationKeys[i].mValue.y, rotationKeys[i].mValue.z);
	}
}

void Joint::setScalingKeys(aiVectorKey *scalingKeys, size_t nbr) {
	_scalingKeys.resize(nbr);
	for (size_t i = 0; i < nbr; i++) {
		_scalingKeys[i].frameTime = scalingKeys[i].mTime;
		_scalingKeys[i].scaling =
			glm::vec3(scalingKeys[i].mValue.x, scalingKeys[i].mValue.y,
					  scalingKeys[i].mValue.z);
	}
}

glm::mat4 Joint::_toYAxisUp = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
										  glm::vec3(1.0, 0.0, 0.0));