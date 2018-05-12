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

class game
{
public:
	explicit game(class MainWindow& wnd);
	game(const game&) = delete;
	game& operator=(const game&) = delete;
	void go();


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

	// set the state of the game
	enum game_state { standby, game_over, running };
	int game_state_ = standby;

	// sound effect calls
	SoundEffect sfx_feed_ = SoundEffect({ L"sound\\feed.wav" }, false, 0);
	Sound snd_musicloop_ = Sound(L"sound\\music.wav", Sound::LoopType::AutoFullSound); // loop this forever
	Sound snd_dead_ = Sound(L"sound\\fail.wav");

	// snake movement variables
	static constexpr float snake_mov_period_min = 0.06f;
	static constexpr float instant_multiplier = 0.8f;
	static constexpr float default_move_period = 0.09f;
	static constexpr int init_snake_s = 3;
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
	static constexpr int ls_increase_ratio = 40;
	static constexpr int ss_amount_tofeed_ls = 5;
	static constexpr int ss_limit = Graphics::ScreenWidth - s_padding;
	static constexpr int ss_increase_ratio = ss_limit / ss_amount_tofeed_ls;

};