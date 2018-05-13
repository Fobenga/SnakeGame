#include "MainWindow.h"
#include "Game.h"

game::game(MainWindow& wnd)
	:
	wnd_(wnd),
	gfx_(wnd),
	brd_(gfx_),
	snake_({ brd_.GetGridWidth() / 2, brd_.GetGridHeight() / 2}),
	rng_(std::random_device()()),
	goal_(rng_, brd_, snake_),
	score_(gfx_)
{
	snd_intro_.Play(1.0f);
}

void game::go()
{
	gfx_.BeginFrame();
	update_model();
	compose_frame();
	gfx_.EndFrame();
}

void game::update_model()
{
	const auto dt = ft_.Mark();

	if (game_state_ == running)
	{
		if (game_state_ != game_over)
		{
			// Movement control
			{
				wnd_.kbd.DisableAutorepeat();
				if (wnd_.kbd.KeyIsPressed(VK_UP) && p_current_direction_ != down)
				{
					is_up_being_pressed_ = true;
					p_current_direction_ = up;
					delta_loc_ = { 0,-1 };
				}
				else is_up_being_pressed_ = false;

				if (wnd_.kbd.KeyIsPressed(VK_DOWN) && p_current_direction_ != up)
				{
					is_down_being_pressed_ = true;
					p_current_direction_ = down;
					delta_loc_ = { 0,1 };
				}
				else is_down_being_pressed_ = false;

				if (wnd_.kbd.KeyIsPressed(VK_LEFT) && p_current_direction_ != right)
				{
					is_left_being_pressed_ = true;
					p_current_direction_ = left;
					delta_loc_ = { -1,0 };
				}
				else is_left_being_pressed_ = false;

				if (wnd_.kbd.KeyIsPressed(VK_RIGHT) && p_current_direction_ != left)
				{
					is_right_being_pressed_ = true;
					p_current_direction_ = right;
					delta_loc_ = { 1,0 };
				}
				else is_right_being_pressed_ = false;

				if (wnd_.kbd.KeyIsPressed(VK_SHIFT))
				{
					snake_mov_period_ *= instant_multiplier;
					is_shift_being_pressed_ = true;
				}
				else
				{
					is_shift_being_pressed_ = false;
					snake_mov_period_ = default_move_period;
				}

			}
		}

		{
			snake_mov_counter_ += dt;
			if (snake_mov_counter_ >= snake_mov_period_)
			{
				snake_mov_counter_ -= snake_mov_period_;
				const Location next = snake_.GetNextHeadLocation(delta_loc_);
				if (!brd_.IsInsideBoard(next) || snake_.IsInTileExceptEnd(next) || brd_.CheckForObstacle(next))
				{
					if (!brd_.IsInsideBoard(next)) death_by_wall_ = true;

					game_state_ = game_over;
					snd_dead_.Play();
					snd_musicloop_.StopAll();
				}
				else
				{
					if (next == goal_.GetLocation())
					{
						snake_.GrowAndMoveBy(delta_loc_);
						goal_.Respawn(rng_, brd_, snake_);
						if (ls_counter_ % 2 == 0 && ls_counter_ != 0)
						{
							for (auto i = 0; i < 2; ++i)
							{
								brd_.SpawnObstacle(rng_, snake_, goal_);
								obstacles_generated_++;
							}
						}

						if (brd_.CheckForObstacle(goal_.GetLocation()))
							goal_.Respawn(rng_, brd_, snake_);

						ss_++;
						snake_size_++;
						fruits_catched_++;

						score_.small_score_counter += ss_increase_ratio;

						if (score_.small_score_counter > ss_limit)
						{
							sfx_difpass_.Play(rng_, default_sfx_vol);
							new_stage_ = true;
							ls_counter_++;
							ls_++;
							score_.small_score_counter = s_padding;
							score_.long_score_counter += ls_increase_ratio;
						}
						else
						{
							sfx_feed_.Play(rng_, default_sfx_vol);
							new_stage_ = false;
						}
					}
					else
					{
						snake_.MoveBy(delta_loc_);
						total_movement_++;
						score_.small_score_counter -= 2;

						if (score_.small_score_counter <= s_padding) score_.small_score_counter = s_padding;

						if (fruits_catched_ >= 1 && score_.small_score_counter <= s_padding && !new_stage_)
						{
							snd_dead_.Play();
							game_state_ = game_over;
							death_by_time_ = true;
							snd_musicloop_.StopAll();
						}
					}
				}
			}
			snake_mov_period_ = std::max(snake_mov_period_ - dt * snake_velocity_factor_, snake_mov_period_min);
		}
	}
	else if (game_state_ == standby)
	{

		if (wnd_.kbd.KeyIsPressed(VK_RIGHT) ||
			wnd_.kbd.KeyIsPressed(VK_LEFT) ||
			wnd_.kbd.KeyIsPressed(VK_UP) ||
			wnd_.kbd.KeyIsPressed(VK_DOWN) ||
			wnd_.kbd.KeyIsPressed(VK_SHIFT))
		{
			wnd_.kbd.KeyIsEmpty();
		}
		if (wnd_.kbd.KeyIsPressed('X'))
			wnd_.Kill();

		if (wnd_.kbd.KeyIsPressed(VK_RETURN))
		{
			snd_intro_.StopOne();
			snd_musicloop_.Play(1.0f, 0.6f);
			game_state_ = running;
		}
	}
}

void game::compose_frame()
{
	switch (game_state_)
	{
	case running:
	{
		// animated intro 
		{

			x_enterkey_pos_ += 30;
			x_game_title_pos_ -= 30;

			if (fruits_catched_ < 1)
			{
				gfx_.DrawSprite(x_game_title_pos_, y_game_title_pos_, game_title_.get_rect(), brd_.get_rect(), game_title_, key_chroma);
				gfx_.DrawSprite(x_enterkey_pos_, y_enterkey_pos_, enter_key_.get_rect(), brd_.get_rect(), enter_key_, key_chroma);
			}

			if (key_align_pos_x == x_final_pos)
			{
				key_align_pos_x = x_final_pos;

				if (is_shift_being_pressed_)
					gfx_.DrawRectDim(key_align_pos_x, shift_pos_y_, 50, 50, overlay_color);

				if (is_up_being_pressed_)
					gfx_.DrawRectDim(key_align_pos_x, up_pos_y_, 50, 50, overlay_color);

				if (is_left_being_pressed_)
					gfx_.DrawRectDim(key_align_pos_x, left_pos_y_, 50, 50, overlay_color);

				if (is_right_being_pressed_)
					gfx_.DrawRectDim(key_align_pos_x, right_pos_y_, 50, 50, overlay_color);

				if (is_down_being_pressed_)
					gfx_.DrawRectDim(key_align_pos_x, down_pos_y_, 50, 50, overlay_color);
			}
			else
				key_align_pos_x += 10;

				// key overlay sprites


			

			gfx_.DrawSprite(key_align_pos_x, left_pos_y_, left_key_.get_rect(),overlay_clip, left_key_, key_chroma);
			gfx_.DrawSprite(key_align_pos_x, right_pos_y_, right_key_.get_rect(), overlay_clip, right_key_, key_chroma);
			gfx_.DrawSprite(key_align_pos_x, down_pos_y_, down_key_.get_rect(), overlay_clip, down_key_, key_chroma);
			gfx_.DrawSprite(key_align_pos_x, shift_pos_y_, shift_key_.get_rect(), overlay_clip, shift_key_, key_chroma);
			gfx_.DrawSprite(key_align_pos_x, up_pos_y_, up_key_.get_rect(), overlay_clip, up_key_, key_chroma);

		}

		// game loop
		if (!snake_already_initialized_)
			for (auto i = 0; i < init_snake_s; i++)
				snake_.GrowAndMoveBy(delta_loc_);

		snake_.Draw(brd_);
		snake_already_initialized_ = true;

		goal_.Draw(brd_);
		brd_.DrawBorder();
		brd_.DrawObstacles();
		score_.draw_score(Colors::Red);
	}
	break;
	case standby:
	{
		if (x_enterkey_pos_ == x_ekey_end_pos)
			x_enterkey_pos_ = x_ekey_end_pos;
		else
			x_enterkey_pos_ += 50;

		if (x_game_title_pos_ == x_gtitle_end_pos)
			x_game_title_pos_ = x_gtitle_end_pos;
		else
			x_game_title_pos_ -= 50;


		gfx_.DrawSprite(x_game_title_pos_, y_game_title_pos_, game_title_.get_rect(), brd_.get_rect(), game_title_, key_chroma);
		gfx_.DrawSprite(x_enterkey_pos_, y_enterkey_pos_, enter_key_.get_rect(), brd_.get_rect(), enter_key_, key_chroma);
		brd_.DrawBorder();
	}
	break;
	case game_over:
	{
		static constexpr auto fruit_value = 300;
		int final_score;

		// final score calculation formulas
		if (fruits_catched_ <= 1)
			final_score = fruit_value * fruits_catched_;
		else if (ls_counter_ <= 1)
			final_score = static_cast<int>(fruit_value * sqrt(fruits_catched_)) - total_movement_;
		else
			final_score = static_cast<int>(fruit_value * sqrt(fruits_catched_) * obstacles_generated_ - total_movement_ * 2);

		std::wstring dbt;
		if (death_by_time_)
			dbt = L"DEATH BY TIME";
		else
			dbt = std::to_wstring(final_score);

		wnd_.ShowMessageBox_OK(L"GAME OVER! DIFFICULTY " + std::to_wstring(ls_counter_),
							   L"----------------------------\n"
							   "Fruits Catched: " + std::to_wstring(fruits_catched_) +
							   L"\n----------------------------\n"
							   "Obstacles: " + std::to_wstring(obstacles_generated_) +
							   L"\n----------------------------\n"
							   "Final Score: " + dbt +
							   L"\n----------------------------\n");

		const int ask_restart = wnd_.ShowMessageBox_YN(L"Game Restart", L"Do you want to restart the game?");

		switch (ask_restart)
		{
		case IDYES:
		{
			game t_g(wnd_);

			// glitch handler / closes window
			if ((is_left_being_pressed_ && p_current_direction_ == left ||
				 is_right_being_pressed_ && p_current_direction_ == right ||
				 is_down_being_pressed_ && p_current_direction_ == down ||
				 is_up_being_pressed_ && p_current_direction_ == up) && !death_by_wall_)
			{
				const int glitch = wnd_.ShowMessageBox_OK(L"ERROR: Snake Initialization Error", L"An error ocurred trying to restart the game.\n"
														  "                     Please open it again.");
				if (glitch == IDOK)
				{
					wnd_.Kill();
				}
			}

			// if (wnd_.kbd.KeyIsPressed(VK_UP) ||
			// 	wnd_.kbd.KeyIsPressed(VK_DOWN) ||
			// 	wnd_.kbd.KeyIsPressed(VK_LEFT) ||
			// 	wnd_.kbd.KeyIsPressed(VK_RIGHT) ||
			// 	wnd_.kbd.KeyIsPressed(VK_SHIFT))
			// {
			// 	wnd_.kbd.Flush();
			// }

			if (restarting)
			{
				while (wnd_.ProcessMessage())
					t_g.go();

				restarting = false;
				wnd_.Kill();
			}

		}
		case IDNO:
		{
			wnd_.Kill();
		}
		break;
		default:
			break;
		}
	}
	break;
	default: break;
	}
}