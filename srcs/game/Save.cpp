#include <fstream>
#include <iomanip>
#include <iostream>

#include "json/json.hpp"  // https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp

#include "game/Save.hpp"

void to_json(nlohmann::json& j, const Save& p) {
	j = nlohmann::json{{"upKey", p.upKey},
					   {"leftKey", p.leftKey},
					   {"downKey", p.downKey},
					   {"rightKey", p.rightKey},
					   {"level", p.level},
					   {"isFullScreen", p.isFullScreen},
					   {"resolutionsIdx", p.resolutionsIdx},
					   {"musicVolume", p.musicVolume},
					   {"soundsVolume", p.soundsVolume}};
}

void from_json(const nlohmann::json& j, Save& p) {
	j.at("upKey").get_to(p.upKey);
	j.at("leftKey").get_to(p.leftKey);
	j.at("downKey").get_to(p.downKey);
	j.at("rightKey").get_to(p.rightKey);
	j.at("level").get_to(p.level);
	j.at("isFullScreen").get_to(p.isFullScreen);
	j.at("resolutionsIdx").get_to(p.resolutionsIdx);
	j.at("musicVolume").get_to(p.musicVolume);
	j.at("soundsVolume").get_to(p.soundsVolume);
}

const std::vector<std::tuple<std::string, size_t, size_t>> getResolutions() {
	std::vector<std::tuple<std::string, size_t, size_t>> res =
		std::vector<std::tuple<std::string, size_t, size_t>>();
	res.push_back(std::make_tuple("1024x576", 1024, 576));
	res.push_back(std::make_tuple("1280x720 (HD)", 1280, 720));
	res.push_back(std::make_tuple("1920x1080 (FHD)", 1920, 1080));
	res.push_back(std::make_tuple("3840x2160 (4K)", 3840, 2160));
	return res;
}
const std::vector<std::tuple<std::string, size_t, size_t>> Save::RESOLUTIONS =
	getResolutions();

const std::vector<std::tuple<std::string, bool>> getFullScreen() {
	std::vector<std::tuple<std::string, bool>> res =
		std::vector<std::tuple<std::string, bool>>();
	res.push_back(std::make_tuple("No", false));
	res.push_back(std::make_tuple("Yes", true));
	return res;
}
const std::vector<std::tuple<std::string, bool>> Save::FULL_SCREEN =
	getFullScreen();

const std::vector<std::tuple<std::string, int>> getVolumeOptions() {
	std::vector<std::tuple<std::string, int>> res =
		std::vector<std::tuple<std::string, int>>();
	res.push_back(std::make_tuple("0", 0));
	res.push_back(std::make_tuple("1", 1));
	res.push_back(std::make_tuple("2", 2));
	res.push_back(std::make_tuple("3", 3));
	res.push_back(std::make_tuple("4", 4));
	res.push_back(std::make_tuple("5", 5));
	res.push_back(std::make_tuple("6", 6));
	res.push_back(std::make_tuple("7", 7));
	res.push_back(std::make_tuple("8", 8));
	res.push_back(std::make_tuple("9", 9));
	res.push_back(std::make_tuple("10", 10));
	return res;
}
const std::vector<std::tuple<std::string, int>> Save::VOLUMES =
	getVolumeOptions();

Save::Save(void) {
	savePath = __FILE__;
	savePath.erase(savePath.begin() + savePath.rfind("/Save.cpp") + 1,
				   savePath.end());
	savePath += "save.json";

	std::ifstream rFile(savePath);
	if (rFile.is_open()) {
		std::string line;
		std::string allLines = "";

		while (getline(rFile, line)) {
			allLines += line;
		}
		rFile.close();

		nlohmann::json parsedJson = nlohmann::json::parse(allLines.c_str());

		try {
			from_json(parsedJson, *this);
			if (!areValuesGood()) initNewSave();
		} catch (std::exception e) {
			initNewSave();
		}
	} else
		initNewSave();
}

void Save::initNewSave(void) {
	resetAll();
	doSave();
}

void Save::doSave(void) {
	std::ofstream myfile(savePath);
	if (myfile.is_open()) {
		// Check integrity of
		if (!areValuesGood()) resetSettings();

		nlohmann::json myJson = *this;
		myfile << std::setw(4) << myJson << std::endl;
		myfile.close();
	}
}

void Save::resetSettings(void) {
	upKey = KEY_W;
	leftKey = KEY_A;
	downKey = KEY_S;
	rightKey = KEY_D;
	isFullScreen = 0;
	resolutionsIdx = 1;
	musicVolume = 5;
	soundsVolume = 10;
}

void Save::resetLevel(void) { level = 0; }

void Save::resetAll(void) {
	resetSettings();
	resetLevel();
}

bool Save::areValuesGood(void) {
	if (!isalpha(upKey) || !isupper(upKey) || !isalpha(downKey) ||
		!isupper(downKey) || !isalpha(leftKey) || !isupper(leftKey) ||
		!isalpha(rightKey) || !isupper(rightKey))
		return false;
	if (isFullScreen > 1) return false;
	if (resolutionsIdx >= RESOLUTIONS.size()) return false;
	if (musicVolume > 10 || soundsVolume > 10) return false;
	return true;
}
