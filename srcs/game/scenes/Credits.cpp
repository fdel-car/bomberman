#include "game/scenes/Credits.hpp"
#include "engine/GameRenderer.hpp"

extern std::string _assetsDir;

Credits::Credits(WorldLocation &startLocation, Bomberman *bomberman)
	: SceneTools(0, 0, startLocation, bomberman, "Credits", "MainMenu") {
	_startMusic = "Audio/Musics/Credits.wav";
	_neededImages.push_back(std::tuple<std::string, std::string, bool>(
		(_assetsDir + "GUI/icons/jimmy.jpg"), "jimmy", false));
	_neededImages.push_back(std::tuple<std::string, std::string, bool>(
		(_assetsDir + "GUI/icons/claudio.jpg"), "claudio", false));
	_neededImages.push_back(std::tuple<std::string, std::string, bool>(
		(_assetsDir + "GUI/icons/fabien.jpg"), "fabien", false));
	_neededImages.push_back(std::tuple<std::string, std::string, bool>(
		(_assetsDir + "GUI/icons/vivien.jpg"), "vivien", false));
}

Credits::~Credits(void) {}

void Credits::configGUI(GUI *graphicUI) {
	graphicUI->setAssetImages(_neededImages);
	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight()));
	_jimmy.push_back("Jimmy");
	_jimmy.push_back("Chen Ma");

	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight() * 2));
	_fabien.push_back("Fabien");
	_fabien.push_back("Del Carmen");

	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight() * 3));
	_claudio.push_back("Claudio");
	_claudio.push_back("Mutti");

	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight() * 4));
	_vivien.push_back("Vivien");
	_vivien.push_back("Klaousen");

	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight() * 5));
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
	_credits.push_back("Test1");
}

void Credits::drawGUI(GUI *graphicUI) {
	size_t width = _gameEngine->getGameRenderer()->getWidth() / 3;
	size_t height = _gameEngine->getGameRenderer()->getHeight() / 3;

	_evolveCoords(&std::get<0>(coords[0]), &std::get<1>(coords[0]), -4, false,
				  8, 4, height);
	_creditsWindow(graphicUI, "jimmy", std::get<0>(coords[0]),
				   std::get<1>(coords[0]), width, height, _jimmy, "", 30,
				   "jimmy", true);

	_evolveCoords(&std::get<0>(coords[1]), &std::get<1>(coords[1]), -4, false,
				  -8, 4, height);
	_creditsWindow(graphicUI, "fabien", std::get<0>(coords[1]),
				   std::get<1>(coords[1]), width, height, _fabien, "", 30,
				   "fabien", true);

	_evolveCoords(&std::get<0>(coords[2]), &std::get<1>(coords[2]), -4, false,
				  8, -4, height);
	_creditsWindow(graphicUI, "claudio", std::get<0>(coords[2]),
				   std::get<1>(coords[2]), width, height, _claudio, "", 30,
				   "claudio", false);

	_evolveCoords(&std::get<0>(coords[3]), &std::get<1>(coords[3]), -4, false,
				  -8, -4, height);
	_creditsWindow(graphicUI, "vivien", std::get<0>(coords[3]),
				   std::get<1>(coords[3]), width, height, _vivien, "", 30,
				   "vivien", false);

	_evolveCoords(&std::get<0>(coords[4]), &std::get<1>(coords[4]), -4, true,
				  -2, -4, height * 3);
	_creditsWindow(graphicUI, "credits", std::get<0>(coords[4]),
				   std::get<1>(coords[4]), width, height * 3, _credits);

	if (_gameEngine->isKeyJustPressed(KEY_SPACE)) _newSceneName = "MainMenu";
}

void Credits::_creditsWindow(GUI *graphicUI, std::string id, int x, int y,
							 size_t width, size_t height,
							 std::vector<std::string> texts,
							 std::string fontName, int rowSize,
							 std::string imageName, bool imageBeforeText) {
	if (graphicUI->uiStartBlock(id.c_str(), "", nk_rect(x, y, width, height),
								NK_WINDOW_NO_SCROLLBAR)) {
		int imgHeight = height - (texts.size() * rowSize);
		imgHeight = imgHeight < 0 ? 0 : imgHeight - 30;
		int imgWidth = ((width / 3) - imgHeight) / 2 + (width / 3);
		if (imageBeforeText) {
			graphicUI->uiRowMultipleElem(true, imgHeight, 1);
			graphicUI->uiAddElemInRow(imgHeight);
			graphicUI->uiAddElemOffset(imgWidth);
			graphicUI->uiSetImage(imageName);
			graphicUI->uiRowMultipleElem(false);
		}
		for (const auto &text : texts)
			graphicUI->uiHeader(text.c_str(), NK_TEXT_CENTERED, rowSize,
								fontName);
		if (!imageBeforeText) {
			graphicUI->uiRowMultipleElem(true, imgHeight, 1);
			graphicUI->uiAddElemInRow(imgHeight);
			graphicUI->uiAddElemOffset(imgWidth);
			graphicUI->uiSetImage(imageName);
			graphicUI->uiRowMultipleElem(false);
		}
	}
	graphicUI->uiEndBlock();
}

void Credits::_evolveCoords(int *x, int *y, int yEvolution, bool neverStop,
							int xNextEvolution, int yNextEvolution,
							int height) {
	if (neverStop && *y >= -height)
		*y += yEvolution;
	else if (!neverStop) {
		int xStop = xNextEvolution < 0
						? 0
						: (_gameEngine->getGameRenderer()->getWidth() / 3) * 2;
		int yStop = yNextEvolution < 0
						? 0
						: (_gameEngine->getGameRenderer()->getHeight() / 3) * 2;
		if (*y > _gameEngine->getGameRenderer()->getHeight() / 3 &&
			*x == _gameEngine->getGameRenderer()->getWidth() / 3)
			*y += yEvolution;
		else if (*x != xStop && *y != yStop) {
			*x += xNextEvolution;
			*y += yNextEvolution;
		}
	}
}
