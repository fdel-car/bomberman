#ifndef SAVE_HPP
#define SAVE_HPP

#include <string>
#include <tuple>
#include <vector>

struct Save {
   public:
	static const std::vector<std::tuple<std::string, size_t, size_t>>
		RESOLUTIONS;
	static const std::vector<std::tuple<std::string, bool>> FULL_SCREEN;

	std::string upKey = "W";
	std::string leftKey = "A";
	std::string downKey = "S";
	std::string rightKey = "D";
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
	std::string savePath;
};

#endif