#pragma once

#include "AGameScene.hpp"

class AGame {

	public:
		AGame(void);
		virtual ~AGame(void);

		std::vector<AGameScene *> & getGameScenes();
		std::vector<std::tuple<float, std::string, std::string>> & getNeededFont();

	protected:
		std::vector<AGameScene *> gameScenes;
		std::vector< std::tuple< float, std::string, std::string > > vNeededFont;

};
