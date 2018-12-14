#ifndef SAVE_HPP
#define SAVE_HPP

#include "engine/Engine.hpp"

struct Save {
   public:
	static const std::vector<std::tuple<std::string, size_t, size_t>>
		RESOLUTIONS;
	static const std::vector<std::tuple<std::string, bool>> FULL_SCREEN;
	static const std::vector<std::tuple<std::string, int>> VOLUMES;

	int upKey = KEY_W;
	int leftKey = KEY_A;
	int downKey = KEY_S;
	int rightKey = KEY_D;
	size_t level = 0;
	size_t isFullScreen = 0;
	size_t resolutionsIdx = 0;
	size_t musicVolume = 10;
	size_t soundsVolume = 10;

	Save();
	virtual ~Save(void) {}

	void initNewSave(void);
	void doSave(void);
	void resetSettings(void);
	void resetLevel(void);
	void resetAll(void);
	bool areValuesGood(void);

   private:
	Save(Save const &src);
	Save &operator=(Save const &rhs);
	std::string savePath;
};

#endif