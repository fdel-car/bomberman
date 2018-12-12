#include "engine/Joint.hpp"

Joint::Joint(std::string const &name, glm::mat4 const &offsetMatrix, int index)
	: name(name), offsetMatrix(offsetMatrix), index(index) {}

Joint::~Joint(void) {}

void Joint::applyAnimationTransform(double currentAnimTime,
									std::string const &currentAnimName) {
	PositionKey prevPosition, nextPosition;
	RotationKey prevRotation, nextRotation;
	float mixRatio;

	// Scale not taken into account for now
	for (size_t i = 0; i < _animations[currentAnimName].positionKeys.size();
		 i++) {
		if (currentAnimTime <
			_animations[currentAnimName].positionKeys[i].frameTime) {
			prevPosition = _animations[currentAnimName].positionKeys[i - 1];
			nextPosition = _animations[currentAnimName].positionKeys[i];
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
	for (size_t i = 0; i < _animations[currentAnimName].rotationKeys.size();
		 i++) {
		if (currentAnimTime <
			_animations[currentAnimName].rotationKeys[i].frameTime) {
			prevRotation = _animations[currentAnimName].rotationKeys[i - 1];
			nextRotation = _animations[currentAnimName].rotationKeys[i];
			break;
		}
	}
	mixRatio = (currentAnimTime - prevRotation.frameTime) /
			   (nextRotation.frameTime - prevRotation.frameTime);

	glm::mat4 interpolated = glm::mat4(
		glm::slerp(prevRotation.rotation, nextRotation.rotation, mixRatio));
	localTransform[0][0] = interpolated[0][0];
	localTransform[0][1] = interpolated[0][1];
	localTransform[0][2] = interpolated[0][2];
	localTransform[1][0] = interpolated[1][0];
	localTransform[1][1] = interpolated[1][1];
	localTransform[1][2] = interpolated[1][2];
	localTransform[2][0] = interpolated[2][0];
	localTransform[2][1] = interpolated[2][1];
	localTransform[2][2] = interpolated[2][2];
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

void Joint::setPositionKeys(std::string const &animName,
							aiVectorKey *positionKeys, size_t nbr) {
	if (_animations.find(animName) == _animations.end())
		_animations[animName] = Animation();

	_animations[animName].positionKeys.resize(nbr);
	for (size_t i = 0; i < nbr; i++) {
		_animations[animName].positionKeys[i].frameTime = positionKeys[i].mTime;
		_animations[animName].positionKeys[i].position =
			glm::vec3(positionKeys[i].mValue.x, positionKeys[i].mValue.y,
					  positionKeys[i].mValue.z);
	}
}

void Joint::setRotationKeys(std::string const &animName,
							aiQuatKey *rotationKeys, size_t nbr) {
	if (_animations.find(animName) == _animations.end())
		_animations[animName] = Animation();

	_animations[animName].rotationKeys.resize(nbr);
	for (size_t i = 0; i < nbr; i++) {
		_animations[animName].rotationKeys[i].frameTime = rotationKeys[i].mTime;
		_animations[animName].rotationKeys[i].rotation =
			glm::quat(rotationKeys[i].mValue.w, rotationKeys[i].mValue.x,
					  rotationKeys[i].mValue.y, rotationKeys[i].mValue.z);
	}
}

void Joint::setScalingKeys(std::string const &animName,
						   aiVectorKey *scalingKeys, size_t nbr) {
	if (_animations.find(animName) == _animations.end())
		_animations[animName] = Animation();

	_animations[animName].scalingKeys.resize(nbr);
	for (size_t i = 0; i < nbr; i++) {
		_animations[animName].scalingKeys[i].frameTime = scalingKeys[i].mTime;
		_animations[animName].scalingKeys[i].scaling =
			glm::vec3(scalingKeys[i].mValue.x, scalingKeys[i].mValue.y,
					  scalingKeys[i].mValue.z);
	}
}

glm::mat4 Joint::_toYAxisUp = glm::mat4(1.0f);
// glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
//										  glm::vec3(1.0, 0.0, 0.0));