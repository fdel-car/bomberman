#include "engine/Joint.hpp"

Joint::Joint(std::string const &name, glm::mat4 const &offsetMatrix, int index)
	: name(name), offsetMatrix(offsetMatrix), index(index) {}

Joint::~Joint(void) { _animations.clear(); }

void Joint::applyAnimationTransform(double currentAnimTime,
									std::string const &currentAnimName) {
	PositionKey prevPosition, nextPosition;
	RotationKey prevRotation, nextRotation;
	ScalingKey prevScaling, nextScaling;
	float mixRatio;

	for (size_t i = 0; i < _animations[currentAnimName].scalingKeys.size();
		 i++) {
		if (currentAnimTime <
			_animations[currentAnimName].scalingKeys[i].frameTime) {
			prevScaling = _animations[currentAnimName].scalingKeys[i - 1];
			nextScaling = _animations[currentAnimName].scalingKeys[i];
			break;
		}
	}
	mixRatio = (currentAnimTime - prevScaling.frameTime) /
			   (nextScaling.frameTime - prevScaling.frameTime);
	glm::mat4 scalingMatrix =
		glm::scale(glm::mat4(1.0f), prevScaling.scaling * (1.0f - mixRatio) +
										nextScaling.scaling * mixRatio);

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
	glm::mat4 translationMatrix = glm::translate(
		glm::mat4(1.0f), prevPosition.position * (1.0f - mixRatio) +
							 nextPosition.position * mixRatio);

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
	glm::mat4 rotationMatrix = glm::mat4(
		glm::slerp(prevRotation.rotation, nextRotation.rotation, mixRatio));

	// Compute localTransform final value
	localTransform = translationMatrix * rotationMatrix * scalingMatrix;
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

glm::mat4 Joint::_toYAxisUp = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
										  glm::vec3(1.0, 0.0, 0.0));