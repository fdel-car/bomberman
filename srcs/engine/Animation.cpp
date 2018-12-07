#include "engine/Animation.hpp"

Animation::Animation(aiNodeAnim *jointAnimation)
	: _jointAnimation(jointAnimation) {
	_name = jointAnimation->mNodeName.C_Str();
	_initTransformationVar();
}

Animation::Animation(Animation const &src) { *this = src; }

Animation::~Animation(void) {}

Animation &Animation::operator=(Animation const &rhs) {
	return *this;
	(void)rhs;
}

aiVectorKey *Animation::getPosition(void) { return _position; }

aiVectorKey *Animation::getScale(void) { return _scale; }

aiQuatKey *Animation::getRotation(void) { return _rotation; }

std::string Animation::getName(void) { return _name; }

void Animation::_initTransformationVar(void) {
	if (_jointAnimation->mNumPositionKeys == 0)
		_position = nullptr;
	else {
		// If there are position keys, there will also be at least one scaling
		// and one rotation key.
		std::cout << _jointAnimation->mNumPositionKeys << std::endl;
		_position = _jointAnimation->mPositionKeys;
		_scale = _jointAnimation->mScalingKeys;
		_rotation = _jointAnimation->mRotationKeys;
	}
}