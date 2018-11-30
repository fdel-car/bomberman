#include "game/entities/Portal.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/entities/Player.hpp"
#include "game/scenes/SceneTools.hpp"

Portal::Portal(glm::vec3 position, Entity *sceneManager)
	: Entity(glm::vec3(position.x, position.y + 0.3f, position.z), glm::vec3(0),
			 new Collider(Collider::Circle, LayerTag::PortalLayer, 0.1f, 0.1f,
						  true),
			 "Wall", "Portal", "Portal", sceneManager),
	  _hasTriggered(false) {
	scale(glm::vec3(0.3, 2.4, 0.3));
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
