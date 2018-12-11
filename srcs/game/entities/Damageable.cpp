#include "game/entities/Damageable.hpp"
#include "engine/GameEngine.hpp"

Damageable::Damageable(glm::vec3 position, glm::vec3 eulerAngles,
					   Collider *collider, std::string modelName,
					   std::string name, std::string tag, size_t hp,
					   int baseLayer, int damagedLayer, float damagedMaxTime, Entity *sceneManager,
					   glm::vec3 damagedColor)
	: Entity(position, eulerAngles, collider, modelName, name, tag,
			 sceneManager),
	  _alive(hp != 0),
	  _hp(hp),
	  _baseLayer(baseLayer),
	  _damagedLayer(damagedLayer),
	  _damagedMaxTime(damagedMaxTime),
	  _timeDamaged(0.0f),
	  _damagedColor(damagedColor),
	  _doFlicker(false) {}

Damageable::~Damageable(void) {}

void Damageable::update(void) {
	// Every Damageable with > 1 hp must do this
	if (_alive && _timeDamaged > 0.0f) {
		_timeDamaged -= _gameEngine->getDeltaTime();

		if (_doFlicker) {
			_flickerTimer += _gameEngine->getDeltaTime();
			while (_flickerTimer > _flickerHideTime + _flickerDisplayTime)
				_flickerTimer -= _flickerHideTime + _flickerDisplayTime;
			if (_flickerTimer < _flickerHideTime)
				_showModel = false;
			else
				_showModel = true;
		}

		if (_timeDamaged <= 0.0f) {
			_collider->layerTag = _baseLayer;
			setColor(_defaultColor);
			_showModel = true;
		}
	}
}

void Damageable::takeDamage(std::vector<std::string> demagingSounds) {
	if (_alive && _timeDamaged <= 0.0f) {
		onTakeDamage(demagingSounds);
		if (_hp == 0) {
			_alive = false;
			onDeath();
		}
	}
}

void Damageable::onTakeDamage(std::vector<std::string> demagingSounds) {
	if (_alive) {
		_hp -= 1;
		_timeDamaged = _damagedMaxTime;
		_defaultColor = this->getColor();
		setColor(_damagedColor);
		if (_collider) _collider->layerTag = _damagedLayer;
		if (demagingSounds.size() != 0) {
			int randomIdx = rand() % demagingSounds.size();
			_gameEngine->playSound(demagingSounds[randomIdx]);
		}
	}
}

void Damageable::onDeath(void) {
	if (!_alive) {
		if (_collider) _collider->layerTag = _baseLayer;
		_needToBeDestroyed = true;
	}
}

void Damageable::setFlickering(float flickerHideTime,
							   float flickerDisplayTime) {
	_doFlicker = true;
	_flickerTimer = 0.0f;
	_flickerHideTime = flickerHideTime;
	_flickerDisplayTime = flickerDisplayTime;
}