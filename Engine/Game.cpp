#include "MainWindow.h"
#include "Game.h"

game::game(MainWindow& wnd)
	:
	wnd_(wnd),
	gfx_(wnd),
	brd_(gfx_),
	snake_({ 2,2 }),
	rng_(std::random_device()()),
	goal_(rng_, brd_, snake_),
	score_(gfx_)
{}

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
				if (wnd_.kbd.KeyIsPressed(VK_UP) && p_current_direction_ != down)
				{
					p_current_direction_ = up;
					delta_loc_ = { 0,-1 };
				}
				else if (wnd_.kbd.KeyIsPressed(VK_DOWN) && p_current_direction_ != up)
				{
					p_current_direction_ = down;
					delta_loc_ = { 0,1 };
				}
				else if (wnd_.kbd.KeyIsPressed(VK_LEFT) && p_current_direction_ != right)
				{
					p_current_direction_ = left;
					delta_loc_ = { -1,0 };
				}
				else if (wnd_.kbd.KeyIsPressed(VK_RIGHT) && p_current_direction_ != left)
				{
					p_current_direction_ = right;
					delta_loc_ = { 1,0 };
				}

				if (wnd_.kbd.KeyIsPressed(VK_SHIFT))
				{
					snake_mov_period_ *= instant_multiplier;
				}
				else snake_mov_period_ = default_move_period;
			}
		}
			// Timing and Score configurations
		{
			snake_mov_counter_ += dt;
			if (snake_mov_counter_ >= snake_mov_period_)
			{
				snake_mov_counter_ -= snake_mov_period_;
				const Location next = snake_.GetNextHeadLocation(delta_loc_);
				if (!brd_.IsInsideBoard(next) || snake_.IsInTileExceptEnd(next) || brd_.CheckForObstacle(next))
				{
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

						for (auto i = 1; i <= ls_counter_; ++i)
						{
							brd_.SpawnObstacle(rng_, snake_, goal_);
							obstacles_generated_++;
						}


						if (brd_.CheckForObstacle(goal_.GetLocation()))
							goal_.Respawn(rng_, brd_, snake_);

						ss_++;
						snake_size_++;
						fruits_catched_++;

						score_.small_score_counter += ss_increase_ratio;

						if (score_.small_score_counter > ss_limit)
						{
							sfx_difpass_.Play(rng_, 0.8f);
							new_stage_ = true;
							ls_counter_++;
							ls_++;
							score_.small_score_counter = s_padding;
							score_.long_score_counter += ls_increase_ratio;
						}
						else
						{
							sfx_feed_.Play(rng_, 0.8f);
							new_stage_ = false;
						}
					}
					else
					{
						snake_.MoveBy(delta_loc_);
						total_movement_++;
						score_.small_score_counter -= 2; // small score decrease ratio

						if (score_.small_score_counter <= s_padding) score_.small_score_counter = s_padding;

						if (fruits_catched_ >= 1 && score_.small_score_counter <= s_padding && !new_stage_)
						{
							snd_dead_.Play();
							game_state_ = game_over;
							death_by_time_ = true;
						}
					}
				}
			}
			snake_mov_period_ = std::max(snake_mov_period_ - dt * snake_velocity_factor_, snake_mov_period_min);
		}
	}
	else if (game_state_ == standby)
	{
		gfx_.DrawSprite(Graphics::ScreenWidth / 2 - brd_.GetGridWidth()/2 - enter_key_.get_width() / 2, 
						Graphics::ScreenHeight / 2 - brd_.GetGridHeight()/2 - enter_key_.get_height() / 2,
						enter_key_.get_rect(), enter_key_);

		if (wnd_.kbd.KeyIsPressed(VK_RETURN))
		{
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
		snake_.Draw(brd_);
		goal_.Draw(brd_);
		brd_.DrawBorder();
		brd_.DrawObstacles();
		score_.draw_score(Colors::Red);

		if (!snake_already_initialized_)
			for (auto i = 0; i < init_snake_s; i++)
				snake_.GrowAndMoveBy(delta_loc_);

		snake_already_initialized_ = true;
	}
	break;
	case standby:
		brd_.DrawBorder();
		break;
	case game_over:
	{

	// todo: gonna create another sprites later
	// SpriteCodex::DrawGameOver(350, 265, gfx);

		/*
		 * In order to restart tha game:
		 * 1 - put the snake in the initial position
		 * 2 - clear the snake segments
		 * 3 - clear the score
		 * 4 - clear the board
		 * 5 - clear the obstacles
		 */

		static constexpr auto fruit_value = 300;
		int final_score;

		// three formulas to calculate the final score 
		if (fruits_catched_ <= 1)
			final_score = fruit_value * fruits_catched_;
		else if (ls_counter_ < 1)
			final_score = static_cast<int>(fruit_value * sqrt(fruits_catched_)) - total_movement_;
		else
			final_score = static_cast<int>(fruit_value * sqrt(fruits_catched_) * obstacles_generated_ - total_movement_ * 2);

		std::wstring dbt;
		if (death_by_time_)
			dbt = L"DEATH BY TIME";
		else
			dbt = std::to_wstring(final_score);

		wnd_.ShowMessageBox(L"GAME OVER! DIFFICULTY " + std::to_wstring(ls_counter_),
							L"Fruits Eaten: " + std::to_wstring(fruits_catched_) +
							L"\n----------------------------\n"
							"Obstacles: " + std::to_wstring(obstacles_generated_) +
							L"\n----------------------------\n"
							"Final Score: " + dbt);

		wnd_.Kill();
	}
	break;
	default: break;
	}
}