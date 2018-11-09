#pragma once

#include "header.hpp"
#include "AudioManager.hpp"
#include "AEntity.hpp"

#define MAP_SIZE 40

#define WINDOW_W 1100
#define WINDOW_H 1200
#define WINDOW_MIN_X_OFFSET 50
#define WINDOW_MIN_Y_OFFSET 100
#define MIN_SQUARE_SIZE 10
#define MAX_SQUARE_SIZE 25
#define OUTLINE_TICKNESS 2

#define GET_USER_INPUT_FUNC get_user_input
#define REFRESH_WINDOW_FUNC refresh_window
#define CLOSE_WINDOW_FUNC close_window

#define START_SOUND_FUNC play_start_sound
#define EAT_SOUND_FUNC play_eat_sound
#define DEATH_SOUND_FUNC play_death_sound

#define KEY_0 "0"
#define KEY_W "W"
#define KEY_A "A"
#define KEY_S "S"
#define KEY_D "D"
#define KEY_W_LOWER "w"
#define KEY_A_LOWER "a"
#define KEY_S_LOWER "s"
#define KEY_D_LOWER "d"
#define KEY_LEFT "Left"
#define KEY_UP "Up"
#define KEY_RIGHT "Right"
#define KEY_DOWN "Down"

#define KEY_ESCAPE "Escape"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define FRAME_TIME 0.1f

class GameRenderer;

class GameLogic
{
  private:
	static const std::list<std::string> change_library_keys;
	static const std::vector<std::tuple<std::string, int>> change_direction_keys;

	GameLogic(GameLogic const &src);

	GameLogic &operator=(GameLogic const &rhs);

	void print_usage(void);
	void change_library_request(std::string key_code);
	void update_game_state(void);
	int update_gui(void);
	void regulate_frame_sleep(void);
	void change_direction_to(int &player_direction, int &player_direction_requested, int newDir);

	void init_player(void);
	bool player_can_move(void);
	void move_player(std::tuple<int, int> &player_body, int &player_dir);

	// Graphic libraries vars
	GameRenderer *graphic_lib;
	int square_size;
	int x_offset;
	int y_offset;
	int dl_index;
	int dl_pastIndex;
	time_t timer;
	double past_frame_length;
	AudioManager *audio_manager;

	// Game model vars
	float frame_time;
	bool running;
	bool collide_with_walls;
	int map_h;
	int map_w;
	bool is_player_alive;
	bool has_shown_death;
	int player_direction;
	int player_direction_requested;
	bool restart_request;
	std::tuple<int, int> player_pos;
	std::chrono::high_resolution_clock::time_point creation_precise_time;

	std::vector<AEntity *> entities;

  public:
	GameLogic(void);
	~GameLogic(void);

	int run();
	void button_pressed(const char *button);

	int get_square_size(void);
	int get_x_offset(void);
	int get_y_offset(void);
	int get_map_w(void);
	int get_map_h(void);
	int get_player_direction(void);
	bool get_if_is_player_alive(void);
	std::tuple<int, int> &get_player_pos(void);

	bool canRun;
};
