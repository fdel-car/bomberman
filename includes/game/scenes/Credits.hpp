#pragma once

#include "engine/GameEngine.hpp"
#include "game/scenes/SceneTools.hpp"

class Credits : public SceneTools {
   public:
	Credits(WorldLocation &startLocation, Bomberman *bomberman);
	Credits(Credits const &src);

	Credits &operator=(Credits const &rhs);

	virtual void drawGUI(GUI *graphicUI);
	virtual void configGUI(GUI *graphicUI);

   private:
	Credits(void);
	virtual ~Credits(void);

	void _creditsWindow(GUI *graphicUI, std::string id, int x, int y,
						size_t width, size_t height,
						std::vector<std::tuple<std::string, std::string>> texts,
						int rowSize = 30, std::string imageName = "",
						bool imageBeforeText = true);
	void _evolveCoords(int *x, int *y, int yEvolution, bool neverStop,
					   int xNextEvolution, int yNextEvolution, int height);

	std::vector<std::tuple<std::string, std::string>> _jimmy;
	std::vector<std::tuple<std::string, std::string>> _fabien;
	std::vector<std::tuple<std::string, std::string>> _claudio;
	std::vector<std::tuple<std::string, std::string>> _vivien;

	std::vector<std::tuple<std::string, std::string>> _credits;

	std::vector<std::tuple<int, int>> coords;

	bool jimFinish = false;
	bool fabFinish = false;
	bool claFinish = false;
	bool vivFinish = false;
	bool textFinish = false;
};
