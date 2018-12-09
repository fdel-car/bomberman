#include "game/entities/Box.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/entities/Perk.hpp"
#include "game/scenes/SceneTools.hpp"

Box::Box(glm::vec3 position, Entity *sceneManager, std::string modelName,
		 int perkProb, Entity *toSpawn)
	: Damageable(
		  position, glm::vec3(0.0f),
		  new Collider(Collider::Rectangle, LayerTag::BoxLayer, 0.45f, 0.45f),
		  modelName, "Box", "Box", 1, BoxLayer, WallLayer, 1.0f, sceneManager),
	  _onFire(false),
	  _hasSpawned(false),
	  _timer(1.0f),
	  _perkProb(perkProb),
	  _toSpawn(toSpawn) {}

Box::~Box(void) {
	if (!_hasSpawned && _toSpawn != nullptr) {
		delete _toSpawn;
		_toSpawn = nullptr;
	}
}

void Box::update(void) {
	if (!_onFire) return;

	float deltaTime = _gameEngine->getDeltaTime();
	_timer -= deltaTime;

	if (_timer <= 0.0f) {
		_needToBeDestroyed = true;
		if (_toSpawn != nullptr) {
			_hasSpawned = true;
			_toSpawn->translate(getPosition() - _toSpawn->getPosition());
			_gameEngine->addNewEntity(_toSpawn);
		} else if (static_cast<unsigned int>(rand()) % 100 < _perkProb) {
			_gameEngine->addNewEntity(new Perk(getPosition(), _sceneManager));
		}
	}
}

void Box::onDeath(void) {
	setColor(glm::vec3(0.9, 0.6, 0.1));
	_onFire = true;
}
