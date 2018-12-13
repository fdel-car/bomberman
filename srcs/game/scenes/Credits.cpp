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
	_jimmy.push_back(
		std::tuple<std::string, std::string>("Jimmy", "24_slider"));
	_jimmy.push_back(
		std::tuple<std::string, std::string>("Chen Ma", "24_slider"));

	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight()));
	_fabien.push_back(
		std::tuple<std::string, std::string>("Fabien", "24_slider"));
	_fabien.push_back(
		std::tuple<std::string, std::string>("Del Carmen", "24_slider"));

	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight()));
	_claudio.push_back(
		std::tuple<std::string, std::string>("Claudio", "24_slider"));
	_claudio.push_back(
		std::tuple<std::string, std::string>("Mutti", "24_slider"));

	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight()));
	_vivien.push_back(
		std::tuple<std::string, std::string>("Vivien", "24_slider"));
	_vivien.push_back(
		std::tuple<std::string, std::string>("Klaousen", "24_slider"));

	coords.push_back(
		std::tuple<int, int>(_gameEngine->getGameRenderer()->getWidth() / 3,
							 _gameEngine->getGameRenderer()->getHeight()));

	_credits.push_back(
		std::tuple<std::string, std::string>("Bomberman", "48_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("A 42 project", "36_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Developers", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Jimmy Chen Ma", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Rendering", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Jimmy Chen Ma", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("GUI", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(std::tuple<std::string, std::string>(
		"GameEngine Architecture", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(std::tuple<std::string, std::string>("AI", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Models", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Shadows", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Jimmy Chen Ma", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Skybox", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Jimmy Chen Ma", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Animations", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(std::tuple<std::string, std::string>(
		"Background Threads", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Jimmy Chen Ma", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Assets Creation", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Jimmy Chen Ma", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Level Design", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Music & Sounds", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Dialogues", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Save Mecanic", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Scrum Master", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Product Owner", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Gimp hater", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(std::tuple<std::string, std::string>(
		"User Experience Expert", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Gimp supporter", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Blender Guru", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Nuklear lead dev", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));

	_credits.push_back(std::tuple<std::string, std::string>("", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Pokemon Fans", "34_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Jimmy Chen Ma", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Fabien Del Carmen", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Vivien Klaousen", "20_slider"));
	_credits.push_back(
		std::tuple<std::string, std::string>("Claudio Mutti", "20_slider"));
}

void Credits::drawGUI(GUI *graphicUI) {
	size_t width = _gameEngine->getGameRenderer()->getWidth() / 3;
	size_t height = _gameEngine->getGameRenderer()->getHeight() / 3;
	_evolveCoords(&std::get<0>(coords[0]), &std::get<1>(coords[0]), -2, false,
				  4, 2, height);
	_creditsWindow(graphicUI, "jimmy", std::get<0>(coords[0]),
				   std::get<1>(coords[0]), width, height, _jimmy, 30, "jimmy",
				   true);

	if (!jimFinish) {
		if (static_cast<int>(height) < std::get<1>(coords[0])) {
			return;
		}
		jimFinish = true;
	}
	_evolveCoords(&std::get<0>(coords[1]), &std::get<1>(coords[1]), -2, false,
				  -4, 2, height);
	_creditsWindow(graphicUI, "fabien", std::get<0>(coords[1]),
				   std::get<1>(coords[1]), width, height, _fabien, 30, "fabien",
				   true);

	if (!fabFinish) {
		if (static_cast<int>(height) < std::get<1>(coords[1])) {
			return;
		}
		fabFinish = true;
	}
	_evolveCoords(&std::get<0>(coords[2]), &std::get<1>(coords[2]), -2, false,
				  4, -2, height);
	_creditsWindow(graphicUI, "claudio", std::get<0>(coords[2]),
				   std::get<1>(coords[2]), width, height, _claudio, 30,
				   "claudio", false);

	if (!claFinish) {
		if (static_cast<int>(height) < std::get<1>(coords[2])) {
			return;
		}
		claFinish = true;
	}
	_evolveCoords(&std::get<0>(coords[3]), &std::get<1>(coords[3]), -2, false,
				  -4, -2, height);
	_creditsWindow(graphicUI, "vivien", std::get<0>(coords[3]),
				   std::get<1>(coords[3]), width, height, _vivien, 30, "vivien",
				   false);

	if (!vivFinish) {
		if (static_cast<int>(height) < std::get<1>(coords[3])) {
			return;
		}
		vivFinish = true;
	}
	_evolveCoords(&std::get<0>(coords[4]), &std::get<1>(coords[4]), -1, true,
				  -2, -4, height * 13);
	_creditsWindow(graphicUI, "credits", std::get<0>(coords[4]),
				   std::get<1>(coords[4]), width, height * 13, _credits, 30, "",
				   false);

	if (!textFinish) {
		if (-(static_cast<int>(height * 13)) < std::get<1>(coords[4])) {
			return;
		}
		textFinish = true;
	}
	if (graphicUI->uiStartBlock("end", "",
								nk_rect(width, height, width, height),
								NK_WINDOW_NO_SCROLLBAR)) {
		graphicUI->uiHeader("Thanks for playing!", NK_TEXT_CENTERED, 30,
							"40_slider");
		graphicUI->uiHeader("Press space to exit", NK_TEXT_CENTERED, 30,
							"18_slider");
	}
	graphicUI->uiEndBlock();

	if (_gameEngine->isKeyJustPressed(KEY_SPACE)) _newSceneName = "MainMenu";
}

void Credits::_creditsWindow(
	GUI *graphicUI, std::string id, int x, int y, size_t width, size_t height,
	std::vector<std::tuple<std::string, std::string>> texts, int rowSize,
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
			graphicUI->uiHeader(std::get<0>(text).c_str(), NK_TEXT_CENTERED,
								rowSize, std::get<1>(text));
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
	static float timer = 0.0f;
	timer -= _gameEngine->getDeltaTime();
	if (timer > 0.0f) return;
	timer = 0.001f;
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
