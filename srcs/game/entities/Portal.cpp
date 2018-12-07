#include "game/entities/Portal.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/entities/Player.hpp"
#include "game/scenes/SceneTools.hpp"

extern std::string _assetsDir;

Portal::Portal(glm::vec3 position, Entity *sceneManager)
	: Entity(glm::vec3(position.x, position.y, position.z), glm::vec3(0),
			 new Collider(Collider::Circle, LayerTag::PortalLayer, 0.15f, 0.15f,
						  true),
			 "Portal", "Portal", "Portal", sceneManager),
	  _hasTriggered(false) {
	translate(glm::vec3(0.0, 0.0, 0.06));
	_initSounds.push_back("portal_spawn");
}

Portal::~Portal(void) {}

void Portal::onTriggerEnter(Entity *entity) {
	if (_hasTriggered) return;
	Player *player = dynamic_cast<Player *>(entity);

	if (player != nullptr) {
		glm::vec3 distance = getPosition() - player->getPosition();
		if (abs(distance.x) <= 0.1 && abs(distance.z) <= 0.1) {
			SceneTools *sceneManager =
				dynamic_cast<SceneTools *>(_sceneManager);
			if (sceneManager != nullptr) {
				sceneManager->tellLevelSuccess();
				_hasTriggered = true;
			}
		}
	}
}
