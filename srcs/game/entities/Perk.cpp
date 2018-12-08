#include "game/entities/Perk.hpp"
#include "engine/GameEngine.hpp"
#include "game/Bomberman.hpp"
#include "game/entities/Player.hpp"
#include "game/scenes/SceneTools.hpp"

extern std::string _assetsDir;

const float Perk::TIME_BETWEEN_ANIM = 3.0f;

const std::vector<std::tuple<PerkType, size_t, std::string>> getPossiblePerks(
	void) {
	std::vector<std::tuple<PerkType, size_t, std::string>> vPerk;
	vPerk.push_back(std::make_tuple<PerkType, size_t, std::string>(
		SpeedBoost, 1, "SpeedPerk"));
	vPerk.push_back(std::make_tuple<PerkType, size_t, std::string>(
		BombRange, 1, "RangePerk"));
	vPerk.push_back(std::make_tuple<PerkType, size_t, std::string>(
		MaxBomb, 1, "MaxBombPerk"));
	vPerk.push_back(std::make_tuple<PerkType, size_t, std::string>(
		Damage, 1, "DamagePerk"));
	vPerk.push_back(std::make_tuple<PerkType, size_t, std::string>(KickBomb, 1,
																   "KickPerk"));
	return vPerk;
}
const std::vector<std::tuple<PerkType, size_t, std::string>>
	Perk::POSSIBLE_PERKS = getPossiblePerks();

size_t getTotalPerkProbability(
	std::vector<std::tuple<PerkType, size_t, std::string>> const &vPerk) {
	size_t tot = 0;
	for (auto const &i : vPerk) {
		tot += std::get<1>(i);
	}
	return tot;
}
const size_t Perk::TOTAL_PERK_PROBABILITY =
	getTotalPerkProbability(Perk::POSSIBLE_PERKS);

const std::vector<std::string> getDamagingSounds(void) {
	std::vector<std::string> vSounds;
	vSounds.push_back("bad_perk");
	return vSounds;
}
const std::vector<std::string> Perk::damagingSounds = getDamagingSounds();

Perk::Perk(glm::vec3 position, Entity *sceneManager)
	: Entity(glm::vec3(position.x, position.y + 0.3f, position.z), glm::vec3(0),
			 new Collider(Collider::Rectangle, LayerTag::PerkLayer, 0.4f, 0.4f,
						  true),
			 "SetPerk", "Perk", "Perk", sceneManager),
	  _timer(TIME_BETWEEN_ANIM) {
	size_t randResult = rand() % TOTAL_PERK_PROBABILITY;
	for (auto const &perk : POSSIBLE_PERKS) {
		if (std::get<1>(perk) > randResult) {
			_perkType = std::get<0>(perk);
			_modelName = std::get<2>(perk);
			break;
		} else {
			randResult -= std::get<1>(perk);
		}
	}
	_destroySounds.push_back("get_perk_1");
	_destroySounds.push_back("get_perk_2");
	_destroySounds.push_back("get_perk_3");
	_destroySounds.push_back("get_perk_4");
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
				player->gotSpeedBoost(0.3f);
				break;
			case BombRange:
				player->gotBombRangeBoost(1);
				break;
			case MaxBomb:
				player->gotMaxBombBoost(1);
				break;
			case Damage:
				player->takeDamage(damagingSounds);
				_destroySounds.clear();
				break;
			case KickBomb:
				player->gotBombKickBoost(true);
				break;
			default:
				break;
		}
		_needToBeDestroyed = true;
	}
}
