#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Board.h"
#include "Snake.h"
#include "Goal.h"
#include <random>
#include "SoundEffect.h"
#include "FrameTimer.h"
#include "Score.h"
#include "Surface.h"

class game
{
public:
	explicit game(class MainWindow& wnd);
	game(const game&) = delete;
	game& operator=(const game&) = delete;
	void go();
	~game()
	{
		delta_loc_ = { 1,0 };
	}

private:
	void compose_frame();
	void update_model();

	// constructor calls
	MainWindow & wnd_;
	Graphics gfx_;
	Board brd_;
	Snake snake_;
	Location delta_loc_ = { 1,0 };
	std::mt19937 rng_;
	FrameTimer ft_;
	Goal goal_;
	score score_;

	bool restarting = true;

	// set the state of the game
	enum game_state { standby = 0, game_over = 1, running = 2 };
	int game_state_ = standby;
	bool death_by_wall_ = false;

	// intro screen
	Surface enter_key_ = Surface(L"bmp\\enter_key.bmp");
	Surface game_title_ = Surface(L"bmp\\snake_title.bmp");

	// when window starts, do this
	int x_ekey_end_pos = Graphics::ScreenWidth / 2 - brd_.GetGridWidth() / 2 - enter_key_.get_width() / 2;
	int y_ekey_end_pos = Graphics::ScreenHeight / 2 - brd_.GetGridHeight() / 2 - enter_key_.get_height() / 2 + 30;
	int y_gtitle_end_pos = Graphics::ScreenHeight / 2 - brd_.GetGridHeight() / 2 - enter_key_.get_height() / 2 - 50;
	int x_gtitle_end_pos = Graphics::ScreenWidth / 2 - brd_.GetGridWidth() / 2 - enter_key_.get_width() / 2 - 50;

	int x_enterkey_pos_ = x_ekey_end_pos - 1000;
	int y_enterkey_pos_ = y_ekey_end_pos;
	int y_game_title_pos_ = y_gtitle_end_pos;
	int x_game_title_pos_ = x_gtitle_end_pos + 1000;


	// key overlay
	Surface shift_key_ = Surface(L"bmp\\shift_key.bmp");
	Surface left_key_ = Surface(L"bmp\\left_key.bmp");
	Surface right_key_ = Surface(L"bmp\\right_key.bmp");
	Surface down_key_ = Surface(L"bmp\\down_key.bmp");
	Surface up_key_ = Surface(L"bmp\\up_key.bmp");

	static constexpr Color overlay_color = { 80,80,80 };
	static constexpr Color key_chroma = Colors::Black;
	static constexpr int padding_between_keys = 60;
	int key_align_pos_x = 500;
	static constexpr int key_align_pos_y = 50;

	const int shift_pos_y_ = key_align_pos_y;
	const int up_pos_y_ = shift_pos_y_ + padding_between_keys;
	const int left_pos_y_ = up_pos_y_ + padding_between_keys;
	const int right_pos_y_ = left_pos_y_ + padding_between_keys;
	const int down_pos_y_ = right_pos_y_ + padding_between_keys;

	bool is_shift_being_pressed_ = false;
	bool is_left_being_pressed_ = false;
	bool is_right_being_pressed_ = false;
	bool is_down_being_pressed_ = false;
	bool is_up_being_pressed_ = false;

	// overlay animation at intro
	RectI overlay_clip = RectI({ 730,50 }, 50, 60 * 5);
	static constexpr int x_final_pos = 730;

	// sound effect calls
	SoundEffect sfx_feed_ = SoundEffect({ L"sound\\feed.wav" }, false, 0);
	SoundEffect sfx_difpass_ = SoundEffect({ L"sound\\difpass.wav" }, false, 0);
	static constexpr float default_sfx_vol = 0.8f;

	// sound calls
	Sound snd_intro_ = Sound(L"sound\\intro.wav", Sound::LoopType::AutoFullSound);
	Sound snd_musicloop_ = Sound(L"sound\\music.wav", Sound::LoopType::AutoFullSound); // loop this forever
	Sound snd_dead_ = Sound(L"sound\\fail.wav");

	// snake movement variables
	static constexpr float snake_mov_period_min = 0.06f;
	static constexpr float instant_multiplier = 0.8f;
	static constexpr float default_move_period = 0.09f;
	static constexpr int init_snake_s = 2;
	float snake_mov_period_ = default_move_period;
	float snake_mov_counter_ = 0.0f;
	int snake_size_ = init_snake_s + 1;
	float snake_velocity_factor_ = 0.001f;
	enum pos { up, down, left, right };
	int p_current_direction_ = right;
	bool snake_already_initialized_ = false;

	// score variables 
	int ss_;
	int ls_;
	int total_movement_ = 0;
	int fruits_catched_ = 0;
	int ls_counter_ = 0;
	int obstacles_generated_ = 0;
	bool new_stage_ = false;
	bool death_by_time_ = false;
	static constexpr int s_padding = 70;
	static constexpr int ls_increase_ratio = 80;
	static constexpr int ss_amount_tofeed_ls = 5;
	static constexpr int ss_limit = Graphics::ScreenWidth - s_padding;
	static constexpr int ss_increase_ratio = ss_limit / ss_amount_tofeed_ls;

};