#include "GameLogic.hpp"
#include "GameRenderer.hpp"

#include "Player.hpp" // TODO: this should be done by a "game initializer" and not by the game engine !!

// === CONSTRUCTOR =============================================================
GameLogic::GameLogic()
{
	canRun = false;
	running = false;

	// Map size parsing
	map_w = MAP_SIZE;
	map_h = MAP_SIZE;
	int maxSizeW = (WINDOW_W - 2 * WINDOW_MIN_X_OFFSET) / map_w;
	int maxSizeH = (WINDOW_H - 2 * WINDOW_MIN_Y_OFFSET) / map_h;
	square_size = (maxSizeH < maxSizeW) ? maxSizeH : maxSizeW;
	x_offset = (WINDOW_W - square_size * map_w) / 2;
	y_offset = (WINDOW_H - square_size * map_h) / 2;

	// Create interface class
	graphic_lib = new GameRenderer(this);

	// Create audio manager
	audio_manager = new AudioManager();

	// TODO: this should be done by a "game initializer" and not by the game engine !!
	entities.push_back(new Player());

	// Everything good
	canRun = true;
}

GameLogic::GameLogic(GameLogic const &src)
{
	*this = src;
	return;
}

GameLogic::~GameLogic(void)
{
	for (auto entity : entities)
	{
		delete entity;
	}
	delete (audio_manager);
	delete (graphic_lib);
	return;
}

int GameLogic::get_square_size(void)
{
	return square_size;
}

int GameLogic::get_x_offset(void)
{
	return x_offset;
}

int GameLogic::get_y_offset(void)
{
	return y_offset;
}

int GameLogic::get_map_w(void)
{
	return map_w;
}

int GameLogic::get_map_h(void)
{
	return map_h;
}

int GameLogic::get_player_direction(void)
{
	return player_direction;
}

bool GameLogic::get_if_is_player_alive(void)
{
	return is_player_alive;
}

std::tuple<int, int> &GameLogic::get_player_pos(void)
{
	return player_pos;
}

// === END GETTER ==============================================================

// === OPERATORS ===============================================================

GameLogic &GameLogic::operator=(GameLogic const &rhs)
{
	this->canRun = rhs.canRun;
	return *this;
}

void GameLogic::print_usage(void)
{
	std::cerr << "Usage: ./bomberman" << std::endl;
}

void GameLogic::change_library_request(std::string key_code)
{
	int requested_index = std::stoi(key_code);

	// std::cout << "Change index of library to: " << requested_index << std::endl;
	if (requested_index >= 0 && requested_index <= 0)
	{
		dl_index = requested_index;
	}
}

void GameLogic::update_game_state(void)
{
	// Get all pool events in library
	if (graphic_lib)
	{
		graphic_lib->GET_USER_INPUT_FUNC();
	}

	// Check if we want to close window, in this case no need to do further calculations
	if (dl_index == 0)
	{
		// std::cout << "Need to close.." << std::endl;
		running = false;
	}

	if (is_player_alive && running)
	{
		if (player_direction_requested > 0)
		{
			player_direction = player_direction_requested;
			player_direction_requested = -1;

			if (player_can_move())
			{
				move_player(player_pos, player_direction);
			}

			//player actual moving
			if (!is_player_alive)
			{
				std::cerr << "Game Over ! (Press 'R' to restart)" << std::endl;
				audio_manager->DEATH_SOUND_FUNC();
			}
		}
	}
}

int GameLogic::update_gui(void)
{
	if (dl_index < 0 || dl_index > 1)
	{
		std::cerr << "Wrong number given..!" << std::endl;
		return EXIT_FAILURE;
	}

	// Draw window with game infos
	if (graphic_lib && !has_shown_death)
	{
		graphic_lib->REFRESH_WINDOW_FUNC();
		if (!is_player_alive)
			has_shown_death = true;
	}

	return EXIT_SUCCESS;
}

void GameLogic::regulate_frame_sleep(void)
{
	// Timer logic, make thread sleep if needed
	past_frame_length = difftime(timer, time(NULL));
	if (past_frame_length < frame_time)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((frame_time - past_frame_length) * 1000)));
	}
	// std::cout << "frame" << std::endl;
	timer = time(NULL);
}

void GameLogic::init_player(void)
{
	is_player_alive = true;
	has_shown_death = false;

	player_pos = std::tuple<int, int>();

	player_pos = std::make_tuple(map_w / 2, map_h / 2);
	player_direction = (map_w > map_h) ? LEFT : DOWN;
	player_direction_requested = -1;
}

bool GameLogic::player_can_move(void)
{
	// Check for player
	int headX = std::get<0>(player_pos);
	int headY = std::get<1>(player_pos);
	if (player_direction == UP)
		headY -= 1;
	else if (player_direction == DOWN)
		headY += 1;
	else if (player_direction == LEFT)
		headX -= 1;
	else if (player_direction == RIGHT)
		headX += 1;

	// Check if he goes outside the map
	if (collide_with_walls)
	{
		if (headX == map_w || headY == map_h ||
			headX < 0 || headY < 0)
		{
			return false;
		}
	}

	return true;
}

void GameLogic::move_player(std::tuple<int, int> &player_pos, int &player_dir)
{
	// Advance based on direction
	if (player_dir == UP)
		std::get<1>(player_pos) = std::get<1>(player_pos) - 1;
	else if (player_dir == DOWN)
		std::get<1>(player_pos) = std::get<1>(player_pos) + 1;
	else if (player_dir == LEFT)
		std::get<0>(player_pos) = std::get<0>(player_pos) - 1;
	else if (player_dir == RIGHT)
		std::get<0>(player_pos) = std::get<0>(player_pos) + 1;
}

void GameLogic::change_direction_to(int &player_direction, int &player_direction_requested, int newDir)
{
	player_direction_requested = newDir;
	if ((newDir == UP || newDir == DOWN) && (player_direction == LEFT || player_direction == RIGHT))
	{
		player_direction_requested = newDir;
	}
	else if ((newDir == LEFT || newDir == RIGHT) && (player_direction == UP || player_direction == DOWN))
	{
		player_direction_requested = newDir;
	}
}

// === END PRIVATE FUNCS =======================================================

// === PUBLIC FUNCS ============================================================
int GameLogic::run(void)
{
	if (!canRun)
		return EXIT_FAILURE;
	int gui_ret;

	// init vars
	running = true;
	restart_request = false;
	frame_time = FRAME_TIME;
	dl_index = 1;
	dl_pastIndex = -1;
	timer = time(NULL);

	init_player();

	audio_manager->START_SOUND_FUNC();

	// Start game loop
	while (running)
	{
		std::cout << "------- New Frame -------------------" << std::endl;
		for (auto entity : entities)
		{
			std::cout << "Calling Update on entity" << std::endl;
			entity->Update();
		}

		// std::cout << "-- Frame --" << std::endl;
		update_game_state();

		gui_ret = update_gui();
		if (gui_ret != EXIT_SUCCESS || !running)
			break;

		regulate_frame_sleep();
		// running = false;
	}
	if (restart_request)
	{
		std::cout << "Starting new game !" << std::endl;
		return run();
	}
	return gui_ret;
}

void GameLogic::button_pressed(const char *button)
{
	std::string key = !button ? KEY_ESCAPE : std::string(button); // GLFW sends NULL pointer for Escape key..

	// std::cout << "key '" << key << "' was pressed" << std::endl;
	std::list<std::string>::const_iterator iter = std::find(change_library_keys.begin(), change_library_keys.end(), key);
	if (iter != change_library_keys.end())
	{
		change_library_request(key);
	}
	else
	{
		for (const std::tuple<std::string, int> &change_direction_pair : change_direction_keys) // access by reference to avoid copying
		{
			if (std::get<0>(change_direction_pair).compare(key) == 0)
			{
				change_direction_to(player_direction, player_direction_requested, std::get<1>(change_direction_pair));
				return;
			}
		}
		if (!is_player_alive)
		{
			if (key.compare("R") == 0 || key.compare("r") == 0)
			{
				change_library_request("0");
				restart_request = true;
			}
		}
		// std::cout << "value not useful.." << std::endl;
	}
}

// === END PUBLIC FUNCS ========================================================

// === STATICVARS ==============================================================
static std::list<std::string> generate_library_keys()
{ // static here is "internal linkage"
	std::list<std::string> p;
	p.push_front(KEY_0);
	return p;
}
const std::list<std::string> GameLogic::change_library_keys = generate_library_keys();

static std::vector<std::tuple<std::string, int>> generate_direction_keys()
{ // static here is "internal linkage"
	std::vector<std::tuple<std::string, int>> p;
	p.push_back(std::make_tuple(KEY_W, UP));
	p.push_back(std::make_tuple(KEY_A, LEFT));
	p.push_back(std::make_tuple(KEY_S, DOWN));
	p.push_back(std::make_tuple(KEY_D, RIGHT));
	p.push_back(std::make_tuple(KEY_W_LOWER, UP));
	p.push_back(std::make_tuple(KEY_A_LOWER, LEFT));
	p.push_back(std::make_tuple(KEY_S_LOWER, DOWN));
	p.push_back(std::make_tuple(KEY_D_LOWER, RIGHT));
	return p;
}
const std::vector<std::tuple<std::string, int>> GameLogic::change_direction_keys = generate_direction_keys();
// === END STATICVARS ==========================================================