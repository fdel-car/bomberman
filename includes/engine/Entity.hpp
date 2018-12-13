#pragma once

#include <map>

#include "engine/Collider.hpp"
#include "engine/Model.hpp"

class GameEngine;

class Entity {
   public:
	static void resetSpawnedEntities(void);

	Entity(glm::vec3 position, glm::vec3 eulerAngles, Collider *collider,
		   std::string modelName, std::string name, std::string tag,
		   Entity *sceneManager = nullptr);
	virtual ~Entity(void);

	// Animation related
	double currentAnimTime = 0.0;
	std::string currentAnimName = "Idle";
	bool loopAnim = true;
	bool shouldBeAnimated = false;
	float currentAnimSpeed = 1.0f;

	GameEngine *getGameEngine(void) const;
	const glm::vec3 &getPosition(void) const;
	const glm::mat4 &getModelMatrix(void) const;
	const Collider *getCollider(void) const;
	Model *getModel(void) const;
	glm::vec3 const &getEulerAngles(void) const;
	size_t const &getId(void) const;
	std::string const &getName(void) const;
	std::string const &getTag(void) const;
	std::string const &getModelName(void) const;
	glm::vec3 const &getTargetMovement(void) const;
	glm::vec3 const &getColor(void) const;
	bool needsToBeDestroyed(void) const;
	std::set<std::string> getNeededSounds(void) const;
	bool doShowModel(void) const;

	virtual void update(void);
	virtual void initEntity(GameEngine *gameEngine);
	virtual void tellPosition(Entity *entity);
	virtual void tellDestruction(Entity *entity);
	virtual void onCollisionEnter(Entity *entity);
	virtual void onTriggerEnter(Entity *entity);

	void updateModel(void);
	void setColor(glm::vec3 const &color);
	void resetColor(void);
	void scale(glm::vec3 scale);
	void rotate(glm::vec3 axis, float angle);
	void rotate(float angle, glm::vec3 axis);
	void rotateX(float angle);
	void rotateY(float angle);
	virtual void translate(glm::vec3 translation);

   private:
	glm::vec3 _position;
	glm::vec3 _eulerAngles;
	glm::vec3 _color = glm::vec3(-1.0f);

	glm::mat4 _scaleMatrix = glm::mat4(1.0f);
	glm::mat4 _rotationMatrix;
	glm::mat4 _translationMatrix = glm::mat4(1.0f);
	glm::mat4 _modelMatrix = glm::mat4(1.0f);

	Entity &operator=(Entity const &rhs);

	void _updateModelMatrix(void);

   protected:
	static size_t _spawnedEntities;

	size_t _id;
	bool _needToBeDestroyed = false;
	bool _localOrientation = true;
	std::string _modelName;
	std::string _name;
	std::string _tag;
	Entity *_sceneManager = nullptr;

	bool _showModel = true;
	Model *_model = nullptr;
	Collider *_collider = nullptr;
	GameEngine *_gameEngine = nullptr;

	glm::vec3 _targetMovement;
	std::set<std::string> _neededSounds;
	std::vector<std::string> _initSounds;
	std::vector<std::string> _destroySounds;

	Entity(void);
	Entity(Entity const &src);
};

std::ostream &operator<<(std::ostream &o, Entity const &entity);