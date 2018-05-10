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

	if (game_started_)
	{
		if (!game_over_)
		{
			// Behavior control
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

			// Moving timing and speed configurations
			{
				snake_mov_counter_ += dt;
				if (snake_mov_counter_ >= snake_mov_period_)
				{
					snake_mov_counter_ -= snake_mov_period_;
					const Location next = snake_.GetNextHeadLocation(delta_loc_);
					if (!brd_.IsInsideBoard(next) || snake_.IsInTileExceptEnd(next) || brd_.CheckForObstacle(next))
					{
						game_over_ = true;
						snd_dead_.Play();
						snd_musicloop_.StopAll();
					}
					else
					{
						if (next == goal_.GetLocation())
						{
							snake_.GrowAndMoveBy(delta_loc_);
							goal_.Respawn(rng_, brd_, snake_);
							brd_.SpawnObstacle(rng_, snake_, goal_);
							sfx_feed_.Play(rng_, 0.8f);

							ss_++;
							score_.small_score_counter += ss_increase_ratio;
							if (score_.small_score_counter > ss_limit)
							{
								ls_++;
								score_.small_score_counter = s_padding;
								score_.long_score_counter += ls_increase_ratio;
							}
						}
						else
						{
							snake_.MoveBy(delta_loc_);
						}
						sfx_slither_.Play(rng_, 0.08f);
					}
				}
				snake_mov_period_ = std::max(snake_mov_period_ - dt * snake_velocity_factor_, snake_mov_period_min);
			}
		}
	}
	else
	{
		if (wnd_.kbd.KeyIsPressed(VK_RETURN))
		{
			snd_musicloop_.Play(1.0f, 0.6f);
			game_started_ = true;
		}
	}

}

void game::compose_frame()
{

	if (game_started_)
	{
		snake_.Draw(brd_);

		// initializes snake with 2 body objects
		if (!snake_already_initialized_)
		{
			for (auto i = 0; i < 3; i++)
				snake_.GrowAndMoveBy(delta_loc_);
		}
		snake_already_initialized_ = true;

		goal_.Draw(brd_);
		brd_.DrawBorder();
		brd_.DrawObstacles();
		score_.draw_score(Colors::Red);

		if (game_over_)
		{
			// todo: gonna create another sprites later
			// SpriteCodex::DrawGameOver(350, 265, gfx);

			wnd_.ShowMessageBox(L"YOU DIED!", L"---GAME OVER---\nThanks for Playing!");
			wnd_.Kill();
		}
	}
	else
		brd_.DrawBorder();
}
