#include "game/entities/Perk.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/entities/Player.hpp"
#include "game/scenes/SceneTools.hpp"

const float Perk::TIME_BETWEEN_ANIM = 3.0f;

const std::vector<std::tuple<PerkType, size_t>> getPossiblePerks(void) {
	std::vector<std::tuple<PerkType, size_t>> vPerk;
	vPerk.push_back(std::make_tuple<PerkType, size_t>(SpeedBoost, 4));
	vPerk.push_back(std::make_tuple<PerkType, size_t>(BombRange, 2));
	vPerk.push_back(std::make_tuple<PerkType, size_t>(MaxBomb, 2));
	vPerk.push_back(std::make_tuple<PerkType, size_t>(Damage, 1));
	vPerk.push_back(std::make_tuple<PerkType, size_t>(KickBomb, 3));
	return vPerk;
}
const std::vector<std::tuple<PerkType, size_t>> Perk::POSSIBLE_PERKS =
	getPossiblePerks();

size_t getTotalPerkProbability(
	std::vector<std::tuple<PerkType, size_t>> const &vPerk) {
	size_t tot = 0;
	for (auto const &i : vPerk) {
		tot += std::get<1>(i);
	}
	return tot;
}
const size_t Perk::TOTAL_PERK_PROBABILITY =
	getTotalPerkProbability(Perk::POSSIBLE_PERKS);

Perk::Perk(glm::vec3 position, Entity *sceneManager)
	: Entity(glm::vec3(position.x, position.y + 0.3f, position.z), glm::vec3(0),
			 new Collider(Collider::Rectangle, LayerTag::PerkLayer, 0.4f, 0.4f,
						  true),
			 "Box", "Perk", "Perk", sceneManager),
	  _timer(TIME_BETWEEN_ANIM) {
	scale(glm::vec3(0.8, 0.4, 0.8));

	size_t randResult = rand() % TOTAL_PERK_PROBABILITY;
	for (auto const &perk : POSSIBLE_PERKS) {
		if (std::get<1>(perk) > randResult) {
			_perkType = std::get<0>(perk);
			break;
		} else {
			randResult -= std::get<1>(perk);
		}
	}
}

Perk::~Perk(void) {}

void Perk::update(void) {
	float deltaTime = _gameEngine->getDeltaTime();
	_timer -= deltaTime;

	if (_timer <= 0.0f) {
		// TODO: do anim
		_timer = TIME_BETWEEN_ANIM;
	}
}

void Perk::onTriggerEnter(Entity *entity) {
	Player *player = dynamic_cast<Player *>(entity);

	if (player != nullptr) {
		switch (_perkType) {
			case SpeedBoost:
				std::cout << "SpeedBoost Perk" << std::endl;
				player->gotSpeedBoost(0.3f);
				break;
			case BombRange:
				std::cout << "BombRange Perk" << std::endl;
				player->gotBombRangeBoost(1);
				break;
			case MaxBomb:
				std::cout << "MaxBomb Perk" << std::endl;
				player->gotMaxBombBoost(1);
				break;
			case Damage:
				std::cout << "Damage Perk" << std::endl;
				player->takeDamage();
				break;
			case KickBomb:
				std::cout << "KickBomb Perk" << std::endl;
				player->gotBombKickBoost(true);
				break;
			default:
				std::cout << "No Perk" << std::endl;
				break;
		}
		_needToBeDestroyed = true;
	}
}
